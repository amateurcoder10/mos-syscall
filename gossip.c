#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

unsigned int mask = '*';

static void putstr(char *s)      { while (*s != '\0') putchar(*s++); }

void secret()
{
	struct termios old, new;
	int c;

	tcgetattr( STDIN_FILENO, &old);//get old attributes
	new = old;//copy into new
	new.c_lflag &= ~( ICANON | ECHO);//turn off echo and canonical mode
	tcsetattr( STDIN_FILENO, TCSANOW, &new);//set attribute to new immediately

	while ((c=getchar()) != '\r' && c != '\n')//read one character at a time till newline is entered
		{	
			putchar(mask);//echo the mask character
	
		}
		


	tcsetattr( STDIN_FILENO, TCSANOW, &old);//reset to old attributes before exiting
	return;
}

int main()
{
	char buf[1024], c;
	int i;
	
	putstr("Namaste!\n");
	putstr("login : ");
	for (i = 0; i < 1024; i++) {
		buf[i] = getchar();
		if (buf[i] == '\n') break;
	}
	buf[i] = '\0';
	putstr("Secret : ");
	secret();//call the secret function

	putstr("\nhello ");
	putstr(buf);
	putstr(", your secret is safe with me!\n\nPress Enter key to exit\n");
	getchar();
}
