/*
 * This file contains the headers/prototypes for functions,
 * constants, and macros used by the public Bluetooth library API.
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdlib.h>
#include <stdint.h>

#include "bluetooth_settings.h"

/*
 *    ___                 _               _                          _     __  __                         
 *   / __| ___  _ _   ___| |_  __ _  _ _ | |_  ___    __ _  _ _   __| |   |  \/  | __ _  __  _ _  ___  ___
 *  | (__ / _ \| ' \ (_-<|  _|/ _` || ' \|  _|(_-<   / _` || ' \ / _` |   | |\/| |/ _` |/ _|| '_|/ _ \(_-<
 *   \___|\___/|_||_|/__/ \__|\__,_||_||_|\__|/__/   \__,_||_||_|\__,_|   |_|  |_|\__,_|\__||_|  \___//__/
 *                                                                                                        
 *                                      (Constants and Macros)
 */

/*
 * ------------------------------------------------------------------------
 * These constants are used by the bt_[get/set]AuthenticationType function:
 * ------------------------------------------------------------------------
 */

// No PIN code is set up (and none can be set)
#define BT_AUTH_TYPE_NONE                   0
// An encrypted link is required (no man-in-the-middle protection)
#define BT_AUTH_TYPE_ENCRYPTED_LINK         1
// An encrypted link is required (with man-in-the-middle protection)
#define BT_AUTH_TYPE_MITM_PROTECTED_LINK    2
// An secure, encrypted link is required (with man-in-the-middle protection)
#define BT_AUTH_TYPE_SECURE_CONNECTION_LINK 3

/*
 * ----------------------------------------------------------------
 * These constants/macros are for the UART stream and connectivity:
 * ----------------------------------------------------------------
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
 * As the UART stream requires interrupts, this function
 * enables them using sei().
 * 
 * Also, this function blocks until the initial connection
 * status of the module can be determined (so it is not
 * marked as disconnected incorrectly initially), so it may
 * take up to a second to exit.  Calls to bt_connected()
 * will return the correct value after this function exits.
 *
 * @returns 1 if the setup was completed successfully, 0 otherwise
 */
uint8_t bt_setup();

// Allow for the configuration function toggle
#if BT_ENABLE_CONFIGURATION_FUNCTIONS

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
     * @returns 1 if the module responds positively, 0 otherwise 
     */
    uint8_t bt_test();

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
     * @returns the length of the address returned, excluding the null-terminator
     */
    size_t bt_getMACAddress(char* buffer, size_t bufferLength);

    // TODO - AT+BAUD (?)

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
     * @returns the length of the name returned, excluding the null-terminator
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
     * Note: this function will not work if the device is not set
     * up to require some form of authentication.
     * 
     * @param buffer the pre-allocated character buffer where the null-terminated PIN will be stored
     * @param bufferLength the length of the pre-allocated buffer provided to this function
     * @returns the length of the PIN returned, excluding the null-terminator
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

    /**
     * This function retrieves the type of authentication
     * used by the Bluetooth module.
     *
     * It uses the "AT+TYPE?" command to request the name.
     *
     * The possible results from this function are (least
     * to most secure):
     *  - BT_AUTH_TYPE_NONE                   (0)
     *  - BT_AUTH_TYPE_ENCRYPTED_LINK         (1)
     *  - BT_AUTH_TYPE_MITM_PROTECTED_LINK    (2)
     *  - BT_AUTH_TYPE_SECURE_CONNECTION_LINK (3)
     *
     * The first two modes may not be supported by modern
     * smartphones, so if you are unable to pair with your
     * Bluetooth module, try one of the more secure options.
     *
     * @param type the pointer to the location where the type will be stored
     * @returns 1 if the command ran successfully, 0 otherwise
     */
    uint8_t bt_getAuthenticationType(uint8_t* type);

    /**
     * This function sets the type of authentication used
     * by the Bluetooth module.
     *
     * It uses the "AT+TYPE" command to set the name.
     *
     * The possible types are (least to most secure):
     *  - BT_AUTH_TYPE_NONE                   (0)
     *  - BT_AUTH_TYPE_ENCRYPTED_LINK         (1)
     *  - BT_AUTH_TYPE_MITM_PROTECTED_LINK    (2)
     *  - BT_AUTH_TYPE_SECURE_CONNECTION_LINK (3)
     *
     * The first two modes may not be supported by modern
     * smartphones, so if you are unable to pair with your
     * Bluetooth module, try one of the more secure options.
     *
     * @param type the authentication mode for the module
     * @returns 1 if the command ran successfully, 0 otherwise
     */
    uint8_t bt_setAuthenticationType(uint8_t type);

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
     * @returns the length of the response returned, excluding the null-terminator
     */
    size_t bt_sendATQuery(const char* command, const char* expectedResponsePrefix, char* responseBuffer, size_t responseBufferLength);

