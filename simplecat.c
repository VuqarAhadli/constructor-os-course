#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>


typedef struct 
{
	void (*catFunc)(void);
}Cat;

void getcCopy(void)
{
	int ch;

	while ((ch = getc(stdin)) != EOF)
	{
		if(putc(ch, stdout) == EOF)
		{
			perror("unexpected EOF");
			return;
		}
	}

	if (ferror(stdin)) 
	{
		perror("could not open stdin"); 
	}

	if(fflush(stdout) == EOF)
	{ 
		perror("improper ending of a sequence");
	}

}


void readCopy(void)
{
	char ch;
	ssize_t size;

	while((size = read(STDIN_FILENO, &ch, 1)) > 0)
	{
		ssize_t sent = 0;
		
		while(sent < size)
		{
			ssize_t current = write(STDOUT_FILENO, &ch + sent, size - sent);
			
			if(current == -1)
			{
				perror("unable to write\n");
				return;
			}

			sent += current;
		}
	}

	if(size == -1)
	{
		perror("couldnt read the file\n");
	}
}


void sendFileCopy(void)
{
	struct stat st;

	if(fstat(STDIN_FILENO, &st) == -1)
	{
		perror("error reading from stdin\n");
		return;
	}

	off_t offset = 0;

	while (offset < st.st_size)
	{	
		ssize_t sent = sendfile(STDOUT_FILENO, STDIN_FILENO, &offset, st.st_size);
			
		if (sent == -1)
		{
			perror("error sending from stdin to stdout\n");
			return;
		}

		if (sent == 0)
		{
			break;
		}


	}

}

int main(int argc, char *argv[])
{
	Cat cat;

	cat.catFunc = getcCopy;

	int opt;

	while((opt = getopt(argc, argv, "lsp")) != -1)
	{
		switch (opt)
		{	
			case 'l':
			{
				cat.catFunc = getcCopy;
				break;
			}
			case 's':
			{
				cat.catFunc = readCopy;
				break;
			}
			case 'p':
			{
				cat.catFunc = sendFileCopy;
				break;
			}
			default:
			{
				return 1;
			}
		}
	}

	cat.catFunc();
	return 0;
}
