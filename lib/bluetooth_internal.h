/*
 * This file contains the headers/prototypes for internal functions,
 * constants, and macros used by the Bluetooth library.
 */

#ifndef BLUETOOTH_INTERNAL_H
#define BLUETOOTH_INTERNAL_H

#include <stdint.h>

// Define the length of the AT command response buffer
#define BT_AT_RESPONSE_BUFFER_LENGTH 50

/**
 * This function checks if a string has a specific prefix
 * (e.g. whether the first characters in a string match
 * the characters in another string).
 * 
 * @param str the complete string to check
 * @param prefix the prefix to check for
 * @returns 1 if the string has the provided prefix, 0 otherwise
 */
uint8_t strprefix(const char* str, const char* prefix);

#endif // BLUETOOTH_INTERNAL_H
