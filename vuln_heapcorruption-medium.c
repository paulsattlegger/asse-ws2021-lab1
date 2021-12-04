#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef struct service_user
{
  /* ... */
  char name[32];
} service_user;

int main(int argc, char **argv)
{
	// TODO: right types?
	size_t size = 0;
	char **av;
	char *user_args;
	char *to;
	char *from;

	// TODO: this should allocated on the heap behind user_args
	// check on GDB where it is currently
	// and *somehow* get it in the right place
	service_user *user = malloc(sizeof(service_user));
	strcpy(user->name, "unprivileged");

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

	// in the original exploit, a shared library would be loaded with
	// the name libnss_{user->name}.so.2
	if (strcmp(user->name, "root") == 0) {
		printf("root access");
	} else {
		printf("unprivileged access");
	}
	printf("\n");

	exit(EXIT_SUCCESS);
}
