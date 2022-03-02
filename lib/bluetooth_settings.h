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

// Define the PINX, DDRX, and PX# values for the RX pin
#define BT_RX_PIN  PINB
#define BT_RX_DDR  DDRB
#define BT_RX_BIT  PB3

// Define the PORTX, DDRX, and PX# values for the TX pin
#define BT_TX_PORT PORTB
#define BT_TX_DDR  DDRB
#define BT_TX_BIT  PB2

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

// Define the maximum number of attempts the library will make to connect/communicate
// with the Bluetooth module before it registers a failure/error
#define BT_MAXIMUM_ATTEMPTS 10

#endif // BLUETOOTH_SETTINGS_H
