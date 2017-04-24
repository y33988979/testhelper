###########################################################################
# Hisilicon SDK config file, see "cfg.mak_notes.txt" for more information #
###########################################################################

##############sdk version config #############
CFG_SDK_VERSION="Hi3716XV100R001C00SPC0A1"

##############chip type config #############
#CFG_CHIP_TYPE=hi3716mv300_fpga
#CFG_CHIP_TYPE=hi3720
#CFG_CHIP_TYPE=hi3716h
#CFG_CHIP_TYPE=hi3716c
#CFG_CHIP_TYPE=hi3716m

ifeq ($(HardwareVersion),0x650)
CFG_HW_VER=0x650
CFG_HI3716_CHIP_TYPE=hi3716m
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mstb
CFG_BOOT_REG=hi3716mstbverb_hi3716mv200_ddr2_256mbyte_16bitx2_2layers.reg
CFG_MUTECTL_GPIO	= 0x65
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x651)
CFG_HW_VER=0x651
CFG_HI3716_CHIP_TYPE=hi3716m
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mstb
CFG_BOOT_REG=hi3716mrefvera_hi3716mv200_ddr3_256mbyte_16bitx2_2layers.reg
CFG_MUTECTL_GPIO	= 0x65
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x652)
CFG_HW_VER=0x652
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x653)
CFG_HW_VER=0x653
CFG_HI3716_CHIP_TYPE=hi3716m
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mstb
CFG_BOOT_REG=hi3716mrefvera_hi3716mv200_ddr3_512mbyte_16bitx2_2layers.reg
CFG_MUTECTL_GPIO	= 0x65
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x654)
CFG_HW_VER=0x654
CFG_HI3716_CHIP_TYPE=hi3716m
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mstb
CFG_BOOT_REG=hi3716mrefvera_hi3716mv200_ddr3_512mbyte_16bitx2_2layers.reg
CFG_MUTECTL_GPIO	= 0x65
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x655)
CFG_HW_VER=0x655
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x656)
CFG_HW_VER=0x656
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x657)
CFG_HW_VER=0x657
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif
 
ifeq ($(HardwareVersion),0x658)
CFG_HW_VER=0x658
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x659)
CFG_HW_VER=0x659
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x661)
CFG_HW_VER=0x661
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_ADVCA_TYPE=NOVEL
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x662)
CFG_HW_VER=0x662
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_ADVCA_TYPE=SUMA
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x663)
CFG_HW_VER=0x663
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_ADVCA_TYPE=NOVEL
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x664)
CFG_HW_VER=0x664
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_ADVCA_TYPE=SUMA
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif
 
ifeq ($(HardwareVersion),0x665)
CFG_HW_VER=0x665
CFG_HI3716_CHIP_TYPE=hi3716mv300
CFG_CHIP_TYPE=hi3716m
CFG_BOARD_TYPE=hi3716mdmo3avera
CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
CFG_MUTECTL_GPIO	= 0x43
CFG_MUTECTL_LEVEL	= 1
CFG_KEYLED_TYPE := CT1642
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x680)
CFG_HW_VER=0x680
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_sd_8bitx4_4layers.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x681)
CFG_HW_VER=0x681
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x682)
CFG_HW_VER=0x682
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x683)
CFG_HW_VER=0x683
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x684)
CFG_HW_VER=0x684
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_network_8bitx4_4layers.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x685)
CFG_HW_VER=0x685
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x687)
CFG_HW_VER=0x687
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
##MLC flash使用，需要修改boot中godbox.h文件中的define CONFIG_HINFC301_HARDWARE_PAGESIZE_ECC
##改为define CONFIG_HINFC301_AUTO_PAGESIZE_ECC，这是因为硬件设计导致boot中硬件检测PAGESIZE_ECC
##会出错，需要在boot中软件检测PAGESIZE_ECC
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_network_8bitx4_4layers.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x100000
CFG_NAND_PAGE_SIZE=4k
CFG_NAND_ECC_TYPE=24bit
endif

ifeq ($(HardwareVersion),0x688)
CFG_HW_VER=0x688
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_sd_8bitx4_4layers.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x689)
CFG_HW_VER=0x689
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x690)
CFG_HW_VER=0x690
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x691)
CFG_HW_VER=0x691
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x693)
CFG_HW_VER=0x693
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716crefvera_hi3716cv100_ddr3_1gbyte_16bitx2_4layers_UsbSD_eMMC.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x697)
CFG_HW_VER=0x697
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716crefvera_hi3716cv100_ddr3_1gbyte_16bitx2_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x698)
CFG_HW_VER=0x698
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716crefvera_hi3716cv100_ddr3_1gbyte_16bitx2_4layers_UsbSD.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

