#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

int main() {
  uint32_t data;
  
  while(1) {
    scanf(" %X", &data);
    printf("%s ", ((data>>26)&0x01)==1? "r":"w");
    printf("0x%X 0x%X ", (data>>21)&0x1F, (data>>16)&0x1F);
    printf("0x%04X\n", data&0xFFFF);
  }

  return 0;
}
