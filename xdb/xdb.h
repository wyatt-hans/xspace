/*
 * copyright (c) 2011 konghan, All rights reserved.
 */

#ifndef XSPACE_XDB_H_
#define XSPACE_XDB_H_

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct xdb          xdb_t;
typedef struct xdb_tab      xdb_tab_t;
typedef struct xdb_row      xdb_row_t;
typedef struct xdb_col      xdb_col_t;

typedef struct xdb_key {
    void    *xk_data;
    size_t  xk_size;
}xdb_key_t;

typedef struct xdb_val {
    union {
        void        *vdata;
        int8_t      i8data;
        int16_t     i16data;
        int32_t     i32data;
        int64_t     i64data;
        uint8_t     u8data;
        uint16_t    u16data;
        uint32_t    u32data;
        uint64_t    u64data;
    }uxv_data;
    size_t      xv_size;
    int         xv_type;
}xdb_val_t;

#define xv_vdata    uxv_data.vdata
#define xv_i8       uxv_data.i8data
#define xv_u8       uxv_data.u8data
#define xv_i16      uxv_data.i16data
#define xv_u16      uxv_data.u16data
#define xv_i32      uxv_data.i32data
#define xv_u32      uxv_data.u32data
#define xv_i64      uxv_data.i64data
#define xv_u64      uxv_data.u64data

extern void *xdb_zalloc(size_t size);
extern void xdb_free(void *buf);

extern int xdb_init();
extern int xdb_fini();

extern int xdb_open(const char *file, int mode, xdb_t **db);
extern int xdb_close(xdb_t *db);

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

#ifdef __cplusplus
}
#endif
#endif  // XSPACE_XDB_H_
