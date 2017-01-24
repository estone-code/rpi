#ifndef GPIO_H
#define GPIO_H

/*
 * http://pinout.xyz/ The GPIO pins have two numbers, the physical pin
 * number seen in a lot of pictures on the internet (1-40), and then a
 * "broadcom name" like GPIO7 or GPIO8 (also called the "BCM pin
 * number"). Unfortunately the numbers don't match up. The pinout
 * website is really helpful, it's interactive so click on the pins.
 *
 * Each pin can be programmed to take on one of (up to 6) alternate
 * functions. For initial UART programming, we want to use ALT5
 * for both the TX and RX pins.
 *
 * UART14 aka phys-pin8 is TXD for UART1 under ALT5
 * UART15 aka phys-pin10 is RXD for UART1 under ALT5
 */

#define GPFSEL0   0x3F200000 /* GPIO Function Select 0. size: 32 */
#define GPFSEL1   0x3F200004 /* GPIO Function Select 1. size: 32 */
#define GPFSEL2   0x3F200008 /* GPIO Function Select 2. size: 32 */
#define GPFSEL3   0x3F20000C /* GPIO Function Select 3. size: 32 */
#define GPFSEL4   0x3F200010 /* GPIO Function Select 4. size: 32 */
#define GPFSEL5   0x3F200014 /* GPIO Function Select 5. size: 32 */
#define GPSET0    0x3F20001C /* GPIO Pin Output Set 0. size: 32 */
#define GPSET1    0x3F200020 /* GPIO Pin Output Set 1. size: 32 */
#define GPCLR0    0x3F200028 /* GPIO Pin Output Clear 0. size: 32 */
#define GPCLR1    0x3F20002C /* GPIO Pin Output Clear 1. size: 32 */
#define GPLEV0    0x3F200034 /* GPIO Pin Level 0. size: 32 */
#define GPLEV1    0x3F200038 /* GPIO Pin Level 1. size: 32 */
#define GPEDS0    0x3F200040 /* GPIO Pin Event Detect Status 0. size: 32 */
#define GPEDS1    0x3F200044 /* GPIO Pin Event Detect Status 1. size: 32 */
#define GPREN0    0x3F20004C /* GPIO Pin Rising Edge Detect Enable 0. size: 32 */
#define GPREN1    0x3F200050 /* GPIO Pin Rising Edge Detect Enable 1. size: 32 */
#define GPFEN0    0x3F200058 /* GPIO Pin Falling Edge Detect Enable 0. size: 32 */
#define GPFEN1    0x3F20005C /* GPIO Pin Falling Edge Detect Enable 1. size: 32 */
#define GPHEN0    0x3F200064 /* GPIO Pin High Detect Enable 0. size: 32 */
#define GPHEN1    0x3F200068 /* GPIO Pin High Detect Enable 1. size: 32 */
#define GPLEN0    0x3F200070 /* GPIO Pin Low Detect Enable 0. size: 32 */
#define GPLEN1    0x3F200074 /* GPIO Pin Low Detect Enable 1. size: 32 */
#define GPAREN0   0x3F20007C /* GPIO Pin Async. Rising Edge Detect 0. size: 32 */
#define GPAREN1   0x3F200080 /* GPIO Pin Async. Rising Edge Detect 1. size: 32 */
#define GPAFEN0   0x3F200088 /* GPIO Pin Async. Falling Edge Detect 0. size: 32 */
#define GPAFEN1   0x3F20008C /* GPIO Pin Async. Falling Edge Detect 1. size: 32 */
#define GPPUD     0x3F200094 /* GPIO Pin Pull-up/down Enable. size: 32 */
#define GPPUDCLK0 0x3F200098 /* GPIO Pin Pull-up/down Enable Clock 0. size: 32 */
#define GPPUDCLK1 0x3F20009C /* GPIO Pin Pull-up/down Enable Clock 1. size: 32 */

/* for GPFSEL* registers */
#define GPIO_IS_INPUT   0b000
#define GPIO_IS_OUTPUT  0b001
#define GPIO_ALT_FUNC_0 0b100
#define GPIO_ALT_FUNC_1 0b101
#define GPIO_ALT_FUNC_2 0b110
#define GPIO_ALT_FUNC_3 0b111
#define GPIO_ALT_FUNC_4 0b011
#define GPIO_ALT_FUNC_5 0b010

#define GPIO_14_SEL_SHIFT 12
#define GPIO_15_SEL_SHIFT 15

#endif /* GPIO_H */
