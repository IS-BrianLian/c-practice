#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	clock_t begin, end;
	unsigned int begin_time, end_time;
	int i;

	begin = clock();
	begin_time = time(NULL);

	
	i = 0;
	while(1) {
		i++;
		if (i > 10000 * 20) {
			break;
		}
	}

	end = clock();
	end_time = time(NULL);

	printf("Excute Time is %lf Sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
	printf("Excute Time is %d Sec.\n", (end_time - begin_time));

	return 0;
}
