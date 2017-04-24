#*****************************************************************************
#
# Description: 
#    Makefile for 3716C_T
#
# Revision History:
#    Date          Author         Modification
# ---------------------------------------------------------------------
# 1. 2012-11-20    wrwein      创建
#*****************************************************************************

## 公共变量与规则
include $(DVD_MAKE)/generic.mak

## 编译选项
CFLAGS := $(CFLAGS)

## 输入库名
ST40_IMPORTS :=

## 输出库名
ST40_EXPORTS := libatbm.a

## 对象文件列表
OBJS := atbm886x.o

## 目标库名
ST40_TARGETS := libatbm.a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

## 创建目标库
# -------------
$(ST40_TARGETS) : $(ST40_IMPORTS) $(OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)

clean:
	@$(ECHO) ---- Cleaning ystuner ----
	-$(RM) *.o
	-$(RM) *.a

# 依赖关系
