#include <stdio.h>
#include <stdlib.h>

#define SIZE_MB ((1 * 1024) * 1024)

int main(int argc, char *argv[]) {
	FILE *fp;
	int i, fileSize;
	char *buff;
	char chars[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

	fileSize = atoi(argv[1]);
	fileSize = SIZE_MB * sizeof(char) * fileSize;
	buff = malloc(fileSize);
	printf("Generate file size is %dMB\n", fileSize/1024/1024);
    
	fp = fopen("image.bin", "wb+");
	if (!fp) {
		fclose(fp);
		return -1;
	}

	for (i=0; i < fileSize; i++) {
		buff[i] = chars[0];
		buff[i] = 0x55;
	}
  
	fwrite(buff, sizeof(unsigned char), fileSize, fp);
	fclose(fp);
	
	free(buff);
        
	return 0;
}
