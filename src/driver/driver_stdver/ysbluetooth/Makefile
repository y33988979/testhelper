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
ST40_EXPORTS := libysbluetooth.a

## 对象文件列表
OBJS := alsa_cp.o bluetooth_utils.o  test.o obex_client.o obex_server.o obex_io.o sdptool.o hi_bluetooth.o bluetooth_api.o

## 目标库名
ST40_TARGETS := libysbluetooth.a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

## 创建目标库
# -------------
$(ST40_TARGETS) : $(ST40_IMPORTS) $(OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)

clean:
	@$(ECHO) ---- Cleaning ysbluetooth ----
	-$(RM) *.o
	-$(RM) *.a

# 依赖关系
