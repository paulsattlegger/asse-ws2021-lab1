#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char **argv)
{
	// TODO: right types?
	size_t size = 0;
	char **av;
	char *user_args;
	char *to;
	char *from;

	// allocate destination
	for (size = 0, av = argv + 1; *av; av++)
		size += strlen(*av) + 1;
	if (size == 0 || (user_args = malloc(size)) == NULL)
	{
		printf("Invalid size or malloc\n");
		exit(1);
	}

	// unescape vulnerability
	for (to = user_args, av = argv + 1; (from = *av); av++)
	{
		while (*from)
		{
			if (from[0] == '\\' && !isspace((unsigned char)from[1]))
				from++;
			*to++ = *from++;
		}
		*to++ = ' ';
	}

	printf("Done\n");

	exit(2);
}
