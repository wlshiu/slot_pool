/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file slot_pool.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/11/04
 * @license
 * @description
 */

#ifndef __slot_pool_H_wuOQkmcn_lHhY_HrlQ_sdac_uILEt8PoVfhS__
#define __slot_pool_H_wuOQkmcn_lHhY_HrlQ_sdac_uILEt8PoVfhS__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This lib, slot pool, is not thread safe. You should maintain mutex by self.
 */

#include <stdlib.h>
#include <string.h>
//=============================================================================
//                  Constant Definition
//=============================================================================
#define MAX_SLOT_NUM            64

typedef enum slot_pool_err
{
    SLOT_POOL_ERR_OK        = 0,
    SLOT_POOL_ERR_INVALID_PARAM,
    SLOT_POOL_ERR_NO_EMPTY,
} slot_pool_err_t;
//=============================================================================
//                  Macro Definition
//=============================================================================
#if 1//defined(SLOT_POOL_DEBUG)
    #include <stdio.h>
    #define sp_dbg(str, args...)        fprintf(stderr, "%s[%d] " str, __func__, __LINE__, ## args)
#else
    #define sp_dbg(str, args...)
#endif

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct slot_pool
{
    unsigned char   *pPool_addr_start;
    unsigned char   *pPool_addr_end;
    int             one_slot_length;
    int             max_slot_num;

    unsigned int    flags[((MAX_SLOT_NUM)+0x1F)>>5];

    int             sequence_num;
} slot_pool_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
static inline slot_pool_err_t
slot_pool_init(
    slot_pool_t     *pHSlot_pool,
    void            *pPool_addr,
    int             one_slot_length,
    int             max_slot_num)
{
    slot_pool_err_t     rval = 0;

    do {
        if( !pHSlot_pool || !pPool_addr ||
            !one_slot_length || !max_slot_num )
        {
            rval = SLOT_POOL_ERR_INVALID_PARAM;
            sp_dbg("wrong parament: x%p, x%p, %d, %d\n",
                   pHSlot_pool, pPool_addr, one_slot_length, max_slot_num);
            break;
        }

        if( max_slot_num > MAX_SLOT_NUM )
        {
            rval = SLOT_POOL_ERR_INVALID_PARAM;
            sp_dbg("out support range %d/%d\n", max_slot_num, MAX_SLOT_NUM);
            break;
        }

        memset(pHSlot_pool, 0x0, sizeof(slot_pool_t));
        pHSlot_pool->pPool_addr_start = (unsigned char*)pPool_addr;
        pHSlot_pool->one_slot_length  = one_slot_length;
        pHSlot_pool->max_slot_num     = max_slot_num;
        pHSlot_pool->pPool_addr_end   = pHSlot_pool->pPool_addr_start
                                        + (pHSlot_pool->one_slot_length * pHSlot_pool->max_slot_num);

    } while(0);

    return rval;
}

static inline slot_pool_err_t
slot_pool_get_slot(
    slot_pool_t     *pHSlot_pool,
    void            **ppSlot_addr)
{
    slot_pool_err_t     rval = 0;

    do {
        int             i = -1, counter = 0;
        int             max_slot_num = 0, sequence_num = 0;
        unsigned int    *pFlags = 0, cacsh_flags = 0;

        if( !pHSlot_pool || !ppSlot_addr )
        {
            rval = SLOT_POOL_ERR_INVALID_PARAM;
            sp_dbg("null point: x%p, x%p\n", pHSlot_pool, ppSlot_addr);
            break;
        }

        max_slot_num = pHSlot_pool->max_slot_num;
        sequence_num = pHSlot_pool->sequence_num;
        pFlags       = pHSlot_pool->flags;

        while( counter++ < max_slot_num )
        {
            if( i != (sequence_num >> 5) )
            {
                i = (sequence_num >> 5);

                cacsh_flags = *(pFlags + i);
            }

            if( !(cacsh_flags & (0x1u << (sequence_num & 0x1F))) )
            {
                *(pFlags + i) |= (0x1u << (sequence_num & 0x1F));
                break;
            }

            sequence_num++;
            sequence_num = (sequence_num == max_slot_num) ? 0 : sequence_num;
        }

        if( counter < max_slot_num )
        {
            *ppSlot_addr = pHSlot_pool->pPool_addr_start + (sequence_num * pHSlot_pool->one_slot_length);
            pHSlot_pool->sequence_num = sequence_num + 1;
            if( pHSlot_pool->sequence_num == max_slot_num )
                pHSlot_pool->sequence_num = 0;
        }
        else
            rval = SLOT_POOL_ERR_NO_EMPTY;

    } while(0);

    return rval;
}

static inline slot_pool_err_t
slot_pool_recover_slot(
    slot_pool_t     *pHSlot_pool,
    void            *pSlot_addr)
{
    slot_pool_err_t     rval = 0;

    do {
        int             i = 0;

        if( !pHSlot_pool || !pSlot_addr )
        {
            rval = SLOT_POOL_ERR_INVALID_PARAM;
            sp_dbg("null point: x%p, x%p\n", pHSlot_pool, pSlot_addr);
            break;
        }

        if( (unsigned char*)pSlot_addr < pHSlot_pool->pPool_addr_start ||
            (unsigned char*)pSlot_addr >= pHSlot_pool->pPool_addr_end )
        {
            rval = SLOT_POOL_ERR_INVALID_PARAM;
            sp_dbg("slot addr (x%p) is not in the range !\n", pSlot_addr);
            break;
        }

        i = ((unsigned char*)pSlot_addr - pHSlot_pool->pPool_addr_start) / pHSlot_pool->one_slot_length;

        pHSlot_pool->flags[i >> 5] &= ~(0x1u << (i & 0x1F));

    } while(0);

    return rval;
}



#ifdef __cplusplus
}
#endif

#endif
