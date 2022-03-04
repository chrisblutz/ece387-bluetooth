/*
 * This file contains the headers/prototypes for internal functions,
 * constants, and macros used by the Bluetooth library.
 */

#ifndef BLUETOOTH_INTERNAL_H
#define BLUETOOTH_INTERNAL_H

#include <stdint.h>

#include "bluetooth_settings.h"
#include "bluetooth.h"

/*
 *    ___                 _               _                          _     __  __                         
 *   / __| ___  _ _   ___| |_  __ _  _ _ | |_  ___    __ _  _ _   __| |   |  \/  | __ _  __  _ _  ___  ___
 *  | (__ / _ \| ' \ (_-<|  _|/ _` || ' \|  _|(_-<   / _` || ' \ / _` |   | |\/| |/ _` |/ _|| '_|/ _ \(_-<
 *   \___|\___/|_||_|/__/ \__|\__,_||_||_|\__|/__/   \__,_||_||_|\__,_|   |_|  |_|\__,_|\__||_|  \___//__/
 *                                                                                                        
 *                                      (Constants and Macros)
 */

// Define the length of the AT command response buffer
#define BT_AT_RESPONSE_BUFFER_LENGTH 50

// Define the names of connection/disconnection handler functions
#define BT_CONNECTION_HANDLER    bt_handler_onConnection
#define BT_DISCONNECTION_HANDLER bt_handler_onDisconnection

// Declare prototypes for connection/disconnection handlers if they're enabled
#if BT_ENABLE_CONNECTION_HANDLER
    BT_ON_CONNECTION;
#endif
#if BT_ENABLE_DISCONNECTION_HANDLER
    BT_ON_DISCONNECTION;
#endif

// Define a macro to determine the minimum of two values
#define min(X,Y) (((X) < (Y)) ? (X) : (Y))

/*
 *   _   _   _    ___  _____                    _     ___    __ ___  
 *  | | | | /_\  | _ \|_   _|    __ _  _ _   __| |   |_ _|  / // _ \ 
 *  | |_| |/ _ \ |   /  | |     / _` || ' \ / _` |    | |  / /| (_) |
 *   \___//_/ \_\|_|_\  |_|     \__,_||_||_|\__,_|   |___|/_/  \___/ 
 *                                                                   
 *                          (UART and I/O)
 *
 *        Software UART implementation is based on/modified from
 *                https://github.com/blalor/avr-softuart
 */

// Define timer settings (extrapolate from user-provided values)
// Timer must tick at 3x baud rate
#define BT_TIMER_TOP ((F_CPU / BT_TIMER_PRESCALE_VALUE / BT_BAUD_RATE / 3) - 1)
// Double-check that the max timer value fits in the timer's bit width
#if (BT_TIMER_TOP > BT_TIMER_MAXIMUM_VALUE)
    #warning "Timer interval required for baud rate exceeds maximum possible value.  Use a wider timer."
#endif

// Define bit widths of UART input/output
#define BT_UART_TX_BITS 10
#define BT_UART_RX_BITS 8

// Define size of the UART receiver buffer
#define BT_UART_RX_BUFFER_LENGTH 64

// Define the number of ticks required for bt_awaitAvailable() to
// wait the number of milliseconds specified by BT_UART_PACKET_WAIT_MS
#define BT_UART_PACKET_WAIT_TICKS (((F_CPU / BT_TIMER_PRESCALE_VALUE / BT_TIMER_TOP) * BT_UART_PACKET_WAIT_MS) / 1000)

// Define macros to turn on/off the UART TX pin, and to get the UART RX pin state
#define bt_uartSetTxLow()  (BT_TX_PORT &= ~(1 << BT_TX_BIT))
#define bt_uartSetTxHigh() (BT_TX_PORT |= (1 << BT_TX_BIT))
#define bt_uartGetRx()     (BT_RX_PIN & (1 << BT_RX_BIT))

/**
 * This function initializes the pins required for the software UART stream.
 */
void bt_initializeUARTPins();

/**
 * This function sets up the timer and interrupt required for the software
 * UART stream. 
 */
void bt_initializeUARTTimer();

/**
 * This function initializes the software UART stream and sets up the pins
 * and interrupts required.
 */
void bt_initializeUART();

#endif // BLUETOOTH_INTERNAL_H
