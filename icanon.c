#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

unsigned int mask = '*';

static void putstr(char *s)      { while (*s != '\0') putchar(*s++); }

void secret()
{struct termios old, new;
int c;
tcgetattr( STDIN_FILENO, &old);
new = old;
new.c_lflag &= ~( ICANON);
tcsetattr( STDIN_FILENO, TCSANOW, &new);

while ((c=getchar()) != '\r' && c != '\n')
	{	
		putchar(c);
	
	}
		


tcsetattr( STDIN_FILENO, TCSANOW, &old);
return;
}

int main(int argc,char *argv[], char *envp[])
{
	char buf[1024], c;
	int i;
	char *home, *host;

	putstr("Namaste!\n");
	putstr("login : ");
	for (i = 0; i < 1024; i++) {
		buf[i] = getchar();
		if (buf[i] == '\n') break;
	}
	buf[i] = '\0';
	putstr("Secret : ");
	secret();
	
	putstr("\nhello ");
	putstr(buf);
	putstr(", your secret is safe with me!\n\nPress Enter key to exit\n");
	getchar();
}
