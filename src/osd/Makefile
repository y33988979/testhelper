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
LDFLAGS =  -L../lib -lpthread -lrt -ldl -lm -lhi_common -lhi_mpi -lhi_ecs -lhipes -lhigo -lhigoadp -lfreetype -ltde -lhipng -lz -lhijpeg -lhijpge -lhiflash -lhi_subtitle -lasm

LIBS = ../libdriver0x656_stdver.a

CFLAGS = -g -O2 -pipe -Wall -I. -I.. -I../include -I../ysinclude -I../ysinclude/drv

# Change these if necessary

CC = arm-hisiv200-linux-gcc
CPP = arm-hisiv200-linux-gcc -E

SOURCES = hlp_osd.c hlp_burnin_conf.c

OBJS =  $(SOURCES:.c=.o)

all:	$(OBJS)
	cp *.o ..

hlp_osd.o: hlp_osd.c
	$(CC) -c $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
	@echo "Create hlp_osd.o done!"
hlp_burnin_conf.o: hlp_burnin_conf.c
	$(CC) -c $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
	@echo "Create hlp_burnin_conf.o done!"
	
clean:
	rm -f $(OBJS) ~.*
	
# depend stuff
#include .depend

