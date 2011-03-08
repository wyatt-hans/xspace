/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "btree.h"
#include "slab.h"

static xbt_node_t *new_node(xdb_btree_t *xbt, int type) {
    xbt_node_t  *node;

    ASSERT(xbt != NULL);
    
    node = (xbt_node_t *)xdb_slab_alloc(xbt->xb_cache);
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(xbt_node_t));

    if (type & XDB_NODE_INNER) {
        node->xn_kmax = xbt->xb_kmax;
        node->xn_flags = XDB_NODE_INNER;
        node->xn_keys = (void *)(node + 1);
        node->xn_vals = node->xn_keys + node->xn_kmax*xbt->xb_ksize;
    } else {
        node->xn_kmax = xbt->xb_vmax;
        node->xn_keys = (void *)(node + 1);
        node->xn_vals = node->xn_keys + node->xn_kmax*xbt->xb_ksize;
    }

    return node;
}


int xdb_btree_create(uint16_t ksize, uint32_t vsize, uint32_t bsize,
        xdb_compare pfn, xdb_btree_t **xbt) {
    uint32_t    size = vsize > sizeof(void*) ? vsize : sizeof(void*);
    ASSERT(xbt != NULL);

    if (((ksize + size) * 2 + size + sizeof(xbt_node_t)) > bsize) {
        xdb_trace("bsize too small ksize:%d-vsize:%d-bsize:%d\n",
                ksize, vsize, bsize);
        return -EINVAL;
    }

    *xbt = malloc(xdb_btree_t);
    if (*xdb == NULL) {
        xdb_trace("no more memory!\n");
        return -ENOMEM;
    }
    memset(*xbt, 0, sizeof(xdb_btree_t));

    (*xbt)->xb_ksize = ksize;
    (*xbt)->xb_vsize = vsize;

    (*xbt)->xb_cache = xdb_slab_create(bsize);
    if ((*xbt)->xb_cache == NULL) {
        xdb_trace("create slab fail:%d!\n", bsize);
        free((*xbt));
        return -ENOMEM;
    }

    (*xbt)->xb_kmax = (bsize - sizeof(void*) - sizeof(xbt_node_t)) /
                        (ksize + sizeof(void*));
    (*xbt)->xb_vmax = (bsize - vsize - sizeof(xbt_node_t)) /
                        (ksize + vsize);
    if (pfn)
        (*xbt)->xb_comp = pfn;
    else
        (*xbt)->xb_comp = defaul_compare;

    (*xbt)->xb_root = new_node((*xbt), 0);
    if ((*xbt)->xb_root == NULL) {
        xdb_trace("create root node fail\n");
        xdb_slab_destroy((*xbt)->xb_cache);
        free((*xbt));
        return -ENOMEM;
    }
    return 0;
}

int xdb_btree_destroy(xdb_btree_t *xbt) {
    ASSERT(xbt != NULL);

    xdb_slab_destroy(xbt->xb_cache);
    if (xbt->xb_root != NULL) {
    }

    free(xbt);
    return 0;
}

static int btree_pos_left(xdb_btree_t *xbt, xdb_node_t *node,
            uint8_t *key, int *found) {
    int first = 0, second, cr;
    uint8_t *tmp;

    ASSERT(xbt != NULL);
    ASSERT(node != NULL);
    ASSERT(key != NULL);
    ASSERT(found != NULL);

    *found = 0;
    second = node->xn_knum >> 1;
    do {
        tmp = node->xn_keys + (first + second)*xbt->xb_ksize;
        cr = xbt->xb_compare(key, tmp, xbt->xb_ksize);
        if (cr == 0) {
            *found = 1;
            return (first + second);
        }

        if (cr > 0) {
            first += second;
            second = second >> 1;
        } else {
            second = second >> 1;
        }
    } while (second != 0);

    return (cr > 0) ? first : ((first == 0) ? 0 : first  - 1);
}

