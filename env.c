#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

extern char **environ;

void printEnv()
{
	for (char **env = environ; *env != NULL; ++env)
	{
		printf("%s\n", *env);
	}	
}

void deleteEntry(char *name)
{

	size_t nameLength = strlen(name);

	for (char **env = environ; *env != NULL; ++env)
	{
		if (strncmp(*env, name, nameLength) == 0 && (*env)[nameLength] == '=' )
		{	
			char **point = env;
			while(*point != NULL)
			{
				*point = *(point + 1);
				++point;
			}
			return;
		}
	}	
}



int main (int argc, char *argv[])
{
	int opt;
	bool verbose = false;

	while (( opt = getopt(argc, argv,"vu:")) != -1)
	{
		switch (opt)
		{
			case 'v':
				verbose = true;
				break;
			case 'u':
				if (verbose)
				{
					fprintf(stderr, "removing %s\n", optarg);
				}
				deleteEntry(optarg);

				break;
			default:
				return 1;
		}
	}

	int indexStart = optind;

	while(indexStart < argc && strchr(argv[indexStart], '=') != NULL)
	{
		char *isEqual = strchr(argv[indexStart], '=');
		size_t nameLen = isEqual - argv[indexStart];

		char name[nameLen + 1];

		memcpy(name, argv[indexStart], nameLen);
		name[nameLen] = '\0';

		if (verbose)
        	{
            		fprintf(stderr, "setting %s\n", argv[indexStart]);
	        }

		setenv(name, isEqual + 1, 1);

		++indexStart;

	}

	if (indexStart < argc)
	{
		if (verbose)
        	{
        	    fprintf(stderr, "executing %s\n", argv[indexStart]);
	        }

		execvp(argv[indexStart], &argv[indexStart]);
		perror("execvp failed");

		return 1;
	}

	printEnv();
	return 0;
}
