/*
 * This file contains the headers/prototypes for functions,
 * constants, and macros used by the public Bluetooth library API.
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdlib.h>
#include <stdint.h>

#include <avr/interrupt.h>

/*
 *    ___                 _               _                          _     __  __                         
 *   / __| ___  _ _   ___| |_  __ _  _ _ | |_  ___    __ _  _ _   __| |   |  \/  | __ _  __  _ _  ___  ___
 *  | (__ / _ \| ' \ (_-<|  _|/ _` || ' \|  _|(_-<   / _` || ' \ / _` |   | |\/| |/ _` |/ _|| '_|/ _ \(_-<
 *   \___|\___/|_||_|/__/ \__|\__,_||_||_|\__|/__/   \__,_||_||_|\__,_|   |_|  |_|\__,_|\__||_|  \___//__/
 *                                                                                                        
 *                                      (Constants and Macros)
 */

// Define the connection/disconnection handler prototypes
// These can be used as follows:
//   BT_ON_CONNECTION { /* ... */ }
// The handlers are called from within the UART interrupt
// service routine, so they should be designed accordingly.
//
// In order to use these handlers, they must be enabled
// by setting BT_ENABLE_CONNECTION_HANDLER or
// BT_ENABLE_DISCONNECTION_HANDLER to 1 in the
// bluetooth_settings.h file.
#define BT_ON_CONNECTION    void bt_handler_onConnection()
#define BT_ON_DISCONNECTION void bt_handler_onDisconnection()

/*
 *    ___              __  _                         _    _            
 *   / __| ___  _ _   / _|(_) __ _  _  _  _ _  __ _ | |_ (_) ___  _ _  
 *  | (__ / _ \| ' \ |  _|| |/ _` || || || '_|/ _` ||  _|| |/ _ \| ' \ 
 *   \___|\___/|_||_||_|  |_|\__, | \_,_||_|  \__,_| \__||_|\___/|_||_|
 *                           |___/                                     
 * 
 *                          (Configuration)
 */

/*
 * ---------------------------------------------------------------
 * These functions DO need to be run each time the module is used:
 * ---------------------------------------------------------------
 */

/**
 * This function initializes the pins for the Bluetooth module
 * as well as the necessary timers for the UART stream.
 *
 * Unlike other configuration functions, this one must be run
 * at the beginning of every program that uses the Bluetooth
 * module.
 *
 * @returns 1 if the setup was completed successfully, 0 otherwise
 */
uint8_t bt_setup();

/*
 * -------------------------------------------------------------------
 * These functions DO NOT need to be run each time the module is used:
 * -------------------------------------------------------------------
 */

/**
 * This function pings the Bluetooth module to make sure that
 * all of the wiring is working correctly.
 * 
 * It uses the "AT" command to ping the module, and expects a
 * response of "OK".
 * 
 * Note: Calling this function while a device is connected to
 * the module will force that device to disconnect, and the "AT"
 * command will return "OK+LOST" instead.
 * 
 * @returns 1 if the module responds positively, 0 otherwise 
 */
uint8_t bt_test();

// AT+ADC

/**
 * This function retrieves the MAC address for the
 * Bluetooth module.
 * 
 * If the response overflows the provided buffer, the response
 * will be truncated to fill the buffer, and the buffer will end
 * with a null-terminator regardless of if the response overflowed
 * it or not.
 * 
 * It uses the "AT+ADDR?" command to request the
 * address.
 * 
 * @param buffer the pre-allocated character buffer where the null-terminated address will be stored
 * @param bufferLength the length of the pre-allocated buffer provided to this function
 * @returns the length of the address returned, excluding the null-terminator, or -1 if an error occurred
 */
size_t bt_getMACAddress(char* buffer, size_t bufferLength);

// AT+ADVI

// AT+ADTY

// AT+ANCS

// AT+ALLO

// AT+AD

// AT+BEFC

// AT+AFTC

// AT+BATC

// AT+BATT

// AT+BIT7

// AT+BAUD

// AT+COMI

// AT+COMA

// AT+COLA

// AT+COSU

// AT+COUP

// AT+CHAR

// AT+CLEAR

// AT+CONNL

// AT+CO

// AT+COL

// AT+CYC

// AT+COMP

// AT+DISC

// AT+DISI

// AT+CONN

// AT+DELO

// AT+ERASE

// AT+FLAG

// AT+FILT

// AT_FIOW [FLOW?]

// AT+GAIN

