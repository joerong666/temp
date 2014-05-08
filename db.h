#ifndef _STORAGE_HIDB_H_
#define _STORAGE_HIDB_H_

#ifndef FOR_UNIT_TEST
    #include "prod_inc.h"
#else
    #include "dev_inc.h"
#endif

typedef struct db db_t;
typedef rc_t (*func_rulefilter)(char *);

db_t *db_open(const char *path);

void db_close(db_t *db);

rc_t db_put(db_t *db, const char *key, i32_t key_size, const char *val, i32_t val_size);

rc_t db_del(db_t *db, const char *key, i32_t key_size);

rc_t db_get(db_t *db, const char *key, i32_t key_size,
            char **val, i32_t *val_size, Malloc_Func);

rc_t db_set_rulefilter(db_t *db, func_rulefilter rulefilter);

rc_t db_load_dir(db_t *db, const char *dir);

/*
* time format: 2013-12-10 16:47:00
*/
rc_t db_clean_file(db_t *db, const char *time);

rc_t db_freeze(db_t *db);

rc_t db_unfreeze(db_t *db);

#endif
