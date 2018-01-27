#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mount.h>


#define TESTING

char *
ramdisk(const char *ns, const char *sz)
{
	int rc;
	char *mountpoint = NULL, *options = NULL;
	char path[PATH_MAX];

	memset(path, 0, sizeof(path));

	const char *tmpdir = getenv("TMPDIR");
	if (! tmpdir){
		tmpdir = "/tmp/";
	}

	snprintf(path, sizeof(path)-1, "%s%s_XXXXXX", tmpdir, ns);

	mountpoint = mkdtemp(path);
	if (!mountpoint){
		return NULL;
	}

	asprintf(&options, "size=%s,uid=0,gid=0,mode=700", sz);
	rc = mount("tmpfs", mountpoint, "tmpfs", 0, options);
	free(options);

	if (rc != 0){
		perror("tmpfs creation failed");
		rmdir(mountpoint);
		return NULL;
	}
	return strdup(mountpoint);
}

#ifdef TESTING
int 
main()
{
	char *tmpfs = ramdisk("ramdisk", "1M");
	if (tmpfs != NULL){
		printf("created tmpfs here: %s\n", tmpfs);
		free(tmpfs);
	}
	return 0;
}
#endif
