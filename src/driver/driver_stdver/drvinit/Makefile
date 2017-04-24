#*****************************************************************************
#
# Description: 
#    Makefile for 5197
#
# Revision History:
#    Date          Author         Modification
# ---------------------------------------------------------------------
# 1. 2005-11-30    Qiujordan      创建
# 2. 2009-04-13    Jyshan         for 5197
#*****************************************************************************

## 公共变量与规则
include $(DVD_MAKE)/generic.mak

## 编译选项
CFLAGS := $(CFLAGS)

## 输入库名
ST40_IMPORTS :=

## 输出库名
ST40_EXPORTS := libdrvinit.a

## 对象文件列表
OBJS := drv_basic.o drv_snd.o drv_disp.o drv_avplay.o drv_vo.o drv_uart.o drv_key_mouse.o  ##drv_mouse.o

## 目标库名
ST40_TARGETS := libdrvinit.a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

## 创建目标库
# -------------
$(ST40_TARGETS) : $(ST40_IMPORTS) $(OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)

clean:
	@$(ECHO) ---- Cleaning drvinit ----
	-$(RM) *.o
	-$(RM) *.a

# 依赖关系
