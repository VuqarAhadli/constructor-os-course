#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>


typedef struct 
{
	int (*catFunc)(void);
}Cat;

int getcCopy(void)
{
	int ch;

	while ((ch = getc(stdin)) != EOF)
	{
		if(putc(ch, stdout) == EOF)
		{
			perror("unexpected EOF");
			return 1;
		}
	}

	if (ferror(stdin)) 
	{
		perror("error reading from stdin"); 
		return 1;
	}

	return 0;
}


int readCopy(void)
{
	char ch;
	ssize_t size;

	while ((size = read(STDIN_FILENO, &ch, 1)) > 0)
	{
		ssize_t current = write(STDOUT_FILENO, &ch, 1);

		if (current != 1)
		{
			perror("unable to write");
			return 1;
		}
	}

	if (size == -1)
	{
		perror("couldn't read from stdin");
		return 1;
	}

	return 0;
}

int sendFileCopy(void)
{
	struct stat st;

	if(fstat(STDIN_FILENO, &st) == -1)
	{
		perror("error reading from stdin\n");
		return 1;
	}

	off_t offset = 0;

	while (offset < st.st_size)
	{	
		ssize_t sent = sendfile(STDOUT_FILENO, STDIN_FILENO, &offset, st.st_size - offset);
			
		if (sent == -1)
		{
			perror("error sending from stdin to stdout\n");
			return 1;
		}

		if (sent == 0)
		{
			break;
		}


	}

	return 0;

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

	int returnCode = cat.catFunc();
	return returnCode;
}
