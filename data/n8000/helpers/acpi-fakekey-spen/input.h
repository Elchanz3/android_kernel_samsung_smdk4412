#include <inttypes.h>

#include <sys/ioctl.h>
#include <linux/input.h>

#define ev_name(code)  ((code) < EV_MAX  && EV_NAME[code]  ? EV_NAME[code]  : "???")
#define ev_type_name(type, code) ((code) < EV_TYPE_MAX[type] && EV_TYPE_NAME[type][code] ? EV_TYPE_NAME[type][code] : "???")

#define BITFIELD uint32_t

static __inline__ int test_bit(int nr, BITFIELD * addr)
{
	BITFIELD mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}

int device_open(int nr);
int process_event(struct input_event *event);