ifeq ($(HardwareVersion),0x699)
CFG_HW_VER=0x699
CFG_HI3716_CHIP_TYPE=hi3716c
CFG_CHIP_TYPE=hi3716c
CFG_BOARD_TYPE=hi3716cdmoverb
CFG_BOOT_REG=hi3716crefvera_hi3716cv100_ddr3_1gbyte_16bitx2_4layers_UsbSD_eMMC.reg
CFG_MUTECTL_GPIO	= 0x39
CFG_MUTECTL_LEVEL	= 0
CFG_KEYLED_TYPE := STANDARD_KEYLED
CFG_NAND_BLOCK_SIZE=0x20000
CFG_NAND_PAGE_SIZE=2k
CFG_NAND_ECC_TYPE=1bit
endif

##############board type config #############
#CFG_BOARD_TYPE=hi3716mv300_fpga
#CFG_BOARD_TYPE=hi3716cref
#CFG_BOARD_TYPE=hi3716crefs2
#CFG_BOARD_TYPE=hi3716mstb
#CFG_BOARD_TYPE=hi3716mstbverc
#CFG_BOARD_TYPE=hi3716cdmoverb
#CFG_BOARD_TYPE=hi3716hdmoverb
#CFG_BOARD_TYPE=hi3716href
#CFG_BOARD_TYPE=hi3716mref
#CFG_BOARD_TYPE=hi3716mdmo3bvera
#CFG_BOARD_TYPE=hi3716mdmo3dvera
#CFG_BOARD_TYPE=hi3716mdmo3avera

############## ddr relate config ############
#CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_1gbyte_8bitx4_4layers.reg
#CFG_BOOT_REG=hi3716cdmoverb_hi3716cv100_ddr3_512mbyte_8bitx4_4layers.reg
#CFG_BOOT_REG=hi3716crefvera_hi3716cv100_ddr3_1gbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716crefvera_hi3716cv100_ddr3_512mbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716crefs2vera_hi3716cv100_ddr3_512mbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716hdmoverb_hi3716hv100_ddr3_1gbyte_8bitx4_4layers.reg
#CFG_BOOT_REG=hi3716hdmoverb_hi3716hv100_ddr3_512mbyte_8bitx4_4layers.reg
#CFG_BOOT_REG=hi3716hrefvera_hi3716hv100_ddr3_1gbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716hrefvera_hi3716hv100_ddr3_512mbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716mrefvera_hi3716mv200_ddr3_256mbyte_16bitx2_2layers.reg
#CFG_BOOT_REG=hi3716mrefvera_hi3716mv200_ddr3_512mbyte_16bitx2_2layers.reg
#CFG_BOOT_REG=hi3716mstbverb_hi3716mv200_ddr2_256mbyte_16bitx2_2layers.reg
#CFG_BOOT_REG=hi3716mstbverc_hi3716mv200_ddr2_256mbyte_16bitx2_2layers.reg
#CFG_BOOT_REG=hi3716mv200_ddr3_256mbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716mv200_ddr3_512mbyte_16bitx2_4layers.reg
#CFG_BOOT_REG=hi3716mdmo3bvera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
#CFG_BOOT_REG=hi3716mdmo3bvera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
#CFG_BOOT_REG=hi3716mdmo3dvera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
#CFG_BOOT_REG=hi3716mdmo3dvera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
#CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_256mbyte_16bit_2layers.reg
#CFG_BOOT_REG=hi3716mdmo3avera_hi3716mv300_ddr3_512mbyte_16bit_2layers.reg
#CFG_BOOT_REG=hi3716mott3avera_hi3716mv300_ddr3_512mbyte_16bit_4layers.reg
############################################
CFG_H3716MV200_DDR3_LAYER2_RESET=y

