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
ifdef ATBM886X
ST40_IMPORTS := libatbm.a
LINK_SUB_PATH := atbm
else
ST40_IMPORTS := libhisituner.a
LINK_SUB_PATH := hisituner
endif

## 输出库名
ST40_EXPORTS := libystuner.a


## 目标库名
ST40_TARGETS := libystuner.a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

## 创建目标库
# -------------
$(ST40_IMPORTS) :
	$(MAKE) -C $(LINK_SUB_PATH)
	cp $(LINK_SUB_PATH)/$(ST40_IMPORTS) ./
	mv $(ST40_IMPORTS) $(ST40_TARGETS)


clean:
	@$(ECHO) ---- Cleaning ystuner ----
	-$(MAKE) -kC $(LINK_SUB_PATH) clean
	-$(RM) *.o
	-$(RM) *.a

# 依赖关系
#libatbm.a :
#	$(MAKE) -C atbm
#clean:
#	@$(ECHO) ---- Cleaning ystuner ----
#	-$(MAKE) -kC atbm clean
#	-$(RM) *.o
#	-$(RM) *.a
