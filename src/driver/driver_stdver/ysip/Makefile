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
CFLAGS := $(CFLAGS) -DSTTBX_PRINT

## 输入库名
ST40_IMPORTS :=

## 输出库名
ST40_EXPORTS := libysip.a

## 对象文件列表
# local objects which comprise this component
OBJS+=iw_cfg.o
OBJS+=iwcommon.o
OBJS+=ip_cfg.o
OBJS+=if_cfg.o
OBJS+=ipping.o
OBJS+=dhcp_client.o
OBJS+=dhcp_server.o
OBJS+=arpping.o
OBJS+=dhcp_socket.o
OBJS+=dhcp_packet.o
OBJS+=dhcp_system.o
OBJS+=tftp.o
OBJS+=socket.o
OBJS+=cm_ctrl.o

## 目标库名
ST40_TARGETS := libysip.a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

## 创建目标库
# -------------
$(ST40_TARGETS) : $(ST40_IMPORTS) $(OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)

clean:
	@$(ECHO) ---- Cleaning libysip ----
	-$(RM) *.o
	-$(RM) *.a

# 依赖关系


