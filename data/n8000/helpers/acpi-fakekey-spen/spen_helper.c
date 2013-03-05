#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

#include "input.h"

#define FIFO "/var/run/acpi_fakekey"

#define SPEN_SWITCH 0x0E

unsigned char spen_open_key, spen_close_key;

int device_open(int nr) {
	char filename[32];
	int fd;

	snprintf(filename,sizeof(filename), "/dev/input/event%d",nr);

	fd = open(filename,O_RDONLY);

	if (fd == -1) {
		fprintf(stderr,"open %s: %s\n", filename,strerror(errno));

		return -1;
	};

	return fd;
};

int process_event(struct input_event *event) {
	int fd;
	unsigned char key;

	switch (event->type) {
		case EV_SW:
//			printf("%i %i\n", event->code, event->value);
			if (event->code == SPEN_SWITCH) {
				if ((fd = open(FIFO, O_WRONLY)) == -1) {
					perror("fifo");

					return EXIT_FAILURE;
				} else {
					key = event->value ? spen_open_key : spen_close_key;

					if (write(fd, &key, 1) == -1) {
						perror("write");

						return EXIT_FAILURE;
					};

					close(fd);
				};
			};
		break;
	};

	return 0;
};

int main(int argc, char** argv) {
	int devnr, devfd;
	struct input_event event;
	struct timeval tv;
	fd_set set;

	if (argc != 4) {
		printf("Usage: %s <devnr> <spen_open_key> <spen_close_key>\n", argv[0]);

		return EXIT_FAILURE;
	};

	devnr = atoi(argv[1]);
	spen_open_key = atoi(argv[2]);
	spen_close_key = atoi(argv[3]);

	devfd = device_open(devnr);

	while(1) {
		FD_ZERO(&set);
		FD_SET(devfd,&set);

		tv.tv_sec  = 10;
		tv.tv_usec = 0;

		switch (select(devfd+1,&set,NULL,NULL,&tv)) {
			case -1:
				perror("select");

				return EXIT_FAILURE;
			break;
		};

		if (FD_ISSET(devfd, &set)) {
			switch (read(devfd, &event, sizeof(event))) {
				case -1:
					perror("read");

					return EXIT_FAILURE;
				break;

				case 0:
					printf("EOF\n");

					return EXIT_FAILURE;
				break;

				default:
					process_event(&event);
				break;
			};
		};
	};

	close(devfd);

	return EXIT_SUCCESS;
}