static uint16_t btree_pos_right(xdb_btree_t *xbt, xdb_node_t *node,
            uint8_t *key, int *found) {
    int first = 0, second, cr;
    uint8_t *tmp;

    ASSERT(xbt != NULL);
    ASSERT(node != NULL);
    ASSERT(key != NULL);
    ASSERT(found != NULL);

    *found = 0;
    second = node->xn_knum >> 1;
    do {
        tmp = node->xn_keys + (first + second)*xbt->xb_ksize;
        cr = xbt->xb_compare(key, tmp, xbt->xb_ksize);
        if (cr == 0) {
            *found = 1;
            return (first + second);
        }

        if (cr > 0) {
            first += second;
            second = second >> 1;
        } else {
            second = second >> 1;
        }
    } while (second != 0);

    return (cr > 0) ? first + 1 : first;
}

static int btree_find_leaf(xdb_btree_t *xbt, const uint8_t *key,
                uint16_t *index, xdb_node_t **node) {
    xdb_node_t  *n;
    int pos, found;

    ASSERT((xbt != NULL) && (xbt->xb_root != NULL));
    ASSERT(key != NULL);
    ASSERT(index != NULL);
    ASSERT(node != NULL);

    n = xbt->xb_root;
    do {
        pos = btree_pos_right(xbt, n, key, &found);
        *node = n;

        if (found) {
            ASSERT(cr == 0);
            if (n->xn_flags & XDB_NODE_INNER) {
                n = (xdb_node_t*)(n->xn_vals + pos*XDB_POINTER_SIZE);
            } else {
                *index = pos;
                return 0;
            }
        } else {
            if (n->xn_flags & XDB_NODE_INNER) {
                n = (xdb_node_t*)(n->xn_vals + pos*XDB_POINTER_SIZE);
            } else {
                *index = pos;
                return -ENOENT;
            }
        }
    } while (n != NULL);

    *index = pos;
    return -ENOENT;
}

static void btree_insert_leaf(xdb_btree_t *xbt, xdb_node_t *node,
            uint16_t index, const uint8_t *key, const uint8_t *val) {
    ASSERT(xbt != NULL);
    ASSERT(node != NULL);
    ASSERT(key != NULL);
    ASSERT(val != NULL);

    ASSERT(index <= node->xn_knum);

    if (index < node->xn_knum) {
        memmove((node->xn_keys + xbt->xb_ksize*(index + 1)),
                    (node->xn_keys + xbt->xb_ksize*index), 1);
        memmove((node->xn_vals + xbt->xb_vsize*(index + 1)),
                    (node->xn_vals + xbt->xb_vsize*index), 1);
    }
    memcpy(node->xn_keys + xbt->xb_ksize*index, key, xbt->xb_ksize);
    memcpy(node->xn_vals + xbt->xb_vsize*index, val, xbt->xb_vsize);
    node->xn_knum++;
    node->xn_dirty = 1;
    xbt->xb_count++;
}

static void btree_insert_inner(xdb_btree_t *xbt, xdb_node_t *np,
            const xdb_node_t *nd) {
    int     pos, found;

    ASSERT(xbt != NULL);
    ASSERT(np != NULL);
    ASSERT(nd != NULL);

    pos = btree_po_right(xbt, np, nd->xn_keys, &found);
    ASSERT(!found);
    ASSERT(pos <= np->xn_knum);

    if (pos < np->xn_knum) {
        memmove(np->xn_keys + (pos + 1)*xbt->xb_ksize,
                np->xn_keys + pos*xbt->xb_ksize, np->xn_knum - pos);
         memmove(np->xn_vols + (pos + 1)*XDB_POINTER_SIZE,
                np->xn_vols + pos*XDB_POINTER_SIZE, np->xn_knum - pos);
    }
    memcpy(np->xn_keys + pos*xbt->xb_ksize, nd->xn_keys, xbt->xb_ksize);
    (xdb_node_*)(np->xn_vals + pos*XDB_POINTER_SIZE) = nd;

    return 0;
}

