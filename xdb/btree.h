/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "slab.h"

#ifndef XSPACE_XDB_BTREE_H_
#define XSPACE_XDB_BTREE_H_

#define XDB_NODE_INIT       0x01
#define XDB_NODE_LOAD       0x02
#define XDB_NODE_NONE       0x04
#define XDB_NODE_INNER      0x80

#define XDB_NODE_INVAL_INDEX    0xffff

#define XDB_POINTER_SIZE        (sizeof(void *))

typedef struct xdb_node {
    struct xdb_node     *xn_parent; // point to parent node
//  struct xdb_node     *xn_next;   // point to next node
//  struct xdb_node     *xn_prev;   //
    uint16_t            xn_kmax;    // max node in this node
    uint16_t            xn_knum;    // inserted key number
    uint8_t             xn_dirty;   // does dirty node
    uint8_t             xn_flags;   // flags
    uint8_t             *xn_keys;   // point to key array
    uint8_t             *xn_vals;   // point to val array
}xdb_node_t;

typedef int (*xdb_compare)(void *arg1, void *arg2, uint16_t size);

typedef struct xdb_btree {
    uint16_t            xb_ksize;   // key size of this btree
    uint16_t            xb_kmax;
    uint16_t            xb_vmax;
    uint32_t            xb_vsize;   // val size of this btree
    uint32_t            xb_bsize;   // 
    uint64_t            xb_count;   // all inserted items
    xdb_slab_t          *xb_cache;  // node alloc cache
    xdb_compare         xb_comp;    // compare function
    xdb_node_t          *xb_root;   // root of the tree
}xdb_btree_t;

extern int xdb_btree_create(uint16_t ksize, uint32_t vsize, uint32_t bsize,
        xdb_compare pfn, xdb_btree_t **xbt);
extern int xdb_btree_destroy(xdb_btree_t *xbt);

extern int xdb_btree_insert(xdb_btree_t *xbt, const void *key,
        const void *val);
extern int xdb_btree_delete(xdb_btree_t *xbt, const void *key);

extern int xdb_btree_find(xdb_btree_t *xbt, const void *key, void *val);

// int xdb_btree_setkey(xdb_btree_t *xdb, void *key, uint16_t ksize,
//         void *val, uint32_t vsize);

#endif  // XSPACE_XDB_BTREE_H_

