/*
 * This file contains the customizable constants, and macros for the Bluetooth library.
 */

#ifndef BLUETOOTH_SETTINGS_H
#define BLUETOOTH_SETTINGS_H

/*
 *   ___  _                              _     ___            _       
 *  | _ \(_) _ _   ___    __ _  _ _   __| |   | _ \ ___  _ _ | |_  ___
 *  |  _/| || ' \ (_-<   / _` || ' \ / _` |   |  _// _ \| '_||  _|(_-<
 *  |_|  |_||_||_|/__/   \__,_||_||_|\__,_|   |_|  \___/|_|   \__|/__/
 *                                                                    
 *                         (Pins and Ports)
 */

// Define the general board information
#define F_CPU 16000000

// Define the PINX, DDRX, and PX# values for the RX pin
#define BT_RX_PIN  PIND
#define BT_RX_DDR  DDRD
#define BT_RX_BIT  PD3

// Define the PORTX, DDRX, and PX# values for the TX pin
#define BT_TX_PORT PORTD
#define BT_TX_DDR  DDRD
#define BT_TX_BIT  PD2

// Define the timer information used for the UART stream
// * These defaults are for the 8-bit TIMER0, with a prescalar of 8
// * BT_TIMER_MAXIMUM_VALUE should be set to (2^[bit width])-1
#define BT_TIMER_MAXIMUM_VALUE           (255)
#define BT_TIMER_INTERRUPT_VECTOR        TIMER0_COMPA_vect
#define BT_TIMER_COMPARE_REGISTER        OCR0A
#define BT_TIMER_CONTROL_REGISTER_A      TCCR0A
#define BT_TIMER_CONTROL_REGISTER_B      TCCR0B
#define BT_TIMER_COUNTER_REGISTER        TCNT0
#define BT_TIMER_INTERRUPT_MASK_REGISTER TIMSK0
#define BT_TIMER_INTERRUPT_ENABLE_MASK   (1 << OCIE0A)
#define BT_TIMER_CONTROL_REGISTER_A_MASK (1 << WGM01)
#define BT_TIMER_CONTROL_REGISTER_B_MASK (0)
#define BT_TIMER_PRESCALE_VALUE          (8)
#define BT_TIMER_PRESCALER_REG_A_MASK    (0)
#define BT_TIMER_PRESCALER_REG_B_MASK    (1 << CS01)

/*
 *    ___              __  _                         _    _            
 *   / __| ___  _ _   / _|(_) __ _  _  _  _ _  __ _ | |_ (_) ___  _ _  
 *  | (__ / _ \| ' \ |  _|| |/ _` || || || '_|/ _` ||  _|| |/ _ \| ' \ 
 *   \___|\___/|_||_||_|  |_|\__, | \_,_||_|  \__,_| \__||_|\___/|_||_|
 *                           |___/                                     
 * 
 *                          (Configuration)
 */

// Define the baud rate to be used by the UART stream to the Bluetooth module
#define BT_BAUD_RATE 9600

// Define whether the connection and disconnection handlers should be enabled
// If BT_ENABLE_CONNECTION_HANDLER is enabled, BT_ON_CONNECTION { /* ... */ } must be defined
// If BT_ENABLE_DISCONNECTION_HANDLER is enabled, BT_ON_DISCONNECTION { /* ... */ } must be defined
#define BT_ENABLE_CONNECTION_HANDLER    1
#define BT_ENABLE_DISCONNECTION_HANDLER 1

// Define the maximum number of attempts the library will make to connect/communicate
// with the Bluetooth module before it registers a failure/error
#define BT_MAXIMUM_ATTEMPTS 10

#endif // BLUETOOTH_SETTINGS_H
