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
ST40_IMPORTS := libyspanel.a libysnvm.a libysfilter.a libysos.a libysosd.a libysav.a libdrvinit.a libyscas.a libyshdmi.a libysdisk.a libysvfs.a libysip.a libysplayrec.a
ST40_IMPORTS += libystuner.a

ifeq "$(ENABLE_BLUEZ)" "1"
ST40_IMPORTS += libysbluetooth.a
endif

## 输出库名
ST40_EXPORTS := libdriver$(DVD_HWVER_BIND)$(LOCALIZATION_STRING).a

## 对象文件列表
OBJS := drvver.o memchk.o

## 目标库名
ST40_TARGETS := libdriver$(DVD_HWVER_BIND)$(LOCALIZATION_STRING).a

## 缺省变量与规则
include $(DVD_MAKE)/defrules.mak

##包含下层目录
LINK_PATH := driver$(LOCALIZATION_STRING)/yspanel driver$(LOCALIZATION_STRING)/ysnvm driver$(LOCALIZATION_STRING)/ysfilter driver$(LOCALIZATION_STRING)/ysos driver$(LOCALIZATION_STRING)/ysosd\
driver$(LOCALIZATION_STRING)/ysav driver$(LOCALIZATION_STRING)/drvinit driver$(LOCALIZATION_STRING)/yscas driver$(LOCALIZATION_STRING)/yshdmi driver$(LOCALIZATION_STRING)/ysdisk driver$(LOCALIZATION_STRING)/ysvfs driver$(LOCALIZATION_STRING)/ysip driver$(LOCALIZATION_STRING)/ystuner driver$(LOCALIZATION_STRING)/ysplayrec

ifeq "$(ENABLE_BLUEZ)" "1"
LINK_PATH += driver$(LOCALIZATION_STRING)/ysbluetooth
endif

## 创建目标库
# -------------
$(ST40_TARGETS) : $(ST40_IMPORTS) $(OBJS)
	@$(ECHO) Building $@
	$(BUILD_LIBRARY)
	cp $(ST40_TARGETS) $(YSENV_DIR_PROJ)/pub/$(CFG_HI3716_CHIP_TYPE)/lib/$(ST40_TARGETS)

libysav.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysav
libysfilter.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysfilter
libysnvm.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysnvm
libysos.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysos
libysosd.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysosd
libyspanel.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/yspanel
libystuner.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ystuner
libdrvinit.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/drvinit
libyscas.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/yscas
libyshdmi.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/yshdmi
libysdisk.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysdisk
libysvfs.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysvfs
libysip.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysip
libysplayrec.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysplayrec
ifeq "$(ENABLE_BLUEZ)" "1"
libysbluetooth.a :
	$(MAKE) -C driver$(LOCALIZATION_STRING)/ysbluetooth
endif
force:
drvver.o: drvver.c force
	$(COMPILE_C)
	
clean: 
	@echo Cleaning driver
	
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysav clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysfilter clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysnvm clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysos clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysosd clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/yspanel clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysblast clean	
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ystuner clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysplayrec clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/drvinit clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/yscas clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/yshdmi clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysdisk clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysvfs clean
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysip clean
ifeq "$(ENABLE_BLUEZ)" "1"
	-$(MAKE) -kC driver$(LOCALIZATION_STRING)/ysbluetooth clean
endif
	$(RM) *.o
	$(RM) *.a
# 依赖关系
