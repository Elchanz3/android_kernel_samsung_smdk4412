#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define FIFO "/var/run/acpi_fakekey"

int main(int argc, char** argv) {
	int fd;
	unsigned char key;

	if (argc != 2) {
		printf("Usage: %s <key>\n", argv[0]);
		return EXIT_FAILURE;
	}
	key = atoi(argv[1]);

	if ((fd = open(FIFO, O_WRONLY)) == -1) {
		perror("fifo");
		return EXIT_FAILURE;
	}
	if (write(fd, &key, 1) == -1) {
		perror("write");
		return EXIT_FAILURE;
	}

	close(fd);
	return EXIT_SUCCESS;
}
