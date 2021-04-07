
#include "debug.h"

void dbgInit() {
    // Initialization of GPIO
    GPIO_init();

    // Bit 7, GPIO P03
    GPIO_setConfig(CONFIG_GPIO_DEBUG_7, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 6, GPIO P05
    GPIO_setConfig(CONFIG_GPIO_DEBUG_6, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 5, GPIO P06
    GPIO_setConfig(CONFIG_GPIO_DEBUG_5, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 4, GPIO P07
    GPIO_setConfig(CONFIG_GPIO_DEBUG_4, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 3, GPIO P08
    GPIO_setConfig(CONFIG_GPIO_DEBUG_3, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 2, GPIO P18
    GPIO_setConfig(CONFIG_GPIO_DEBUG_2, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 1, GPIO P21
    GPIO_setConfig(CONFIG_GPIO_DEBUG_1, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    // Bit 0, GPIO P53
    GPIO_setConfig(CONFIG_GPIO_DEBUG_0, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

}

void dbgEvent(unsigned int event) {

	// convert event to binary
	unsigned int tempEvent = event;
	int binaryEvent[BINARY_EVENT_ARRAY_SIZE];
	int i;
	for(i = 0; i < BINARY_EVENT_ARRAY_SIZE; i++) {
		binaryEvent[i] = tempEvent % BINARY_CONVERSION;
		tempEvent = tempEvent / BINARY_CONVERSION;
	}
	
	// write debug data to GPIO pins
	GPIO_write(CONFIG_GPIO_DEBUG_7, HIGH);
	GPIO_write(CONFIG_GPIO_DEBUG_6, binaryEvent[BIT6]);
	GPIO_write(CONFIG_GPIO_DEBUG_5, binaryEvent[BIT5]);
	GPIO_write(CONFIG_GPIO_DEBUG_4, binaryEvent[BIT4]);
	GPIO_write(CONFIG_GPIO_DEBUG_3, binaryEvent[BIT3]);
	GPIO_write(CONFIG_GPIO_DEBUG_2, binaryEvent[BIT2]);
	GPIO_write(CONFIG_GPIO_DEBUG_1, binaryEvent[BIT1]);
	GPIO_write(CONFIG_GPIO_DEBUG_0, binaryEvent[BIT0]);
	GPIO_write(CONFIG_GPIO_DEBUG_7, LOW);
	
	if (event > FATAL_ERROR_LIMIT) {
		vTaskSuspendAll();
		//IntMasterDisable();
		// OR
		// Hwi_disable();
	}
}
