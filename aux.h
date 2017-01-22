#ifndef AUX_H
#define AUX_H

/*
 * UART1 (the "mini UART"), SPI1, and SPI2 are grouped together as the
 * "Auxillary" peripherals in the BCM2835 documentation. They share
 * a common interrupt.
 *
 * "MU" stands for mini-uart.
 */

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

#endif /* AUX_H */
