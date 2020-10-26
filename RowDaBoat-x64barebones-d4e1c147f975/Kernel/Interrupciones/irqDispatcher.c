//#include "time.h"
#include <stdint.h>
#include "keyboardDriver.h"
#include "interrupts.h"

void irqDispatcher(uint64_t irq/*,uint64_t rsp*/) {
	switch (irq) {
		// case 0:
		// 	int_20();
		// 	break;
		case 1:
			int_21();
			break;
	}
	return;
}

// void int_20() {
// 	timer_handler();
// }
void int_21() {
	keyboardHandler();
}
