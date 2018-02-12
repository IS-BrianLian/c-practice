#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	unsigned char *ADDR; 
	int device_pointer;
	device_pointer = open("/dev/mem", O_RDWR);
	if (device_pointer < 0){
		printf("device file open error !, %d\n", device_pointer);
		return 0;
	}
	ADDR = mmap(NULL, 0xFFFFF, PROT_READ|PROT_WRITE, MAP_SHARED, device_pointer, 0);
	//*(volatile unsigned int *)(ADDR + 0x00) = 0x1; //操作實際位址0x37F80000，令其值為1
	//*(volatile unsigned int *)(ADDR + 0x04) = 0x1; //操作實際位址0x37F80004，令其值為1

	printf("%X\n", *(volatile unsigned int *)(ADDR+0x32000));
	munmap(ADDR, 0xFFFFF);
	close(device_pointer);
	return 0;
}
