#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	pid_t pid = getpid();
	printf("mypid: %u\n", pid);
	return 0;
}
