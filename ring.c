#include "ring.h"

struct ring {
    int r_cursor;
    int w_cursor;
    int capacity;
    void *data[1];
};

ring_t *ring_create(int capacity)
{
    ring_t *thiz = NULL;
    int _cap;

    OP_GO_OUT_IF_FAIL(capacity > 0, NOP, L_ERROR, "Invalid args: capacity(%d)", capacity);

    _cap = capacity + 1; /* leave one ele not used */
    thiz = (ring_t *)Malloc(offsetof(ring_t, data) + _cap * sizeof(void*));
    OP_GO_OUT_IF_FAIL(thiz != NULL, NOP, errno, "Malloc fail");

    thiz->r_cursor = 0;
    thiz->w_cursor = 0;
    thiz->capacity = _cap;

_out:
    return thiz;
}

rc_t ring_push(ring_t *thiz, void *data)
{
    rc_t rc = RC_OK;
    int w_cursor = 0;

    GO_OUT_IF_FAIL(thiz != NULL, L_ERROR, "Ring pointer is NULL");

    w_cursor = (thiz->w_cursor + 1) % thiz->capacity;
    GO_OUT_IF_FAIL(w_cursor != thiz->r_cursor, L_WARN, "Ring is full");

    thiz->data[thiz->w_cursor] = data;
    thiz->w_cursor = w_cursor;

_out:
    return rc;  
}

rc_t ring_pop(ring_t *thiz, void **data)
{
    rc_t rc = RC_OK;

    GO_OUT_IF_FAIL(thiz != NULL && data != NULL, L_WARN, "Invalid args:thiz(%p),data(%p)", thiz, data);

    GO_OUT_IF_FAIL(thiz->r_cursor != thiz->w_cursor, L_WARN, "Ring is empty");

    *data = thiz->data[thiz->r_cursor];
    thiz->r_cursor = (thiz->r_cursor + 1) % thiz->capacity;

_out:
    return rc;
}

void ring_destroy(ring_t *thiz)
{
    Free(thiz);
}

#if defined(FOR_UNIT_TEST) && defined(TEST_RING)
int main(int argc, char *argv[])
{
    int rc = 0, i, cap = 5;
    char buf[LTL_BUF_SIZE];
    void *p;
    ring_t *ring;

    ring = ring_create(cap); 
    
    for(i = 0; i < cap + 1; i++) {
        snprintf(buf, sizeof(buf), "%s-%d", "test", i);
        log_debug("push:%s", buf);
        p = Strdup(buf);
        if(ring_push(ring, p) != RC_OK) {
            Free(p);
        }
    } 

     for(i = 0; i < cap + 1; i++) {
        if(ring_pop(ring, &p) == RC_OK) {
            log_debug("pop:%s", (char *)p);
            Free(p);
        }
    } 

    ring_destroy(ring);
    return rc;
}
#endif
