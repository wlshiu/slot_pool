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
    void        *pPool_addr;
    int         one_slot_length;
    int         max_slot_num;

    int         is_used; // TODO: use flag to mark

    int         sequence_num;
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

        memset(pHSlot_pool, 0x0, sizeof(slot_pool_t));
        pHSlot_pool->pPool_addr      = pPool_addr;
        pHSlot_pool->one_slot_length = one_slot_length;
        pHSlot_pool->max_slot_num    = max_slot_num;

        // TODO: handle flag

    } while(0);

    return rval;
}

static inline slot_pool_err_t
slot_pool_get_slot(
    slot_pool_t     *pHSlot_pool,
    void            **ppSlot_addr)
{
    slot_pool_err_t     rval = 0;

    return rval;
}

static inline slot_pool_err_t
slot_pool_recover_slot(
    slot_pool_t     *pHSlot_pool,
    void            *pSlot_addr)
{
    slot_pool_err_t     rval = 0;

    return rval;
}



#ifdef __cplusplus
}
#endif

#endif
