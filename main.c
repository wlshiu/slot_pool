#include <stdio.h>
#include <stdlib.h>

#include "slot_pool.h"

#define MAX_NODE_CNT        15

typedef struct foo
{
    int              order;
    unsigned int     data;
} foo_t;

int main()
{
    slot_pool_err_t     err = SLOT_POOL_ERR_OK;
    int                 i;
    foo_t               a[MAX_NODE_CNT] = {{0}};
    slot_pool_t         hSlot_pool = {0};


    for(i = 0; i < MAX_NODE_CNT; ++i)
    {
        a[i].order = i;
        a[i].data  = i;
    }

    slot_pool_init(&hSlot_pool, &a, sizeof(foo_t), MAX_NODE_CNT);

    for(i = 0; i < MAX_NODE_CNT; ++i)
    {
        foo_t       *pNode = 0;

        err = slot_pool_get_slot(&hSlot_pool, (void**)&pNode);
        if( err )
        {
            printf("%d-th (err= x%x)\n", i, err);
            continue;
        }

        printf("order= %d, data= %u\n", pNode->order, pNode->data);
    }

    printf("recover %d-th\n", MAX_NODE_CNT>>1);
    slot_pool_recover_slot(&hSlot_pool, &a[MAX_NODE_CNT>>1]);
    a[MAX_NODE_CNT>>1].data = MAX_NODE_CNT + 1;

    printf("=============\n");
    for(i = 0; i < MAX_NODE_CNT; ++i)
    {
        foo_t       *pNode = 0;

        err = slot_pool_get_slot(&hSlot_pool, (void**)&pNode);
        if( err )
        {
            // printf("%d-th (err= x%x)\n", i, err);
            continue;
        }

        printf("order= %d, data= %u\n", pNode->order, pNode->data);
    }

    return 0;
}
