/*
 * Make sure to consult:
 * http://elinux.org/BCM2835_datasheet_errata
 *
 * This is for the full UART (uart0), but has good clean code:
 * http://wiki.osdev.org/Raspberry_Pi_Bare_Bones
 *
 * Also, for this project's console purposes, ignore hardware driven
 * flow control (RTS/CTS) signaling capabilities. That reqires additional
 * wires and GPIO headers connected that I don't have.
 */
#include "aux.h"
#include "gpio.h"
#include "types.h"
#include "system.h"

#define AUX_IRQ     0x3F215000 /* Auxiliary Interrupt status. size: 3 */
#define AUX_ENABLES 0x3F215004 /* Auxiliary enables. size: 3 */

#define MU_ENABLE_MASK 0x1

#define AUX_MU_IO_REG   0x3F215040 /* Mini Uart I/O Data. size: 8 */
#define AUX_MU_IER_REG  0x3F215044 /* Mini Uart Interrupt Enable. size: 8 */
#define AUX_MU_IIR_REG  0x3F215048 /* Mini Uart Interrupt Identify. size: 8 */
#define AUX_MU_LCR_REG  0x3F21504C /* Mini Uart Line Control. size: 8 */
#define AUX_MU_MCR_REG  0x3F215050 /* Mini Uart Modem Control. size: 8 */
#define AUX_MU_LSR_REG  0x3F215054 /* Mini Uart Line Status. size: 8 */
#define AUX_MU_MSR_REG  0x3F215058 /* Mini Uart Modem Status. size: 8 */
#define AUX_MU_SCRATCH  0x3F21505C /* Mini Uart Scratch. size: 8 */
#define AUX_MU_CNTL_REG 0x3F215060 /* Mini Uart Extra Control. size: 8 */
#define AUX_MU_STAT_REG 0x3F215064 /* Mini Uart Extra Status. size: 32 */
#define AUX_MU_BAUD_REG 0x3F215068 /* Mini Uart Baudrate. size: 16 */

static void init_gpio_for_mu() {

	u32 io_reg_val;

	wmb();

	/* set GPIO pins 14 and 15 to alt-func-5 */
	io_reg_val = *(volatile u32 *)GPFSEL1;
	io_reg_val &= ~(7 << GPIO_14_SEL_SHIFT); /* blank out 14's 3 bits */
	io_reg_val |= (GPIO_ALT_FUNC_5 << GPIO_14_SEL_SHIFT);
	io_reg_val &= ~(7 << GPIO_15_SEL_SHIFT); /* blank out 15's 3 bits */
	io_reg_val |= (GPIO_ALT_FUNC_5 << GPIO_15_SEL_SHIFT);
	*(volatile u32 *)GPFSEL1 = io_reg_val;

	/*
	 * set the next GPIO pull config update to be "no pull"
	 * then wait 150 cycles to let this settle to the device.
	 */
	*(volatile u32 *)GPPUD = 0;
	delay(150);

	/*
	 * send the staged GPIO pull config update to pins 14 and 15
	 * then wait 150 cycles to let this settle to the device.
	 * then clear the instruction so the update is not sent again
	 */
	*(volatile u32 *)GPPUDCLK0 = (1 << 14) | (1 << 15);
	delay(150);
	*(volatile u32 *)GPPUDCLK0 = 0;

	rmb();
}

s32 aux_mu_init()
{
	u32 reg_val;

	init_gpio_for_mu();

	wmb();

	/* enable the mini-uart (and its control registers) */
	reg_val = *(volatile u32 *)AUX_ENABLES;
	reg_val |= 0x1;
	*(volatile u32 *)AUX_ENABLES = reg_val;

	/* temporarily disable RX and TX */
	reg_val = *(volatile u32 *)AUX_MU_CNTL_REG;
	reg_val &= ~0x3;
	*(volatile u32 *)AUX_MU_CNTL_REG = reg_val;

	/* clear the UART's fifo's (both RX and TX) */
	reg_val = *(volatile u32 *)AUX_MU_IIR_REG;
	reg_val |= 0x6;
	*(volatile u32 *)AUX_MU_IIR_REG = reg_val;

	/*
	 * set for 8-bit (not 7-bit) transimission
	 * there is a bug in the datasheet that says this should be '1' not '3'
	 * http://elinux.org/BCM2835_datasheet_errata
	 */
	reg_val = *(volatile u32 *)AUX_MU_LCR_REG;
	reg_val |= 0x3;
	*(volatile u32 *)AUX_MU_LCR_REG = reg_val;

	/*
	 * set the uart's baudrate_reg for a 115200 baudrate.
	 *
	 * baudrate = system_clock_feq / (8*(baudrate_reg + 1))
	 *
	 * If the Pi is confiruged for 400MHz core clock rate:
	 * 115,200 = 400,000,000 / (8*(baudrate_reg+1))
	 * baudrate_reg = 433.02777  (433)
	 *
	 * If the Pi is confiruged for 250MHz core clock rate:
	 * 115,200 = 250,000,000 / (8*(baudrate_reg+1))
	 * baudrate_reg = 270.267   (270)
	 *
	 * Note the mini-uart uses the video-core's clockrate to drive
	 * its sampling, not the ARM cores.
	 *
	 */
	*(volatile u32 *)AUX_MU_BAUD_REG = 433; /* a decimal value */

	/* re-Enable RX and TX */
	reg_val = *(volatile u32 *)AUX_MU_CNTL_REG;
	reg_val |= 0x3;
	*(volatile u32 *)AUX_MU_CNTL_REG = reg_val;

	rmb();

	return 0;
}

s32 aux_mu_tx_byte_busywait(u8 byte)
{
	u32 lsr_reg_val;
	wmb();
	while (1)
	{
		lsr_reg_val = *(volatile u32 *)AUX_MU_LSR_REG;
		if (lsr_reg_val & 0x20) {
			/* Transmitter Empty. Fifo can accept at least one byte. */
			break;
		}
	}
	*(volatile u32 *)AUX_MU_IO_REG = byte;
	rmb();
	return 0;
}

u8 aux_mu_rx_byte_busywait()
{
	u32 lsr_reg_val;
	u8 retval;

	wmb();
	while (1)
	{
		lsr_reg_val = *(volatile u32 *)AUX_MU_LSR_REG;
		if (lsr_reg_val & 0x1) {
			/* Data ready. RX fifo has at least one symbol. */
			break;
		}
	}
	retval = *(volatile u32 *)AUX_MU_IO_REG;
	rmb();
	return retval;
}
