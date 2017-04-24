
include $(DVD_MAKE)/cfg.mak

#$(warning ---------- sdk user cfg-------)
#$(warning CFG_SDK_VERSION = $(CFG_SDK_VERSION))
#$(warning CFG_CHIP_TYPE = $(CFG_CHIP_TYPE))
#$(warning CFG_BOARD_TYPE = $(CFG_BOARD_TYPE))
#$(warning CFG_IR_TYPE = $(CFG_IR_TYPE))
#$(warning CFG_EGL_TYPE = $(CFG_EGL_TYPE))
#$(warning CFG_VERSION_TYPE = $(CFG_VERSION_TYPE))
#$(warning --------------------------------)

########################config for cross toolchain ######################
ifeq ($(CFG_TOOLCHAINS_NAME),gcc-3.4.3-vfp)
    export CROSS:=arm-vfp_uclibc-linux-gnu
    USER_CFLAGS = -march=armv5te -mtune=arm926ej-s
endif

ifeq ($(CFG_TOOLCHAINS_NAME),gcc-3.4.3-soft)
    export CROSS:=arm-hismall-linux
    USER_CFLAGS = -march=armv5te -mtune=arm926ej-s
endif

ifeq ($(CFG_TOOLCHAINS_NAME),gcc-4.4.0-vfp)
    export CROSS:=arm-hisiv100-linux
    USER_CFLAGS = -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16
endif

ifeq ($(CFG_TOOLCHAINS_NAME),gcc-4.4.1-vfp_glibc)
    export CROSS:=arm-hisiv200-linux
    USER_CFLAGS = -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16
endif

AR      = $(CROSS)-ar
AS      = $(CROSS)-as
LD      = $(CROSS)-ld
CPP     = $(CROSS)-cpp
CC      = $(CROSS)-gcc
NM      = $(CROSS)-nm
STRIP   = $(CROSS)-strip
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump

#######################common configs: chiptype, boardtype, sdkversion####################
CFLAGS = -DCHIP_TYPE_$(CFG_CHIP_TYPE) -DBOARD_TYPE_$(CFG_BOARD_TYPE) -DSDK_VERSION=$(CFG_SDK_VERSION) -Wall -O0 -g
HI_CFLAGS = -Wall -O2 -g
####################### config for MCE/Kernel: #######################################
### if MCE_SUPPORT is YES, the code in source/msp_base will be compiled to the kernel(zImage) ###
### if MCE_SUPPORT is NO, the code in source/msp_base will be compiled to hi_mpi.ko ###
ifeq ($(CFG_MCE_SUPPORT),YES)
export MCE_SUPPORT:=YES
CFLAGS += -DMCE_SUPPORT
else
export MCE_SUPPORT:=NO
endif

ifeq ($(CFG_GIF_HWDECODE),YES)
CFLAGS += -DDRV_GIF_HWDEC
endif
ifeq ($(CFG_SLZK_SUPPORT),YES)
CFLAGS += -DDRV_SLZK_SUPPORT
endif
############################ exports for msp/tools ######################################## 
ifeq ($(CFG_SVDEC_SUPPORT),YES)
export SVDEC_SUPPORT:=YES
else
export SVDEC_SUPPORT:=NO
endif

export CFG_SDK_VERSION CFG_CHIP_TYPE CFG_BOARD_TYPE CFLAGS USER_CFLAGS CFG_TOOLCHAINS_NAME CFG_CHIP_VERSION
export CROSS AR AS LD CPP CC NM STRIP OBJCOPY OBJDUMP
export CFG_IR_TYPE 
export CFG_EGL_TYPE
################################################################################
# This file describe the customed make rules
include $(DVD_MAKE)/ysstb.mak
