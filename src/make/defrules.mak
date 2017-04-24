## ****************************************************************************
## 
## Make系统的公共设置
## 
## Revision History:
##     Date        Version   Author       Modification
## ---------------------------------------------------------------------
## 01. 2011-06-14  0x010000  jyshan       创建
## 
## ****************************************************************************


## ****************************************************************************
## 工具设置
## ****************************************************************************
CP=cp
MV=mv
RM=rm
ECHO=echo


## *****************************************************************************
## 缺省的编译方法
## *****************************************************************************

## 抽取首尾空格
LIBRARY_PATH = $(strip $(LINK_PATH) $(YSMAK_DIR_LIB_DEV) $(YSMAK_DIR_LIB_USR) $(SUBDIR_LIBRARY_PATH))

## 在每个单词前添加前缀
LIBRARIES += $(addprefix $(LIBRARY_PREFIX),$(LIBRARY_PATH))


define LINK_EXECUTABLE
$(LINK) $(LIBRARIES) \
      $(DEFAULT_LKFLAGS) $($(basename $@)_LKFLAGS) $($(basename $@)_$(ARCHITECTURE)_LKFLAGS) \
      $(filter %.o,$^) \
      $(foreach LIB,$(filter lib%.a,$(notdir $^)),$(if $(LINUX_$(LIB)_NOLINK),,$(addprefix -l,$(patsubst lib%.a,%,$(LIB))))) \
          $(LDOPTS) -o $@
endef


define COMPILE_C
$(CC) $(CFLAGS) $(DEFAULT_CFLAGS) $($(basename $@)_CFLAGS) $($(basename $@)_$(ARCHITECTURE)_CFLAGS) $(DRV_CFLAGS) \
      $(INCLUDES) $(BASE_INCLUDES) $(OS_INCLUDES) -o $@ -c $<
endef

# The build library function is complex because the librarian cannot include
# a library in a library. We have to extract the libraries into a temporary
# directory and then insert those files into the library.

define FIRST_LIB_IN_PATH
$(if $(wildcard $(addsuffix /$(IMP),$(LIBRARY_PATH))),\
     $(firstword $(wildcard $(addsuffix /$(IMP),$(LIBRARY_PATH)))),\
     $(error Failed to find library $(IMP) on path ($(LIBRARY_PATH))))
endef

define BUILD_LIBRARY
$(if $(wildcard $@),@$(RM) $@)
$(if $(wildcard ar.mac),@$(RM) ar.mac)
$(if $(filter lib%.a,$(notdir $^)),
@echo CREATE $@ > ar.mac
$(foreach IMP,$^,
  $(if $(filter lib%.a,$(notdir $(IMP))),
   $(if $(LINUX_$(IMP)_NOLIB),,
    $(if $(findstring ./,$(dir $(IMP))),
      @echo ADDLIB $(FIRST_LIB_IN_PATH) >> ar.mac
     ,@echo ADDLIB $(IMP) >> ar.mac
     )
    )
   )
)
@echo SAVE >> ar.mac
@echo END >> ar.mac
@$(AR) -M <ar.mac
@$(RM) ar.mac
)
$(if $(filter %.o,$^),@$(AR) -q $@ $(filter %.o,$^))
$(AR) -s $@
endef


# Default rule to build a .o object file
%.o: %.c
	@$(ECHO) Compiling $<
	$(COMPILE_C)

# Default rule to build a library file from a single .o file
%.a: %.o
	@$(ECHO) Building library $@
	$(BUILD_LIBRARY)
