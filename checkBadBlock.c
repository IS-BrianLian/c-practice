#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <mtd/mtd-user.h>
#include <mtd/mtd-abi.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/fcntl.h>

int main() {
	int fd;
	mtd_info_t meminfo;
	
	fd = open("/dev/mtd3", O_RDONLY);
	ioctl(fd, MEMGETINFO, &meminfo);
	printf("type: 0x%X\n", meminfo.type);
	printf("flags: 0x%X\n", meminfo.flags);
	printf("size: 0x%X\n", meminfo.size);
	printf("erasesize: 0x%X\n", meminfo.erasesize);
	printf("writesize: 0x%X\n", meminfo.writesize);
	printf("oobsize: 0x%X\n", meminfo.oobsize);
	printf("padding: 0x%X\n", meminfo.padding);
	printf("\n");

	int iNumOfBadBlocks = get_bb_number(fd, &meminfo);
    printf("Number of the bad blocks is %d\n", iNumOfBadBlocks);
}


int get_bb_number(int fd, const mtd_info_t *meminfo) {
    int isNAND = meminfo->type == MTD_NANDFLASH ? 1 : 0;
    int ibbCounter = 0;
    erase_info_t erase;
    erase.length = meminfo->erasesize;
	

    for (erase.start = 0; erase.start < meminfo->size; erase.start += meminfo->erasesize) {
        //loff_t offset = erase.start;
        long long offset = erase.start;

        int ret = ioctl(fd, MEMGETBADBLOCK, &offset);
        if (ret > 0) {
			printf("Bad Block at 0x%llX\n", offset);
            ibbCounter++;
            continue;
        } else if (ret < 0) {
            if (errno == EOPNOTSUPP) {
                if (isNAND) {
                    printf("Bad block check notavailable\n");
                    exit(1);
                }
            } else {
                printf("MTD get bad block failed: %d\n", strerror(errno));
                exit(1);
            }
        }
    }

    return ibbCounter;
}
