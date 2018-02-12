CROSS_COMPILE = arm-openwrt-linux-
CC = gcc
LDFLAGS = -L/home/brianlian/openwrt-kernel-4.4/staging_dir/target-arm_cortex-a9+vfpv3_musl-1.1.16_eabi/usr/lib -lblobmsg_json -ljson-c -lubox -lubus -pthread
CFLAGS = -g -I/home/brianlian/openwrt-kernel-4.4/staging_dir/target-arm_cortex-a9+vfpv3_musl-1.1.16_eabi/usr/include

SOURCE = client1.c client2.c client.c
PROJECT_CLIENT1 = client1.o
PROJECT_CLIENT2 = client2.o
PROJECT_CLIENT = client.o
PROJECT = $(PROJECT_CLIENT1) $(PROJECT_CLIENT2) $(PROJECT_CLIENT)

OBJECT_CLIENT1 = client1
OBJECT_CLIENT2 = client2
OBJECT_CLIENT = client
OBJECT = $(OBJECT_CLIENT1) $(OBJECT_CLIENT2)  $(OBJECT_CLIENT)


$(OBJECT) : $(PROJECT)
	${CROSS_COMPILE}${CC} $(PROJECT_CLIENT1) -o $(OBJECT_CLIENT1) ${LDFLAGS}
	${CROSS_COMPILE}${CC} $(PROJECT_CLIENT2) -o $(OBJECT_CLIENT2) ${LDFLAGS}
	${CROSS_COMPILE}${CC} $(PROJECT_CLIENT) -o $(OBJECT_CLIENT) ${LDFLAGS}

$(PROJECT) : $(SOURCE)
	${CROSS_COMPILE}${CC} -c $^ ${CFLAGS}

clean :
	rm -f $(PROJECT) $(OBJECT) *.o

