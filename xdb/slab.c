/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

struct xdb_slab {
    int64_t     count;
    size_t      size;
};

xdb_slab_t* xdb_slab_create(size_t size) {
    xdb_slab_t *slb = malloc(sizeof(struct slab));
    if (slb != NULL) {
        slb->count = 0;
        slb->size = size;
    }

    return slb;
}

void xdb_slab_destroy(xdb_slab_t *slb) {
    ASSERT(slb != NULL);
    ASSERT(slb->count == 0);
    free(slb);
}

void *xdb_slab_alloc(xdb_slab_t *slb) {
    void *p;

    ASSERT(slb != NULL);

    p = malloc(slb);
    if (p == NULL) {
        ASSERT(0);
        exit(0);
    }
    slb->count++;
    return p;
}

void xdb_slab_free(xdb_slab_t *slb, void *buf) {
    ASSERT(slb != NULL);
    ASSERT(buf != NULL);
    slb->count--;
    free(buf);
}

