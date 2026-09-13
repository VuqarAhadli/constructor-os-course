#include <stdio.h>
#include <time.h>

int main()
{
	char buf[64];

	time_t now = time(NULL);
	struct tm *time = localtime(&now);

	strftime(buf, sizeof(buf), "%F", time);
	puts(buf);
	return 0;
}
