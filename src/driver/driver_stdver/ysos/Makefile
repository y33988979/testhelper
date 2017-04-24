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
ST40_EXPORTS := libysos.a

## 对象文件列表
OBJS := drv_os.o ys_os.o drv_sys.o ysostime.o ysoslist.o ysreset.o ysosdecompress.o drv_memory.o drv_avmem.o drv_message.o drv_debug.o

## 目标库名
ST40_TARGETS := libysos.a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

## 创建目标库
# -------------
$(ST40_TARGETS) : $(ST40_IMPORTS) $(OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)

clean:
	@$(ECHO) ---- Cleaning ys_os ----
	-$(RM) *.o
	-$(RM) *.a

# 依赖关系
