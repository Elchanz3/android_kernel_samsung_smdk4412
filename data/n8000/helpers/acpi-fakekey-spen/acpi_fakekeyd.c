#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/uinput.h>
#include <sys/stat.h>

#define FIFO "/var/run/acpi_fakekey"

void fail(const char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

void daemonize() {
	int pid;
	if ((pid = fork()) == -1)
		fail("fork");

	if (pid)
		exit(EXIT_SUCCESS);

	if (setsid() == -1)
		fail("setsid");

	if (chdir("/") == -1)
		fail("chdir");

	if (!freopen("/dev/null", "r", stdin))
		fail("freopen");
	if (!freopen("/dev/null", "w", stdout))
		fail("freopen");
	if (!freopen("/dev/null", "w", stderr))
		fail("freopen");
}

int main(int argc, char** argv) {
	int fd;
	int fifo;
	int i;
	struct uinput_user_dev dev;
	fd_set sfd;

	if ((fd = open("/dev/uinput", O_WRONLY | O_NDELAY)) == -1)
		fail("open device");

	memset(&dev, 0, sizeof(dev));
	strncpy(dev.name, "ACPI Virtual Keyboard Device", UINPUT_MAX_NAME_SIZE);
	dev.id.version = 4;
	dev.id.bustype = BUS_USB;

	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	for (i = 0; i < 256; i++)
		ioctl(fd, UI_SET_KEYBIT, i);
	if (write(fd, &dev, sizeof(dev)) == -1)
		fail("write");

	if (ioctl(fd, UI_DEV_CREATE) == -1)
		fail("create device");

	remove(FIFO);
	if (mkfifo(FIFO, 0200) == -1)
		fail("mkfifo");
	
	if ((fifo = open(FIFO, O_RDWR | O_NONBLOCK)) == -1)
		fail("open fifo");

	daemonize();

	FD_ZERO(&sfd);
	FD_SET(fifo, &sfd);
	while (select(fifo+1, &sfd, 0, 0, 0) != -1) {
		int n;
		unsigned char key; 
		struct input_event event;
		if ((n = read(fifo, &key, 1)) == -1)
			break;
		if (!n)
			continue;

		event.type = EV_KEY;
		event.code = key;
		event.value = 1;
		if (write(fd, &event, sizeof event) == -1)
			break;
		event.value = 0;
		if (write(fd, &event, sizeof event) == -1)
			break;
		/* Need to write sync event */
		memset(&event, 0, sizeof(event));
		event.type = EV_SYN;
		event.code = SYN_REPORT;
		event.value = 0;
		if (write(fd, &event, sizeof event) == -1)
			break;
	}

	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	remove(FIFO);
	return EXIT_FAILURE;
}
