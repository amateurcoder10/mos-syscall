#include <stdio.h>

int main(int argc, char *argv[])
{
	char name[128];
	char secret[128];

	printf("Name : ");
	scanf("%s", name);
	printf("secret : ");
	scanf("%s", secret);
	printf("hello %s your secret is safe with me!\n\nPress enter key to exit\n", name);
	getchar();
}
