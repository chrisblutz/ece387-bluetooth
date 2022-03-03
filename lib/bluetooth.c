/*
 * This file contains the implementations for all functions in
 * the Bluetooth library.
 */

#include <stdio.h>
#include <string.h>

#include "bluetooth_settings.h"
#include "bluetooth_internal.h"
#include "bluetooth.h"

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

uint8_t bt_setup() {
    return 0; // TODO
}

/*
 * -------------------------------------------------------------------
 * These functions DO NOT need to be run each time the module is used:
 * -------------------------------------------------------------------
 */

uint8_t bt_test() {
    // Send the AT command (expecting OK or OK+LOST in response)
    char responseBuffer[6]; // Should hold either "" (OK) or "+LOST" (OK+LOST) after next line
    size_t responseLength = bt_sendATQuery("AT", "OK", responseBuffer, 6);
    return responseLength == 0 || (responseLength > 0 && strcmp(responseBuffer, "+LOST") == 0);
}

// AT+ADC

size_t bt_getMACAddress(char* buffer, size_t bufferLength) {
    // Send the AT+ADDR? command (expecting OK+ADDR: to prefix the response)
    return bt_sendATQuery("AT+ADDR?", "OK+ADDR:", buffer, bufferLength);
}

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

size_t bt_getModuleName(char* buffer, size_t bufferLength) {
    // Send the AT+NAME? command (expecting OK+NAME: to prefix the response)
    return bt_sendATQuery("AT+NAME?", "OK+NAME:", buffer, bufferLength);
}

uint8_t bt_setModuleName(const char* name) {
    // Fix the name to the required length (12 + null-terminator)
    // All positions not occupied by the name will be filled
    // with the null character
    char fixedLengthName[13];
    size_t nameLength = strlen(name);
    if (nameLength <= 12) {
        // Copy the name and fill any remaining positions
        strcpy(fixedLengthName, name);
        while (nameLength < 12) {
            fixedLengthName[nameLength++] = '\0';
        }
    } else {
        // Truncate the name at 12 characters
        strncpy(fixedLengthName, name, 12);
    }
    fixedLengthName[12] = '\0';

    // Generate command and expected response
    char command[20];
    char response[20];
    sprintf(command, "AT+NAME%s", fixedLengthName);
    sprintf(response, "OK+Set:%s", fixedLengthName);

    return bt_sendATCommand(command, response);
}

// AT+PCTL

// AT+PARI

// AT+PIO1

// AT+PIO

size_t bt_getModulePIN(char* buffer, size_t bufferLength) {
    // Send the AT+PASS? command (expecting OK+Get: to prefix the response)
    return bt_sendATQuery("AT+PASS?", "OK+Get:", buffer, bufferLength);
}

uint8_t bt_setModulePIN(const char* pin) {
    // Fix the PIN to the required length (6 + null-terminator)
    char fixedLengthPin[7];
    size_t pinLength = strlen(pin);
    if (pinLength == 6) {
        strcpy(fixedLengthPin, pin);
    } else if (pinLength > 6) {
        // Truncate the PIN at 6 digits
        strncpy(fixedLengthPin, pin, 6);
    } else {
        strncpy(fixedLengthPin, pin, pinLength);
        // Append 0's to fill 6 digits
        while (pinLength < 6) {
            fixedLengthPin[pinLength++] = '0';
        }
    }
    fixedLengthPin[6] = '\0';

    // Generate command and expected response
    char command[14];
    char response[14];
    sprintf(command, "AT+PASS%s", fixedLengthPin);
    sprintf(response, "OK+Set:%s", fixedLengthPin);

    return bt_sendATCommand(command, response);
}

// AT+POWE

// AT+PWRM

// AT+RELI

uint8_t bt_resetFactoryDefaults() {
    // Send the AT+RENEW command (expecting OK+RENEW in response)
    return bt_sendATCommand("AT+RENEW", "OK+RENEW");
}

uint8_t bt_reset() {
    // Send the AT+RESET command (expecting OK+RESET in response)
    return bt_sendATCommand("AT+RESET", "OK+RESET");
}

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

