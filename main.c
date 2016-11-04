#include <stdio.h>
#include <stdlib.h>

#include "slot_pool.h"

int main()
{
    int             a[15] = {0};
    slot_pool_t     hSlot_pool = {0};

    slot_pool_init(&hSlot_pool, &a, sizeof(int), 15);

    printf("Hello world!\n");
    return 0;
}
