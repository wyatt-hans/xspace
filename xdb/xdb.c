/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#include "xdb.h"

struct xdb {
    FILE    *xd_fd;
    int     mode;
};

struct xdb_tab {
    struct xdb *xt_db;
    int64_t     xt_rows;
    size_t      xt_ksize;
    int64_t     xt_size;
};

struct xdb_row {
    struct xdb_tab *xr_tab;
    int             xr_cols;
};

struct xdb_col {
    struct xdb_row  *xc_row;
    int             xc_index;
    void            *xc_data;
    size_t          *xc_size;

#define XDB_COL_TYPE_DATA       0x00000001
#define XDB_COL_TYPE_I8         0x00000002
#define XDB_COL_TYPE_U8         0x00000004
#define XDB_COL_TYPE_I16        0x00000008
#define XDB_COL_TYPE_U16        0x00000010
#define XDB_COL_TYPE_I32        0x00000020
#define XDB_COL_TYPE_U32        0x00000040
#define XDB_COL_TYPE_I64        0x00000080
#define XDB_COL_TYPE_U64        0x00000100
#define XDB_COL_TYPE_TAB        0x80000000
    int             xc_flags;
};

void *xdb_zalloc(size_t size) {
    void *p = malloc(size);
    if (p != NULL) {
        memset(p, 0, size);
    }
    return p;
}

void xdb_free(void *buf) {
    free(buf);
}

typedef struct slab {
    int64_t     count;
    size_t      size;
}slab_t;

static slab_t* slab_create(size_t size) {
    slab_t *slb = malloc(sizeof(struct slab));
    if (slb != NULL) {
        slb->count = 0;
        slb->size = size;
    }

    return slb;
}

static void slab_destroy(slab_t *slb) {
    ASSERT(slb != NULL);
    ASSERT(slb->count == 0);
    free(slb);
}

static void *slab_alloc(slab_t *slb) {
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

static void slab_free(slab_t *slb, void *buf) {
    ASSERT(slb != NULL);
    ASSERT(buf != NULL);
    slb->count--;
    free(buf);
}

static slab_t *kslab_tab_cache = NULL;
static slab_t *kslab_row_cache = NULL;
static slab_t *kslab_col_cache = NULL;

int xdb_init() {
    kslab_tab_cache = slab_create(sizeof(struct xdb_tab));
    if (kslab_tab_cache == NULL) {
        return -1;
    }

    kslab_row_cache = slab_create(sizeof(struct xdb_row));
    if (kslab_row_cache == NULL) {
        slab_destroy(kslab_tab_cache);
        kslab_tab_cache = NULL;
        return -1;
    }

    kslab_col_cache = slab_create(sizeof(struct xdb_col));
    if (kslab_col_cache == NULL) {
        slab_destroy(kslab_tab_cache);
        kslab_tab_cache = NULL;
        slab_destroy(kslab_row_cache);
        kslab_row_cache = NULL;
        return -1;
    }

    return 0;
}

int xdb_fini() {
    slab_destroy(kslab_tab_cache);
    kslab_tab_cache = NULL;
    slab_destroy(kslab_row_cache);
    kslab_row_cache = NULL;
    slab_destroy(kslab_col_cache);
    kslab_col_cache = NULL;

    return 0;
}

int xdb_open(const char *file, int mode, xdb_t **db) {
}

int xdb_close(xdb_t *db) {
}

extern int xdb_tab_open(xdb_t *db, const char *name, int mode,
                xdb_tab_t **tab);
extern int xdb_tab_close(xdb_tab_t *tab);

extern int xdb_row_insert(xdb_tab_t *tab, const xdb_key_t *key,
                const xdb_col_t *col, int n, xdb_row_t **row);
extern int xdb_row_delete(xdb_tab_t *tab, const xdb_key_t *key,
                int flags);

extern int xdb_row_fetch(xdb_tab_t *tab, const xdb_key_t *key,
                int flags, xdb_row_t **row);
extern int xdb_row_first(xdb_tab_t *tab, xdb_row_t **row);
extern int xdb_row_last(xdb_tab_t *tab, xdb_row_t **row);
extern int xdb_row_count(xdb_tab_t *tab, uint64_t *count);
extern int xdb_row_next(xdb_row_t **row);
extern int xdb_row_close(xdb_row_t *row);

extern int xdb_col_count(xdb_row_t *row);
extern int xdb_col_get(xdb_row_t *row, int idx, xdb_col_t **col);
extern int xdb_col_set(xdb_row_t *row, int idx, const xdb_col_t *col);
extern int xdb_col_add(xdb_row_t *row, int idx, const xdb_col_t *col);
extern int xdb_col_del(xdb_row_t *row, int idx);
extern int xdb_col_colse(xdb_col_t *col);

extern int xdb_col_opentable(xdb_row_t *row, int idx, int mode,
        xdb_tab_t **tab);
extern int xdb_col_closetable(xdb_tab_t *tab);

extern int xdb_col_setval(const xdb_val_t *val, xdb_col_t **col);
extern int xdb_col_getval(const xdb_col_t *col, xdb_val_t *val);
extern int xdb_col_setvals(const xdb_val_t **val, int n, xdb_col_t **col);
extern int xdb_col_getvals(const xdb_col_t **col, xdb_val_t **val, int n);

