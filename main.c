#include "ets_sys.h"
#include "gpio.h"
#include "hw_timer.c"
#include "mem.h"
#include "os_type.h"
#include "osapi.h"
#include "sntp.h"
#include "uart.h"
#include "user_interface.h"

#define HW_TIMER_INTERVAL 1000
#define PIN_COUNT         7

static uint32_t pins[PIN_COUNT] = { 13, 5, 4, 0, 2, 14, 12 };

static volatile uint8_t initialized = 0;
static volatile uint32_t counter = 0;

void hw_timerfunc(void)
{
    uint8_t c;

    if (initialized == 0) {
	for (c = 0; c < PIN_COUNT; ++c) {
	    GPIO_OUTPUT_SET(pins[c], 0);
	}
	initialized = 1;
    }

    GPIO_OUTPUT_SET(pins[counter % PIN_COUNT], 1);
    GPIO_OUTPUT_SET(pins[(counter + PIN_COUNT - 1) % PIN_COUNT], 0);
#if 0
    os_printf("Counter: %d, GPIO: %d\n", counter, pins[counter % PIN_COUNT]);
#endif
    ++counter;
}

void ICACHE_FLASH_ATTR user_init(void)
{
    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    gpio_init();
    
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);

    hw_timer_init(NMI_SOURCE, 0);
    hw_timer_set_func(hw_timerfunc);
    hw_timer_arm(HW_TIMER_INTERVAL);
}