############## mute gpio config #############
# MUTECTL_GPIO Support notes:
#===============================================================================
# board_type             		level	gpio_pin (gpio_pin = gpio_group * 8 + gpio_num)
# BOARD_TYPE_hi3716cdmoverb		0		0x39(GPIO7_1)
# BOARD_TYPE_hi3716hdmoverb		0		0x39(GPIO7_1)
# BOARD_TYPE_hi3716mstb			0		0x65(GPIO12_5)
# BOARD_TYPE_hi3716mstbverc		0		0x5b(GPIO11_3)
# BOARD_TYPE_hi3716mtst			0		0x5b(GPIO11_3)
# BOARD_TYPE_hi3716mdmo3avera	1		0x43(GPIO8_3)
# BOARD_TYPE_hi3716mtst3avera	1		0x43(GPIO8_3)

#CFG_MUTECTL_GPIO	= 0x43
#CFG_MUTECTL_LEVEL	= 1

################################# wifi config ##################################
#  WIFI Support notes:
#===============================================================================
#  device             sta     ap
#  pcie_ar9285        YES     YES
#  pcie_rt3090:       YES     NOT*
#  pcie_rtl8192ce:    YES     NOT*
#  pcie_rtl8192se:    YES     NOT*
#  usb_ar9271:        YES     YES
#  usb_rt3070:        YES     YES
#  usb_rt5370:        YES     YES
#  usb_rtl8188cu:     YES     YES
#
#  YES    Current wifi project support this function
#  NOT    Current wifi project NOT support this function.
#  *      WIFI Provider NOT support or NOT recommend
################################################################################
#  WIFI_DEVICE = none            // Don't build wifi
#  WIFI_DEVICE = usb_ar9271

WIFI_DEVICE   = none


# select wifi mode  
# WIFI_MODE = sta
# WIFI_MODE = ap
#
WIFI_MODE   = sta

##############ir config #############
#CFG_IR_TYPE=IR_LIRC
CFG_IR_TYPE=IR_S2

CFG_ANDROID_TOOLCHAIN=NO
#############egl config##################
#CFG_EGL_TYPE=higo
CFG_EGL_TYPE=fb
#CFG_EGL_TYPE=directfb

##############  gif hw decode config #############
#CFG_GIF_HWDECODE=YES
CFG_GIF_HWDECODE=NO
##############  hisi siliangziku support config #############
#CFG_SLZK_SUPPORT=YES  ##所有调用到下列函数的任务的堆栈都要增加140*1024 bit左右,否则在调用下列函数的地方会出现死机，
CFG_SLZK_SUPPORT=NO    ##由于下列函数都要消耗140*1024 bit的堆栈；矢量字库的画图效率低于普通的点阵字库，不建议使用，下列函数:DRV_OSDCharacterDraw、DRV_OSDTextLineDraw、DRV_BitmapDrawChar、DRV_BitmapTextLineDraw.
##############  mce  config #############
#CFG_MCE_SUPPORT=YES
CFG_MCE_SUPPORT=NO

##############  mmz  config #############
#CFG_MMZ_V2_SUPPORT=YES
CFG_MMZ_V2_SUPPORT=NO

##############  advca config ############

#CFG_ADVCA_TYPE=CONAX
#CFG_ADVCA_TYPE=NOVEL
#CFG_ADVCA_TYPE=NAGRA
#CFG_ADVCA_TYPE=SUMA
#CFG_ADVCA_TYPE=CTI
#CFG_ADVCA_TYPE=OTHER

ifdef CFG_ADVCA_TYPE
#
# Macro CFG_ADVCA_FUNCTION is to config the functions of kernel,rootfs,uboot,
# loader for ADVCA, the value should be RELEASE/DEBUG
# Not define CFG_ADVCA_FUNCTION to get full functions.
#

#CFG_ADVCA_FUNCTION=DEBUG
CFG_ADVCA_FUNCTION=RELEASE

endif # CFG_ADVCA_TYPE


ifeq ($(CFG_ADVCA_TYPE),NAGRA)
ifeq ($(CFG_CHIP_TYPE),hi3716m)

#
# CFG_CHIP_VERSION=Hi3716Mv300
#
# Only for Nagra bootcode, should not be used for other module.
# We used the MACRO to choose which NAND/SPI flash driver source code could be
# compiled. It will make U-Boot binary image is smaller.
# Hi3716Mv300 and Hi3716(C/H/Mv100/Mv200) use different NAND/SPI flash driver.
#
# Hi3716(C/H/Mv100/Mv200) - not define the MARCO, Hi3716MV100/V200 does not support Nagra CA.
# Hi3716Mv300             - define the MARCO 
# 

CFG_CHIP_VERSION=Hi3716Mv300
endif
endif

