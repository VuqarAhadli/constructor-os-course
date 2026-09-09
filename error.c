#define _DEFAULT_SOURCE

#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>


void errorPrinterHelper(const char *funcName)
{
	fprintf(stderr, "[%s%s%s] %s |errno: %d\n", ANSI_RED ,funcName, ANSI_RESET, strerror(errno), errno); 
}

int main()
{
	int fd = open("answers.txt", O_EXCL | O_CREAT, 0644);
	int pid = 1;

	if(fd == -1)
	{
		errorPrinterHelper("open");
	}

	if (close(-1) == -1)
	{
		errorPrinterHelper("close");
	}
	
	char* invBuf = NULL;

	if (write(STDOUT_FILENO, invBuf, 21) == -1)
	{
		errorPrinterHelper("write");
	}


	if(kill(pid,SIGINT) == -1)
	{
		errorPrinterHelper("kill");
	}
	
	return 0;
}
