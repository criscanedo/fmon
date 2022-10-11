#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include "handles.h"

struct action {
	uint32_t imask;
	void (*handle)(struct file_info *);
} actions[] = {
	{ IN_MODIFY, printlines },
};

#endif //CONFIG_H