################################# keyled type config ##################################
#CFG_KEYLED_TYPE := STANDARD_KEYLED
#CFG_KEYLED_TYPE := CT1642
#CFG_KEYLED_TYPE := PT6961
#CFG_KEYLED_TYPE := PT6964


###################CFG_KEYLED_CT1642_KERNEL_SUPPORT#####################
#Only valid when CFG_KEYLED_TYPE = CT1642
#Yes: CT1642 LED can display when starting kernel
#NO : CT1642 LED can't display when starting kernel
ifeq ($(CFG_KEYLED_TYPE),CT1642)
CFG_KEYLED_CT1642_KERNEL_SUPPORT=YES
#CFG_KEYLED_CT1642_KERNEL_SUPPORT=NO
else
# don't change this item
CFG_KEYLED_CT1642_KERNEL_SUPPORT=NO
endif
#zqyuan mosify start
ifeq ($(CFG_BUILD_LOADER),TRUE)
CFG_KEYLED_CT1642_KERNEL_SUPPORT=NO
endif
#zqyuan mosify end
########################################################################################

##############  vo aspect ratio convert config ############
CFG_PLAYER_ASPECT=NO
#CFG_PLAYER_ASPECT=YES

##############  BLUETOOTH config #############
# Note : Open "CFG_BLUETOOTH_SUPPORT=YES" define to support ALSA or Bluetooth function.
ifndef CFG_ADVCA_TYPE
CFG_BLUETOOTH_SUPPORT=NO
#CFG_BLUETOOTH_SUPPORT=YES
endif

############## (H263/VP6) config #############
CFG_SVDEC_SUPPORT=YES
#CFG_SVDEC_SUPPORT=NO

############## outside qam config #############
CFG_DEMOD_DEV_TYPE_PHILIP	:= YES
CFG_DEMOD_DEV_TYPE_ST		:= YES
CFG_DEMOD_DEV_TYPE_GX		:= YES
CFG_DEMOD_DEV_TYPE_AVL6211	:= YES 
############## tuner type config #############
CFG_TUNER_TMX70600			:= YES
CFG_TUNER_XG_3BL			:= YES
CFG_TUNER_CD1616			:= YES
CFG_TUNER_ALPS_TDAE			:= YES
CFG_TUNER_TDCC				:= YES
CFG_TUNER_TDA18250			:= YES
CFG_TUNER_TDA18251			:= YES
CFG_TUNER_CD1616_LF_GIH_4	        := YES
CFG_TUNER_MXL203			:= YES
CFG_TUNER_TMX7070X			:= YES
CFG_TUNER_CU1216			:= YES
CFG_TUNER_TMX7046			:= YES
CFG_TUNER_ALPS				:= YES
CFG_TUNER_AV2011			:= YES
CFG_TUNER_SHARP7903			:= YES

############## DiSEqC support config #############
CFG_DISEQC_SUPPORT		:= YES

############## Demod config #############
#Note: one demod is YES, others must be NO
CFG_ITE9170_SUPPORT		:= NO
CFG_ITE9133_SUPPORT		:= YES

############## CI/CI+ device config #############
CFG_CIMAX_SUPPORT		:= YES
CFG_CIMAXPLUS_SUPPORT		:= NO

################# Loader config  ##################

#************ mini kernel config file **********
MINI_KERNEL_CFG_NAME=hi3716x_mini_net_usb_defconfig
#MINI_KERNEL_CFG_NAME=hi3716x_mini_usb_defconfig
#MINI_KERNEL_CFG_NAME=hi3716x_mini_net_defconfig
#MINI_KERNEL_CFG_NAME=hi3716x_mini_lowest_defconfig
#MINI_KERNEL_CFG_NAME=hi3716x_mini_net_usb_emmc_defconfig

#************ mini kernel config file **********
MINI_FS_CFG_NAME=lowest_config
#MINI_FS_CFG_NAME=lowest_emmc_config

#************ config environment variables size **********
CFG_ENV_SIZE :=0x20000

########################## root file system config  ############################
# cross compile tools select
#
# CFG_TOOLCHAINS_NAME=gcc-3.4.3-soft
# CFG_TOOLCHAINS_NAME=gcc-3.4.3-vfp
# CFG_TOOLCHAINS_NAME=gcc-4.4.1-vfp_glibc
#
CFG_TOOLCHAINS_NAME=gcc-4.4.1-vfp_glibc

