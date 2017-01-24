/*
 * UART1 (the "mini UART"), SPI1, and SPI2 are grouped together as the
 * "Auxillary" peripherals in the BCM2835 documentation. They share
 * a common interrupt.
 *
 * "MU" stands for mini-uart.
 */
#ifndef AUX_H
#define AUX_H

#include "types.h"

/**
 * Initialize the mini-uart.
 */
s32 aux_mu_init();

s32 aux_mu_tx_byte_busywait(u8 byte);

u8 aux_mu_rx_byte_busywait(void);

#endif /* AUX_H */