static int btree_split_insert(xdb_btree_t *xbt, xdb_node_t *node,
        uint16_t cidx, const uint8_t *key, const uint8_t *val) {
    xdb_node_t  *nd, *np, *ni, *tmp;
    uint16_t    num, move;
    void        *ndk, *npk;
    int         i;

    ASSERT(xbt != NULL);
    ASSERT(node != NULL);
    ASSERT(key != NULL);
    ASSERT(val != NULL);

    nd = new_node(xbt, 0);
    if (nd == NULL) {
        xdb_trace("no more memory!\n");
        return -ENOMEM;
    }

    num = (node->xn_knum + 1) >> 1;
    move = node->xn_knum - num;
    memmove(nd->xn_keys, node->xn_keys + xbt->xb_ksize*num, move);
    memmove(nd->xn_vals, node->xn_vals + xbt->xb_vsize*num, move);
    node->xn_knum = num;
    nd->xn_knum = move;
    ndk = nd->xn_keys;

    if (cidx < num) {
        btree_insert_leaf(xbt, node, cidx, key, val);
    } else {
        btree_insert_leaf(xbt, nd, cidx - num, key, val);
    }

    np = node->xn_parent;
    while (np) {
        ASSERT((np->xn_flags & XDB_NODE_INNER) != 0);
        ASSERT(np->xn_knum >= (np->xn_kmax / 2));

         if (np->xn_knum < np->xn_kmax) {
             btree_insert_inner(xbt, np, nd);
             nd->xn_parent = np;
             return 0;
         } else {
            ni = new_node(xbt, 0);
            if (ni == NULL) {
                // FIXME:rollback splited leafs and inners
                xdb_trace("no more memory!\n");
                return -ENOMEM;
            }

            num = (np->xn_knum + 1) >> 1;
            move = np->xn_knum - num;
            memmove(ni->xn_keys, np->xn_keys + xbt->xb_ksize*num, move);
            memmove(ni->xn_vals, np->xn_vals + XDB_POINTER_SIZE*num, move);
            for (i = 0; i < move; i++) {
                tmp = (xdb_node_t *)(ni->xn_vals + XDB_POINTER_SIZE*i);
                tmp->xn_parent = ni;
            }
   
            np->xn_knum = num;
            ni->xn_knum = move;
            npk = ni->xn_keys;
   
            if (xbt->xb_compare(npk, ndk, xbt->xb_ksize) >= 0) {
                btree_insert_inner(xbt, ni, nd);
                nd->xn_parent = ni;
            } else {
                btree_insert_inner(xbt, np, nd);
                nd->xn_parent = np;
            }
            
            ndk = ni->xn_keys;
            nd = ni;
            np = np->xn_parent;
         }
    }

    // root have splited
    if (np == NULL) {
        xbt->xb_root = ni;
    }

    return 0;
}

int xdb_btree_insert(xdb_btree_t *xbt, const void *key,
        const void *val) {
    xdb_node_t      *node;
    uint16_t        cidx;
    int             found = 0;

    ASSERT(xbt != NULL);
    ASSERT(key != NULL);
    ASSERT(val != NULL);
    
    found = btree_find_leaf(xdb, key, &cidx, &node);
    ASSERT(node != NULL);
    ASSERT((node->xn_flags & XDB_NODE_INNER) == 0);

    if (found) {
        return -EEXIST;
    }

    if (node->xn_knum < node->xn_kmax) {
        btree_insert_leaf(xbt, node, cidx, key, val);
        return 0;
    } else {
        return btree_split_insert(xbt, node, cidx, key, val);
    }
}

int xdb_btree_delete(xdb_btree_t *xbt, const uint8_t *key) {
    xdb_node_t *node, *left = NULL, *right = NULL;
    xdb_node_t *pleft = NULL, *pright = NULL;
    uint16_t    pos;
    int         found;

    ASSERT(xbt != NULL);
    ASSERT(key != NULL);

    node = xbt->xb_root;

    do {
        pos = btree_pos_right(xbt, node, key, &found);
        if (found) {
        }
    } while (node != NULL);
}

int xdb_btree_find(xdb_btree_t *xbt, const uint8_t *key, uint8_t *val) {
    xdb_node_t *node;

    ASSERT(xbt != NULL);
    ASSERT(key != NULL);
    ASSERT(vol != NULL);

    if (btree_find(xbt, key, &pos, &node) != 0) {
        return -ENOENT;
    }

    memcpy(val, node->xn_val + pos*xbt->xb_vsize, xbt->xb_vsize);
    return 0;
}


