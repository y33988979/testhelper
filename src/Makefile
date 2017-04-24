#
# Copyright (C) ychen
# Copyright (C) Jiuzhou, Inc.
#

.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean helper depend all dist


GCC_FLAGS = -Wstrict-prototypes -Wpointer-arith -Wcast-align -Wcast-qual\
  -Wtraditional\
  -Wshadow\
  -Wconversion\
  -Waggregate-return\
  -Wmissing-prototypes\
  -Wnested-externs\
  -Wall \
  -Wundef -Wwrite-strings -Wredundant-decls -Winline


srcdir = .
LDFLAGS =  -Llib -lpthread -lrt -ldl -lm -lhi_common -lhi_mpi -lhi_ecs -lhipes -lhigo -lhigoadp -lfreetype -ltde -lhipng -lz -lhijpeg -lhijpge -lhiflash -lhi_subtitle -lasm

LIBS = libdriver0x656_stdver.a

CFLAGS = -g -O2 -pipe -Wall -I. -Iinclude -Iysinclude -Iysinclude/drv

# Change these if necessary

CC = arm-hisiv200-linux-gcc
CPP = arm-hisiv200-linux-gcc -E

SOURCES = hlp_file.c hlp_net.c hlp_ring.c hlp_uart.c hlp_uart_send.c hlp_uart_recv.c hlp_ipc.c\
 hlp_gpio.c hlp_panel.c main.c hlp_file_manager.c hlp_burnin.c hlp_log.c hlp_string.c \
 hlp_communicate.c hlp_ir.c hlp_switch_onoff.c hlp_common.c

OBJS =  $(SOURCES:.c=.o)
OBJS += osd/hlp_osd.o osd/hlp_burnin_conf.o

all:	switch helper

switch:
	$(CC) $(CFLAGS) hlp_serial.c hlp_gpio.c -o hlp_switch -lpthread -Llib -lhi_common -lhi_ecs -lm
	cp hlp_switch /home/work
	cp hlp_switch app
	mv app/hlp_switch app/helper
helper:	$(OBJS)
	make -C osd
	touch main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
	cp helper /home/work
	#cp helper app
	@echo "Create helper done!"
	./mkyaffs2image504 app helper.yaffs2 2k 1bit
	cp helper.yaffs2 ../../image

clean:
	rm -f $(OBJS) helper ~.*
	rm ../../image/helper.yaffs2
	
# depend stuff
#include .depend

