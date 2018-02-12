#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 10

struct Account {
	char username[40];
	char password[40];
};


int main(int argc, char *argv[]) {
	char *username;
	char *password;
	struct Account users[SIZE];
	char *substr = NULL;
	const char *delim = " ";
	int idx = 0;


	username = malloc(sizeof(char) * strlen(argv[1]));
	password = malloc(sizeof(char) * strlen(argv[2]));

	strcpy(username, argv[1]);
	strcpy(password, argv[2]);

	printf("username=%s\n", username);
	printf("password=%s\n", password);


	memset(users, 0, sizeof(struct Account) * SIZE);

	idx = 0;
	substr = strtok(username, delim);
	do {
		// printf("substr=%s\n", substr);
		strcpy(users[idx++].username, substr);
		substr = strtok(NULL, delim);
	} while (substr);

	idx = 0;
	substr = strtok(password, delim);
	do {
		// printf("substr=%s\n", substr);
		strcpy(users[idx++].password, substr);
		substr = strtok(NULL, delim);
	} while (substr);

	for(idx = 0; idx < SIZE; idx++) {
		printf("%d: u: %s, p: %s\n", idx, users[idx].username, users[idx].password);
	}

	free(password);
	free(username);

	return 0;
}
