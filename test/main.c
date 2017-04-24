#include <stdio.h>
#include <hi_common.h>
#include <hi_unf_ecs.h>


#define TEST_GPIO_GROUP		7
#define TEST_GPIO_NUM		4

#define TEST_GPIO_ADDR      0x164

#define MULIT_CONFIG_BASE  (0x10203000)
static HI_U32 g_au32SampleGpioRegValue=0 ;


void usage()
{
	printf("Usage:   ./test GpioGroup GpioNum \n");
	printf("example: ./test 7 2  -> group=7 num=2\n");
}

int main(int argc, char **argv)
{
	int i;
	int Ret;
	int GpioGroup;
	int GpioNum;
	int GpioNo;
	HI_BOOL WriteBit = HI_TRUE;
	HI_BOOL ReadBit;

	GpioGroup = TEST_GPIO_GROUP;
	GpioNum = TEST_GPIO_NUM;
	GpioNo = GpioGroup*8+GpioNum;
	
	if(argc <=2)
	{
		usage();
		return 0;
	}

	if(argc > 2)
	{
		if(atoi(argv[3]) == 0)
			WriteBit = HI_FALSE;
		else
			WriteBit = HI_TRUE;
	}
	
	GpioGroup = atoi(argv[1]);
	GpioNum = atoi(argv[2]);
	GpioNo = GpioGroup*8+GpioNum;
	
	HI_SYS_Init();

	    HI_SYS_ReadRegister(MULIT_CONFIG_BASE + TEST_GPIO_ADDR, &g_au32SampleGpioRegValue); /*store old pin function define */

    g_au32SampleGpioRegValue &= 0xFFFFFFF8;
	g_au32SampleGpioRegValue |= 0x3;
    HI_SYS_WriteRegister(MULIT_CONFIG_BASE + TEST_GPIO_ADDR, g_au32SampleGpioRegValue);

	Ret = HI_UNF_GPIO_Open();
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return -1;
    }
	

	printf("Gpio SetDir is input\n");
	Ret = HI_UNF_GPIO_SetDirBit(GpioNo, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

	Ret = HI_UNF_GPIO_ReadBit(GpioNo, &ReadBit);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }
	printf("Gpio ReadBit GpioNo=%d: val=%d\n", GpioNo, ReadBit);
	
	printf("Gpio SetDir is output\n");
	Ret = HI_UNF_GPIO_SetDirBit(GpioNo, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    Ret = HI_UNF_GPIO_WriteBit(GpioNo, WriteBit);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }
	printf("Gpio WriteBit GpioNo=%d: val=%d\n", GpioNo, WriteBit);
#if 0
	printf("Gpio SetDir is input\n");
	Ret = HI_UNF_GPIO_SetDirBit(GpioNo, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

	Ret = HI_UNF_GPIO_ReadBit(GpioNo, &ReadBit);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }
	printf("Gpio ReadBit GpioNo=%d: val=%d\n", GpioNo, ReadBit);
#endif
    //getchar();
	
ERR1:
    Ret = HI_UNF_GPIO_Close();
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
    }
	HI_SYS_DeInit();

	return 0;	
}

#if 0
void test_gpio()
{
		Ret = HI_UNF_GPIO_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR0;
    }

    printf("Press any key to set GPIO%d_%d to output mode and output high level\n", u32Group, u32Bit);

    Ret = HI_UNF_GPIO_SetDirBit(u32TestGpioNum, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }
    Ret = HI_UNF_GPIO_WriteBit(u32TestGpioNum, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("Press any key to set GPIO%d_%d to input mode and get the input level\n", u32Group, u32Bit);
    getchar();
    Ret = HI_UNF_GPIO_SetDirBit(u32TestGpioNum, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    Ret = HI_UNF_GPIO_ReadBit(u32TestGpioNum, &ReadBitVal);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("GPIO%d_%d Input: %d\n", u32Group, u32Bit, ReadBitVal);

    printf("Press any key to set GPIO%d_%d to up-interrupt mode.\n", u32Group, u32Bit);
    getchar();
    Ret = HI_UNF_GPIO_SetIntType(u32TestGpioNum, HI_UNF_GPIO_INTTYPE_UP);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    Ret = HI_UNF_GPIO_SetIntEnable(u32TestGpioNum, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("Please wait for %d seconds to query interrupt on GPIO%d_%d\n", u32IntWaitTime / 1000, u32Group, u32Bit);
    Ret = HI_UNF_GPIO_QueryInt(&u32IntValue, u32IntWaitTime);
    if ((HI_SUCCESS == Ret) && (u32IntValue == u32TestGpioNum))
    {
        printf("GPIO%d_%d get an up edge interrupt\n", u32Group, u32Bit);
    }
    else if (HI_ERR_GPIO_GETINT_TIMEOUT == Ret)
    {
        printf("GPIO%d_%d has not got an up edge interrupt in %d seconds!\n", u32Group, u32Bit, u32IntWaitTime / 1000);
    }

    Ret = HI_UNF_GPIO_SetIntEnable(u32TestGpioNum, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto ERR1;
    }

    printf("Press any key to quit normally.");
    getchar();
ERR1:

    Ret = HI_UNF_GPIO_DeInit();
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
    }

ERR0:
    HI_SYS_WriteRegister(g_u32Multi_useRegAddr, g_u32Multi_useValue);

	(HI_VOID)HI_SYS_DeInit();
	
	return 0;	

}
#endif

