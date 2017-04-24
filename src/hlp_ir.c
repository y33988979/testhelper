/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 

#include <hlp_ir.h>

#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_unf_ecs.h"



HI_S32 HI_IR_Open(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_IR_Open();
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_IR_SetCodeType(HI_UNF_IR_CODE_NEC_SIMPLE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_UNF_IR_Close();
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_IR_EnableKeyUp(HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_UNF_IR_Close();
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_IR_EnableRepKey(HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_UNF_IR_Close();
        return HI_FAILURE;
    }

    /*Fetch key value from ir model.*/
    s32Ret = HI_UNF_IR_SetFetchMode(0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_UNF_IR_Close();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_IR_Close(HI_VOID)
{
    return  HI_UNF_IR_Close();
}

HI_S32 HI_IR_Enable (HI_VOID)
{
    return HI_UNF_IR_Enable(HI_TRUE);
}

HI_S32 HI_IR_Disable(HI_VOID)
{
    return HI_UNF_IR_Enable(HI_FALSE);
}

HI_S32 HI_IR_GetValue(HI_U64 *u32PressStatus, HI_U64 *u32KeyId)
{
    HI_U64 u64KeyId = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret=HI_UNF_IR_GetValue((HI_UNF_KEY_STATUS_E *) u32PressStatus, &u64KeyId , 0);
    *u32KeyId= (HI_U64)u64KeyId;
    return s32Ret;
}