uint8_t bt_sendATCommand(const char* command, const char* expectedResponse) {
    // If the module is connected to a remote device, return 0
    if (bt_connected())
        return 0;

    bt_writeString(command);

    // Wait for a response to become available, or the number of attempts is maxed out
    size_t attemptCount = 0;
    while (!bt_available() && attemptCount < BT_MAXIMUM_ATTEMPTS)
        attemptCount++;
    
    // If we've received data, process it.  If not, return 0
    if (bt_available()) {
        // Check the characters in the UART stream against the expected response
        char input;
        while (bt_available() && *expectedResponse) {
            input = (char) bt_read();
            if (input != *expectedResponse)
                break;
            expectedResponse++;
        }
        // Now that we've exhausted the expected response, make sure we don't have anything
        // else left in the stream.  If we do, read/dispose of it and return 0
        if (!bt_available() && !*expectedResponse) {
            return 1;
        } else {
            // Read the rest of the available bytes so the stream is ready
            // to process the next command/input
            while (bt_available())
                bt_read();
            return 0;
        }
    } else {
        return 0;
    }
}

size_t bt_sendATQuery(const char* command, const char* expectedResponsePrefix, char* responseBuffer, size_t responseBufferLength) {
    // If the module is connected to a remote device, return -1
    if (bt_connected())
        return -1;

    bt_writeString(command);

    // Wait for a response to become available, or the number of attempts is maxed out
    size_t attemptCount = 0;
    while (!bt_available() && attemptCount < BT_MAXIMUM_ATTEMPTS)
        attemptCount++;
    
    // If we've received data, process it.  If not, return 0
    if (bt_available()) {
        // Read the data into a buffer so we can manipulate it later
        char buffer[BT_AT_RESPONSE_BUFFER_LENGTH];
        size_t bufferIndex = 0;
        while (bt_available() && bufferIndex < BT_AT_RESPONSE_BUFFER_LENGTH)
            buffer[bufferIndex++] = (char) bt_read();

        // If we get enough data to overflow our buffer, read and dispose of the rest of the input
        // so the UART stream can process future commands/input
        while (bt_available())
            bt_read();
        
        // Check that the required prefix is present, and if not, return -1
        if (strprefix(buffer, expectedResponsePrefix)) {
            // Copy the remaining part of the response into the provided response buffer
            size_t prefixLength = strlen(expectedResponsePrefix);
            size_t responseLength = strlen(buffer) - prefixLength;
            size_t responseBufferEnd = min(responseLength, responseBufferLength - 1);
            strncpy(responseBuffer, buffer + strlen(expectedResponsePrefix), responseBufferEnd);
            responseBuffer[responseBufferEnd] = '\0';
            return responseBufferEnd;
        } else {
            return -1;
        }
    } else {
        responseBuffer[0] = '\0';
        return 0;
    }
}

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

ISR(BT_TIMER_INTERRUPT_VECTOR) {

}

uint8_t bt_connected() {
    return 0; // TODO
}

uint8_t bt_available() {
    return 0; // TODO
}

void bt_write(const uint8_t byte) {

}

uint8_t bt_read() {
    return 0; // TODO
}

/*
 *   ___    __ ___      _   _  _    _  _  _  _    _          
 *  |_ _|  / // _ \    | | | || |_ (_)| |(_)| |_ (_) ___  ___
 *   | |  / /| (_) |   | |_| ||  _|| || || ||  _|| |/ -_)(_-<
 *  |___|/_/  \___/     \___/  \__||_||_||_| \__||_|\___|/__/
 *                                                           
 *                        (I/O Utilities)
 */

void bt_writeString(const char* string) {
    // Write all bytes of the string (excluding the null-terminator)
    while (*string) {
        bt_write(*string++);
    }
}

size_t bt_readString(const char delimiter, char* buffer, size_t bufferLength) {
    // Read bytes to fill the given buffer
    size_t bufferIndex = 0;
    char input;
    while (bt_available() && bufferIndex < bufferLength - 1) {
        input = (char) bt_read();

        // If we've reached a delimiter, break
        if (input == delimiter)
            break;

        buffer[bufferIndex++] = input;
    }
    buffer[bufferIndex] = '\0';

    // If we've not reached the delimiter and there are still bytes available
    // (e.g. we overflowed the buffer), read the remaining bytes to clear the input
    // so the UART stream can handle further input
    while (bt_available() && (input = bt_read()) != delimiter);

    return strlen(buffer);
}

/*
 *   ___       _                          _     
 *  |_ _| _ _ | |_  ___  _ _  _ _   __ _ | | ___
 *   | | | ' \|  _|/ -_)| '_|| ' \ / _` || |(_-<
 *  |___||_||_|\__|\___||_|  |_||_|\__,_||_|/__/
 *                                              
 *                   (Internals)
 */

uint8_t strprefix(const char* str, const char* prefix) {
    // Check that the prefix matches all characters in the string
    // up to the end of the prefix
    while (*prefix) {
        if (!*str || *str != *prefix)
            return 0;
        str++;
        prefix++;
    }
    return 1;
}
