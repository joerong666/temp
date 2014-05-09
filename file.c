#include "inc.h"

rc_t scan_files(const char *path, int(*filter)(const struct dirent *),
                   int(*compar)(const struct dirent **, const struct dirent **), 
                   struct dirent ***flist, int *cnt)
{
    rc_t rc = RC_OK;

    *cnt = scandir(path, &flist, filter, compar ? compar : alphasort);
    GO_OUT_IF_FAIL(*cnt != -1, errno, "Scan %s fail", path); 

_out:
    return rc;
}

void free_files(struct dirent **flist, int cnt)
{
    while(cnt--) free(namelist[cnt]);
    free(namelist);
}

rc_t mkdir_if_need(const char *path)
{
    rc_t rc = RC_OK;
    int ret;

    if(access(path, F_OK) != 0) {
        ret = mkdir(path, 0755);
        GO_OUT_IF_FAIL(ret == 0, errno, "Mkdir %s fail", path);
    }

_out:
    return rc;
}

rc_t lock_or_unlock(int fd, BOOL lock)
{
    rc_t rc = RC_OK;
    int ret;

    struct flock f;
    memset(&f, 0, sizeof(struct flock));

    f.l_type = (lock ? F_WRLCK : F_UNLCK);
    f.l_whence = SEEK_SET;
    f.l_start = 0;
    f.l_len = 0;

    ret = fcntl(fd, F_SETLK, &f);
    GO_OUT_IF_FAIL(ret != -1, errno, "fcntl on %s fail", file);

_out:
    return rc;
}

rc_t random_access_file(int fd, char **map, size_t *size)
{
    void *p = NULL;
    int ret;
    rc_t rc = RC_OK;
    stat fst;

    ret = fstat(fd, &fst);
    GO_OUT_IF_FAIL(ret == 0, errno, "Stat fd(%d) fail", fd);

    p = mmap(NULL, fst.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    GO_OUT_IF_FAIL(p != MAP_FAILED, errno, "Mmap %s fail", file);
    
_out:
    if(rc == RC_OK) {
        *size = fst.st_size;
        *map = (char *)p;
    }

    return rc;
}

rc_t release_access(char *map, size_t size)
{
    rc_t rc = RC_OK;
    int ret;

    ret = munmap(map, size);
    GO_OUT_IF_FAIL(ret == 0, errno, "Munmap fail");
    
_out:
    return rc;
}
