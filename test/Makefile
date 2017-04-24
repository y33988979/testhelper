
CC=arm-hisiv200-linux-gcc

LIBS=lib/libdriver0x656_stdver.a lib/libhi_common.a

target:
	$(CC) -I include main.c -Llib -lpthread -lrt -ldl -lm -lhi_common -lhi_mpi -lhi_ecs -lhipes -lhigo -lhigoadp -lfreetype -ltde -lhipng -lz -lhijpeg -lhijpge -lhiflash -lhi_subtitle -lasm  -o test 
	@cp test /home/work 
