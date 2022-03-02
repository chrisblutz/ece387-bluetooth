#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>

/*
 *    ___              __  _                         _    _            
 *   / __| ___  _ _   / _|(_) __ _  _  _  _ _  __ _ | |_ (_) ___  _ _  
 *  | (__ / _ \| ' \ |  _|| |/ _` || || || '_|/ _` ||  _|| |/ _ \| ' \ 
 *   \___|\___/|_||_||_|  |_|\__, | \_,_||_|  \__,_| \__||_|\___/|_||_|
 *                           |___/                                     
 * 
 *                          (Configuration)
 */

uint8_t bt_setup();

/*
 *   _   _   _    ___  _____                    _     ___    __ ___  
 *  | | | | /_\  | _ \|_   _|    __ _  _ _   __| |   |_ _|  / // _ \ 
 *  | |_| |/ _ \ |   /  | |     / _` || ' \ / _` |    | |  / /| (_) |
 *   \___//_/ \_\|_|_\  |_|     \__,_||_||_|\__,_|   |___|/_/  \___/ 
 *                                                                   
 *                          (UART and I/O)
 */

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
 * Calling this function when there is no data available on the 
 * UART stream (i.e. when bt_available() returns 0), this function
 * will return the previous byte of data again.
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
 * Calling this function when there is no data available on the UART stream
 * will result in a string of length 0 (the empty string, "").
 * 
 * @param delimiter the character that ends a string from the UART stream (e.g. "\n", "\0", etc.)
 * @param buffer the pre-allocated character buffer where the null-terminated string will be stored
 * @param bufferLength the length of the pre-allocated buffer provided to this function
 * @returns the number of bytes read from the stream (e.g. length of the string returned, including the null-terminator)
 */
size_t bt_readString(const char delimiter, char* buffer, size_t bufferLength);

#endif // BLUETOOTH_H