// AT+HUMI

// AT+IMME

// AT+IBEA

// AT+IBE0

// AT+IBE1

// AT+IBE2

// AT+IBE3

// AT+MARJ

// AT+MINO

// AT+MEAS

// AT+MODE

// AT+NOTI

// AT+NOTP

/**
 * This function retrieves the name of the Bluetooth module.
 * 
 * If the response overflows the provided buffer, the response
 * will be truncated to fill the buffer, and the buffer will end
 * with a null-terminator regardless of if the response overflowed
 * it or not.
 * 
 * It uses the "AT+NAME?" command to request the name.
 * 
 * @param buffer the pre-allocated character buffer where the null-terminated name will be stored
 * @param bufferLength the length of the pre-allocated buffer provided to this function
 * @returns the length of the name returned, excluding the null-terminator, or -1 if an error occurred
 */
size_t bt_getModuleName(char* buffer, size_t bufferLength);

/**
 * This function sets the name of the Bluetooth module.
 * The maximum length for a name is 12 characters.
 * If a name is provided that is more than 12 characters,
 * only the first 12 characters will be used.
 * 
 * It uses the "AT+NAME" command to set the name.
 * 
 * @param name the name for the module (max 12 characters)
 * @returns 1 if the command ran successfully, 0 otherwise
 */
uint8_t bt_setModuleName(const char* name);

// AT+PCTL

// AT+PARI

// AT+PIO1

// AT+PIO

/**
 * This function retrieves the PIN code for the Bluetooth module.
 * This will be used when connecting from a remote device.
 * 
 * If the response overflows the provided buffer, the response
 * will be truncated to fill the buffer, and the buffer will end
 * with a null-terminator regardless of if the response overflowed
 * it or not.
 * 
 * It uses the "AT+PASS?" command to request the name.
 * 
 * @param buffer the pre-allocated character buffer where the null-terminated PIN will be stored
 * @param bufferLength the length of the pre-allocated buffer provided to this function
 * @returns the length of the PIN returned, excluding the null-terminator, or -1 if an error occurred
 */
size_t bt_getModulePIN(char* buffer, size_t bufferLength);

/**
 * This function sets the PIN code for the Bluetooth module.
 * This will be used when connecting from a remote device, and
 * it must be 6 numeric digits.  If a PIN is provided that contains
 * more than 6 digits, only the first 6 digits will be used.
 * Conversely, if a PIN is provided that contains less than 6 digits,
 * zeros will be appended to the end of the PIN to fill the remaining
 * positions.
 * 
 * It uses the "AT+PASS" command to set the name.
 * 
 * @param name the PIN for the module (6 numeric digits)
 * @returns 1 if the command ran successfully, 0 otherwise
 */
uint8_t bt_setModulePIN(const char* pin);

// AT+POWE

// AT+PWRM

// AT+RELI

/**
 * This function resets all Bluetooth module configurations
 * to their factory-default values.  To just perform a restart
 * of the module, use bt_reset().
 * 
 * It uses the "AT+RENEW" command to reset the configuration.
 * 
 * @returns 1 if the command ran successfully, 0 otherwise
 */
uint8_t bt_resetFactoryDefaults();

/**
 * This function restarts the Bluetooth module.  To reset
 * configurations to their factory-default settings, use
 * bt_resetFactoryDefaults().
 * 
 * It uses the "AT+RESET" command to restart the module.
 * 
 * @returns 1 if the command ran successfully, 0 otherwise
 */
uint8_t bt_reset();

// AT+ROLE

// AT+RSSI

// AT+RADD

// AT+RAT

// AT+STOP

// AT+START

// AT+SLEEP

// AT+SAVE

// AT+SCAN

// AT+SENS

// AT+SHOW

// AT+TEHU

// AT+TEMP

// AT+TCON

// AT+TYPE

// AT+UUID

// AT+UART

// AT+VERS

/*
 * -----------------------------------------------------------------------------
 * These functions are utility functions for use within configuration functions:
 * -----------------------------------------------------------------------------
 */

/**
 * This function sends a configuration command (AT command) to the Bluetooth
 * module, and waits until it receives a response (or times out).
 * 
 * This function only returns whether the command completed successfully, so
 * if information is needed in response to the command (like a return value),
 * use bt_sendATQuery().
 * 
 * @param command the command to send to the module
 * @param expectedResponse the response expected to the command (e.g. "OK")
 * @returns 1 if the command completed successfully, 0 otherwise
 */
