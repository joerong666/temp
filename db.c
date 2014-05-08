#include "db.h"

struct db {
    char *path;
}

db_t *db_create(const char *path)
{
    db_t *db;
    rc_t rc = RC_OK;

    db = Malloc(sizeof(db_t));
    OP_GO_OUT_IF_FAIL(db != NULL, rc = RC_ER, errno, "Malloc fail");
    
    db->path = Strdup(path);
    OP_GO_OUT_IF_FAIL(db->path != NULL, rc = RC_ER, errno, "Strdup db path fail");

    

_out:
    if(rc != RC_OK) db_destroy(&db);

    return db;
}

void db_destroy(db_t **db)
{
    db_t *d_ = *db;

    if(!d_) return;

    Free(d_->path);

    /* free and set NULL */
    Free(*db);
}

rc_t recover_from_manifest(db_t *db)
{
    
}

rc_t recover_from_binlog(db_t *db)
{

}

rc_t db_recover(db_t * db)
{
    rc_t ret, rc = RC_OK;

    ret = recover_from_manifest(db);
    GO_OUT_IF_FAIL(ret == RC_OK, L_ERROR, "Recover from manifest fail");

    ret = recover_from_binlog(db);
    GO_OUT_IF_FAIL(ret == RC_OK, L_ERROR, "Recover from binglog fail");
    
_out:
    return rc;
}

db_t *db_open(const char *path)
{
    db_t *db;
    rc_t ret, rc = RC_OK;

    db = db_create(path);
    OP_GO_OUT_IF_FAIL(db != NULL, rc = RC_ER, E_FATAL, "Create db fail");

    ret = lock_db(db);
    GO_OUT_IF_FAIL(ret == RC_OK, E_FATAL, "Lock db fail");

    ret = db_recover(db);
    GO_OUT_IF_FAIL(ret == RC_OK, E_FATAL, "Recover db fail");

_out:
    if(rc != RC_OK) db_destroy(db);

    return db;
}

void db_close(db_t *db)
{
    db_destroy(db);
}

rc_t db_put(db_t *db, const char *key, i32_t key_size, const char *val, i32_t val_size);

rc_t db_del(db_t *db, const char *key, i32_t key_size);

rc_t db_get(db_t *db, const char *key, i32_t key_size,
            char **val, i32_t *val_size, Malloc_Func);

rc_t db_set_rulefilter(db_t *db, func_rulefilter rulefilter);

rc_t db_load_dir(db_t *db, const char *dir);

rc_t db_clean_file(db_t *db, const char *time);

rc_t db_freeze(db_t *db);

rc_t db_unfreeze(db_t *db);


