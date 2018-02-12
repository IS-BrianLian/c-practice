#include<stdio.h>

int main() {
    FILE *f;
	int i;

	f = fopen( "/etc/core/config.txt", "rt");
	if (f==NULL) {
		printf("config file is NULL\n");
		return -1;
	}

    for ( i=0;!feof(f) && i<100;i++) {
		char key[20];
		char val[80];
		int ntoks;
		ntoks = fscanf( f, "%20[^=]=%s\n", key, val);
		printf("ntoks=%d, key=%s, val=%s\n", ntoks, key, val);
	}

    return 0;
}
