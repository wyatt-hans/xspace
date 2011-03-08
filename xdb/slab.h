/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef XSPACE_XDB_SLAB_H_
#define XSPACE_XDB_SLAB_H_

typedef struct xdb_slab xdb_slab_t;

xdb_slab_t* slab_create(size_t size);
void xdb_slab_destroy(xdb_slab_t *slb);
void *xdb_slab_alloc(xdb_slab_t *slb);
void xdb_slab_free(xdb_slab_t *slb, void *buf);

#endif  // XSPACE_XDB_SLAB_H_