# whether strip the root filesystem
# If you select strip option, you will get fail when use gdb, because
# the clib be striped.
#
# CFG_IS_STRIP=yes - strip /bin /lib /sbin /usr
# CFG_IS_STRIP=not - not strip
#
CFG_IS_STRIP=yes

#########################  config eth phy up/down port phyaddr  ################
CFG_UP_ETH_PHY_ADDR=1
CFG_DOWN_ETH_PHY_ADDR=3
################################################################################

##################################  CONFIG_SDKVERSION  #######################
CONFIG_SDKVERSION="1.0.0.2 - $(CFG_SDK_VERSION)"
################################################################################


#################################    nand chip   ###########################################
#CFG_BOARD_TYPE     chip_name    CFG_NAND_BLOCK_SIZE  CFG_NAND_PAGE_SIZE  CFG_NAND_ECC_TYPE
# hi3716cskt                          0x20000                 2k                1bit
# hi3716ctst                          0x20000                 2k                1bit
# hi3716mtst                          0x20000                 2k                1bit
# hi3716mstbverc                      0x20000                 2k                1bit
# hi3716cdmoverb                     0x100000                 8k                24bit
# hi3716hdmoverb                     0x100000                 8k                24bit
# hi3716mstb                          0x20000                 2k                1bit
# hi3716mdmo3bvera                    0x20000                 2k                1bit
# hi3716mstb        HY27UF084G2B      0x20000                 2k                1bit
# hi3716mdmo3avera  HY27UF084G2B      0x20000                 2k                1bit
############################################################################################

###################################  spi chip  ##########################
# CFG_BOARD_TYPE    chip_name   CFG_SPIFLASH_BLOCK_SIZE
# hi3716mstb        FL128PIFL        0X40000
# hi3716mdmo3avera  25L16DFE         0X10000
#########################################################################

# Nand flash block size if you have nand
#CFG_NAND_BLOCK_SIZE=0x20000

# nand flash chip pages size, such as: 2k,4k,8k
#CFG_NAND_PAGE_SIZE=2k

# nand flash chip ecc type, such as: 1bit,4bit,8bit,24bit,40bit
#CFG_NAND_ECC_TYPE=1bit

# Spi flash block size if you have spi flash
CFG_SPIFLASH_BLOCK_SIZE=0x10000

# eMMC rootfs size, unit is M
CFG_EMMC_ROOTFS_SIZE=46080-45M

# eMMC elf size, unit is M
CFG_EMMC_ELF_SIZE=51200-50M

# eMMC flashdata ext3 size, unit is M
# 为满足升级文件长度校验需要，ext3文件长度需要小于flashdata分区1M
CFG_EMMC_FLASHDATA_SIZE=39936-39M

# eMMC wifi ext3 size, unit is M
# 为满足升级文件长度校验需要，ext3文件长度需要小于wifi分区1M
CFG_EMMC_WIFI_SIZE=29696-29M


################# toolchain for rootfs, do NOT change ##################
ifeq ($(CFG_TOOLCHAINS_NAME),gcc-4.4.1-vfp_glibc)
CROSS_TYPE_FS=hfp_glibc
CROSS_VERSION=gcc4.4.x
TOOLCHAIN_DIR_NAME_FS=arm-hisiv200-linux/runtime_lib/armv7a_vfp_v3d16
LIBC_FILE_NAME=lib.glibc.tgz
LIBCPP_FILE_NAME=lib.stdc++.tgz
FS_DIR=1.16.1
BUSYBOX_EDITION=1.16.1
endif

ifeq ($(CFG_TOOLCHAINS_NAME),gcc-3.4.3-vfp)
CROSS_TYPE_FS=hfp
CROSS_VERSION=gcc3.4.x
TOOLCHAIN_DIR_NAME_FS=arm-vfp_uclibc-linux-gnu
LIBC_FILE_NAME=lib.uClibc.tgz
LIBCPP_FILE_NAME=lib.stdc++.tgz
FS_DIR=1.1.2
BUSYBOX_EDITION=1.1.2
endif

ifeq ($(CFG_TOOLCHAINS_NAME),gcc-3.4.3-soft)
CROSS_TYPE_FS=sfp
CROSS_VERSION=gcc3.4.x
TOOLCHAIN_DIR_NAME_FS=arm-hismall-linux
LIBC_FILE_NAME=lib.uClibc.tgz
LIBCPP_FILE_NAME=lib.stdc++.tgz
FS_DIR=1.1.2
BUSYBOX_EDITION=1.1.2
endif




