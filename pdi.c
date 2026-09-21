#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>

int lo = 1;
int hi = 10000;
bool verbose = false;
int threadCount = 1;

int intLen (int num)
{
	int len = 0;

	if (num == 0) return 1;
	
	while (num)
	{
		++len;
		num /= 10;	
	}

	return len;
}

bool isPdi (int num)
{
	int len = intLen(num);
	long long res = 0;
	int num1 = num;

	
	if (num >= 0 && num <= 9) return true;

	while (num)
	{
		int digit = num % 10;
		long long power = 1;

		for (int i = 0;  i < len; ++i)
		{
			power *= digit;
		}

		res += power;
	      	num /= 10;	
	}

	return res == num1;

}

typedef struct
{
	int high;
	int low;
	int threadNum;
	bool verbose;

} ThreadData;



void *isPdiRange(void *arg)
{
	ThreadData *data = arg;

	if (data->verbose)
	{
		printf("Thread %i is searching [%i - %i]\n", data->threadNum, data->low ,data->high);
	}
	for (int i = data->low ; ; ++i)
	{
		bool pdi = isPdi(i);
		if (pdi) printf("Number %i is pdi\n", i);
		if(pdi && i % 10 == 0 && i + 1 <= data->high)
		{
			printf("Number %i is pdi\n", i+1);
			i++;
		}

		if (i == data->high)
		{
			if (data->verbose)
			{
				printf("Thread %i is stopping\n", data->threadNum);
			}
			break;
		}
	}
	return NULL;
}

int main (int argc, char *argv[])
{
	int opt;
	
	while ((opt = getopt(argc, argv, "s:e:t:v")) != -1)
	{
		switch (opt)
		{
			case 's':
			{
				lo = atoi(optarg); 
				break;
			}
			
			case 'e':
			{
				if (strcmp(optarg, "INT_MAX") == 0) hi = INT_MAX;
				else hi = atoi(optarg);

				break;
			}

			case 'v':
			{
				verbose = true;
				break;
			}

			case 't':
			{
				threadCount = atoi(optarg);
				break;
			}

			default:
				break;
		}
	}	

	if (threadCount < 1) threadCount = 1;

	if (threadCount > hi - lo + 1) threadCount = hi - lo + 1;
	
	pthread_t *threads = malloc(threadCount * sizeof(pthread_t));
	ThreadData *data = malloc(threadCount * sizeof(ThreadData));

	int range = (hi - lo + 1) / threadCount;
	int rem = (hi - lo + 1) % threadCount;

	int start = lo;

	for (int i = 0; i < threadCount ; ++i)
	{
		int size = range;
		
		if ( i < rem)
		{
			++size;
		}	

		data[i].low = start;
		data[i].high = start + size - 1;
		data[i].threadNum = i;
		data[i].verbose = verbose;

		pthread_create(&threads[i], NULL, isPdiRange, &data[i]);

		start = data[i].high + 1;
	}


	for (int i = 0; i < threadCount ; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	free(threads);
	free(data);
	return 0;
}