#endif

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
 * connected to a remote device.  It uses the state provided
 * by the Bluetooth module , so it does not require any
 * UART transmissions to determine connectivity.
 *
 * However, due to the way that the state pin codifies connected
 * vs. disconnected (always high for connected, alternating high/
 * low every 0.5sec for disconnected) this function may not update
 * its return value until about a second after the state has
 * actually changed.
 *
 * @return uint8_t 1 if a remote device is connected, 0 otherwise
 */
uint8_t bt_connected();

/**
 * This function checks to see if a byte of data is available
 * to be read from the Bluetooth module's UART stream.
 * 
 * This function is non-blocking, so even if a byte is currently
 * being read by the UART receiver, this function will not indicate
 * that a new byte is ready until that byte is completely received.
 * 
 * If blocking functionality is required (e.g. concurrent bytes
 * need to be read at the same time), use bt_awaitAvailable().
 * 
 * @returns 1 if a byte is available, 0 if no byte is available
 */
uint8_t bt_available();

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
 * However, this functionality could also lead to multiple consecutive
 * transmissions being read together, as this function waits for an extra
 * UART tick to verify that no further data is being sent.
 * 
 * For non-blocking functionality (which does not wait for the
 * current byte to be received completely), use bt_available().
 * 
 * @returns 1 if a byte is available, 0 if no byte is available
 */
uint8_t bt_awaitAvailable();

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

/**
 * This function resets the input buffers and removes any
 * unprocessed input.
 */
void bt_flush();

/*
 *   ___    __ ___      _   _  _    _  _  _  _    _          
 *  |_ _|  / // _ \    | | | || |_ (_)| |(_)| |_ (_) ___  ___
 *   | |  / /| (_) |   | |_| ||  _|| || || ||  _|| |/ -_)(_-<
 *  |___|/_/  \___/     \___/  \__||_||_||_| \__||_|\___|/__/
 *                                                           
 *                        (I/O Utilities)
 */

// Allow for the complex object read/write function toggle
#if BT_ENABLE_COMPLEX_OBJECT_RX_TX_FUNCTIONS

    /*
    * ----------------------------------------------------------
    * Utility functions for sending strings via the UART stream:
    * ----------------------------------------------------------
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
     * @returns the length of the string read from the stream, excluding the null-terminator
     */
    size_t bt_readString(const char delimiter, char* buffer, size_t bufferLength);

    /*
    * -----------------------------------------------------------
    * Utility functions for sending integers via the UART stream:
    * -----------------------------------------------------------
    */

    /**
     * This function writes a signed 32-bit integer value to the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes should be sent first.
     * 
     * @param value the value to send
     */
    void bt_writeInt32(int32_t value);

    /**
     * This function reads a signed 32-bit integer value from the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes are read first.
     * 
     * If an error occurs while reading, 0 is returned.
     * 
     * @returns the value read
     */
    int32_t bt_readInt32();

    /**
     * This function writes a unsigned 32-bit integer value to the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes should be sent first.
     * 
     * @param value the value to send
     */
    void bt_writeUInt32(uint32_t value);

    /**
     * This function reads a unsigned 32-bit integer value from the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes are read first.
     * 
     * If an error occurs while reading, 0 is returned.
     * 
     * @returns the value read
     */
    uint32_t bt_readUInt32();

    /**
     * This function writes a signed 16-bit integer value to the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes should be sent first.
     * 
     * @param value the value to send
     */
    void bt_writeInt16(int16_t value);

    /**
     * This function reads a signed 16-bit integer value from the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes are read first.
     * 
     * If an error occurs while reading, 0 is returned.
     * 
     * @returns the value read
     */
    int16_t bt_readInt16();

    /**
     * This function writes a unsigned 16-bit integer value to the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes should be sent first.
     * 
     * @param value the value to send
     */
    void bt_writeUInt16(uint16_t value);

    /**
     * This function reads a unsigned 16-bit integer value from the UART stream.
     * 
     * It uses the endianness defined by BT_UART_ENDIANNESS to
     * determine which bytes are read first.
     * 
     * If an error occurs while reading, 0 is returned.
     * 
     * @returns the value read
     */
    uint16_t bt_readUInt16();

#endif

#endif // BLUETOOTH_H
