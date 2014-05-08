#ifndef _DS_RING_H_
#define _DS_RING_H_

#ifndef FOR_UNIT_TEST
    #include "prod_inc.h"
#else
    #include "dev_inc.h"
#endif

typedef struct ring ring_t;

ring_t *ring_create(int size);

rc_t ring_push(ring_t *thiz, void *data);

rc_t ring_pop(ring_t *thiz, void **data);

void ring_destroy(ring_t *thiz);

#endif