uint8_t bt_sendATCommand(const char* command, const char* expectedResponse);

/**
 * This function queries data from the Bluetooth module using configuration
 * commands (AT commands), and waits until it receives a response (or times out).
 * 
 * This function expects additional information to be returned with the query
 * response, so if only the success/failure information is required, use
 * bt_sendATCommand().
 * 
 * If the response overflows the provided buffer, the response
 * will be truncated to fill the buffer, and the buffer will end
 * with a null-terminator regardless of if the response overflowed
 * it or not.
 * 
 * @param command the command to send to the module
 * @param expectedResponsePrefix the prefix expected in the response to the command (e.g. "OK+Get:")
 * @param responseBuffer the pre-allocated character buffer where the null-terminated response will be stored
 * @param responseBufferLength the length of the pre-allocated buffer provided to this function
 * @returns the length of the response returned, excluding the null-terminator, or -1 if an error occurred
 */
size_t bt_sendATQuery(const char* command, const char* expectedResponsePrefix, char* responseBuffer, size_t responseBufferLength);

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

/**
 * This function determines if the Bluetooth module is currently
 * connected to a remote device.  It uses an internal flag set by
 * the internal software UART logic, so it does not require any
 * UART transmissions to determine connectivity.
 *
 * @return uint8_t 1 if a remote device is connected, 0 otherwise
 */
uint8_t bt_connected();

/**
 * This function checks to see if a byte of data is available
 * to be read from the Bluetooth module's UART stream.
 * 
 * This function is blocking, as it waits for the entire byte of
 * data to be read off of the UART stream before returning, if
 * there is a byte currently being read.  If there is no byte
 * currently being read, this function exits immediately.
 * 
 * This functionality allows multiple consecutive bytes to be
 * read as a stream of bytes (using a while loop on bt_available())
 * of unknown length, instead of relying on a known length of bytes.
 * 
 * @returns 1 if a byte is available, 0 if no byte is available
 */
uint8_t bt_available();

/**
 * This function writes a byte of data to the Bluetooth module's
 * UART stream.
 * 
 * @param byte the byte of data to write
 */
void bt_write(const uint8_t byte);

/**
 * This function reads a byte of data from the Bluetooth module's
 * UART stream.
 * 
 * This function does not block, and will return 0 if it is called
 * and no new data is available.  To ensure there is data available
 * to be read, verify bt_available() first.
 * 
 * @returns the byte of data read
 */
uint8_t bt_read();

/*
 *   ___    __ ___      _   _  _    _  _  _  _    _          
 *  |_ _|  / // _ \    | | | || |_ (_)| |(_)| |_ (_) ___  ___
 *   | |  / /| (_) |   | |_| ||  _|| || || ||  _|| |/ -_)(_-<
 *  |___|/_/  \___/     \___/  \__||_||_||_| \__||_|\___|/__/
 *                                                           
 *                        (I/O Utilities)
 */

/**
 * This function writes a string of bytes to the Bluetooth module's
 * UART stream.  It will not write the null-terminator, so if the
 * remote device requires the null-terminator to be sent, use
 * bt_write(0) after calling this function.
 * 
 * @param string the string to write to the stream
 */
void bt_writeString(const char* string);

/**
 * This function reads a string of bytes from the Bluetooth module's
 * UART stream.  The delimiter provided is used to determine when a string
 * ends.  This function will append a null-terminator to the string when read.
 * 
 * If the strings being sent by the remote device do not have a consistent
 * delimiter, this function will not work.  Instead, use a while loop to check
 * if bytes are available, and then read them using bt_read().
 * 
 * This function does not block, so if there is no new data to read, it
 * will result in a string of length 0 (the empty string, "").  To ensure new
 * data is available when this function is called, verify bt_available() first.
 * 
 * If the response overflows the provided buffer, the response
 * will be truncated to fill the buffer, and the buffer will end
 * with a null-terminator regardless of if the response overflowed
 * it or not.
 * 
 * @param delimiter the character that ends a string from the UART stream (e.g. "\n", "\0", etc.)
 * @param buffer the pre-allocated character buffer where the null-terminated string will be stored
 * @param bufferLength the length of the pre-allocated buffer provided to this function
 * @returns the length of the string read from the stream, excluding the null-terminator, or -1 if an error occurred
 */
size_t bt_readString(const char delimiter, char* buffer, size_t bufferLength);

#endif // BLUETOOTH_H
