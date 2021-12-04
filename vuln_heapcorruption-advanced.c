#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <dlfcn.h>

#define __libc_dlopen(name) dlopen(name, RTLD_NOW)

// https://code.woboq.org/userspace/glibc/nss/nsswitch.h.html
typedef enum
{
	NSS_ACTION_CONTINUE,
	NSS_ACTION_RETURN,
	NSS_ACTION_MERGE
} lookup_actions;

// https://code.woboq.org/userspace/glibc/nss/nsswitch.h.html
typedef struct service_library
{
	/* Name of service (`files', `dns', `nis', ...).  */
	const char *name;
	/* Pointer to the loaded shared library.  */
	void *lib_handle;
	/* And the link to the next entry.  */
	struct service_library *next;
} service_library;

// https://code.woboq.org/userspace/glibc/nss/nsswitch.h.html
typedef struct service_user
{
	/* And the link to the next entry.  */
	struct service_user *next;
	/* Action according to result.  */
	lookup_actions actions[5];
	/* Link to the underlying library object.  */
	service_library *library;
	/* Collection of known functions.  */
	void *known;
	/* Name of the service (`files', `dns', `nis', ...).  */
	char name[0];
} service_user;

// https://code.woboq.org/userspace/include/gnu/lib-names-64.h.html
#define LIBNSS_FILES_SO "libnss_files.so.2"

// https://code.woboq.org/userspace/glibc/nss/nsswitch.c.html
static const char *const __nss_shlib_revision = LIBNSS_FILES_SO + 15;

// https://code.woboq.org/userspace/glibc/nss/nsswitch.c.html
static int
nss_load_library(service_user *ni)
{
	// TODO: uncomment
	//if (ni->library->lib_handle == NULL)
	//{
		/* Load the shared library.  */
		size_t shlen = (7 + strlen(ni->name) + 3 + strlen(__nss_shlib_revision) + 1);
		int saved_errno = errno;
		char shlib_name[shlen];
		/* Construct shared object name.  */
		__stpcpy(__stpcpy(__stpcpy(__stpcpy(shlib_name,
											"libnss_"),
								   ni->name),
						  ".so"),
				 __nss_shlib_revision);
		// TODO: buffer overflow needs to have this effect: strcpy(shlib_name, "libnss_X/X.so.2");
		ni->library->lib_handle = __libc_dlopen(shlib_name);
	//}
	return 0;
}

int main(int argc, char **argv)
{
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

	// allocated on the heap somewhere behind user_args
	service_user *ni = malloc(sizeof(service_user));
	ni->library = malloc(sizeof(service_library));

	// heap buffer overflow vulnerability
	// https://github.com/sudo-project/sudo/blob/8255ed69b9c426d90a10c6d68e8d2241d7f3260e/plugins/sudoers/sudoers.c#L877
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

	// call nss_load_libary somewhere behind heap buffer overflow
	nss_load_library(ni);
	exit(EXIT_SUCCESS);
}