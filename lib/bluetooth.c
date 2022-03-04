/*
 * This file contains the implementations for all functions in
 * the Bluetooth library.
 */

#include <stdio.h>
#include <string.h>

#include <avr/interrupt.h>

#include "bluetooth_settings.h"
#include "bluetooth_internal.h"
#include "bluetooth.h"

// TODO - remove when done debugging
#include "USART.h"

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

// This global tracks whether we've completed enough connection polls to determine
// if we're connected to a remote device (start at 4, when we hit 0 we can finish setup)
volatile uint8_t uartInitialConnectionCheckCountdown = 4;
// This global gets incremented every millisecond to allow for timeouts (set to 0 then count until desired)
volatile uint32_t uartMillisecondCounter = 0;

uint8_t bt_setup() {
    // Initialize the software UART stream
    bt_initializeUART();

    // Enable interrupts so that we can handle UART data
    sei();

    // Wait until the initial connection status of the module has been determined
    while (uartInitialConnectionCheckCountdown);

    return 1;
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
uint8_t bt_getAuthenticationType(uint8_t* type) {
    // Send the AT+PASS? command (expecting OK+Get: to prefix the response)
    char buffer[2]; // 1 digit + '\0'
    uint8_t success = bt_sendATQuery("AT+TYPE?", "OK+Get:", buffer, 2);

    // If the command succeeded, parse it to the appropriate integer value
    if (success) {
        // Parse character to integer value and return
        switch (buffer[0]) {
            case '0':
                *type = BT_AUTH_TYPE_NONE;
                break;
            case '1':
                *type = BT_AUTH_TYPE_ENCRYPTED_LINK;
                break;
            case '2':
                *type = BT_AUTH_TYPE_MITM_PROTECTED_LINK;
                break;
            case '3':
                *type = BT_AUTH_TYPE_SECURE_CONNECTION_LINK;
                break;
            default:
                // Since we didn't recognize the result, return 0
                return 0;
        }
        return 1;
    } else {
        return 0;
    }
}

uint8_t bt_setAuthenticationType(uint8_t type) {
    // Check that the type is within bounds
    if (type > 3)
        return 0;

    // Generate command and expected response
    char command[9];
    char response[9];
    sprintf(command, "AT+TYPE%d", type);
    sprintf(response, "OK+Set:%d", type);

    return bt_sendATCommand(command, response);
}

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

    // Wait for a response to become available, or the timeout is exceeded
    uartMillisecondCounter = 0;
    while (!bt_available() && uartMillisecondCounter < BT_TIMEOUT_MS);
    
    // If we've received data, process it.  If not, return 0
    if (bt_available()) {
        // Check the characters in the UART stream against the expected response
        char input;
        while (bt_awaitAvailable() && *expectedResponse) {
            input = (char) bt_read();
            if (input != *expectedResponse)
                break;
            expectedResponse++;
        }

        // Now that we've exhausted the expected response, make sure we don't have anything
        // else left in the stream.  If we do, read/dispose of it and return 0
        if (!bt_available() && !*expectedResponse) {
            // Since the command completed successfully, wait for the defined time
            // before returning for the change to take effect
            uartMillisecondCounter = 0;
            while (uartMillisecondCounter < BT_AT_SET_WAIT_TIME_MS);
            // Now return after the wait period
            return 1;
        } else {
            // Read the rest of the available bytes so the stream is ready
            // to process the next command/input
            while (bt_awaitAvailable())
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
        return 0;

    bt_writeString(command);

    // Wait for a response to become available, or the timeout is exceeded
    uartMillisecondCounter = 0;
    while (!bt_available() && uartMillisecondCounter < BT_TIMEOUT_MS);
    
    // If we've received data, process it.  If not, return 0
    if (bt_available()) {
        // Read the data into a buffer so we can manipulate it later
        char buffer[BT_AT_RESPONSE_BUFFER_LENGTH];
        size_t bufferIndex = 0;
        while (bt_awaitAvailable() && bufferIndex < BT_AT_RESPONSE_BUFFER_LENGTH)
            buffer[bufferIndex++] = (char) bt_read();
        buffer[bufferIndex] = '\0';

        // If we get enough data to overflow our buffer, read and dispose of the rest of the input
        // so the UART stream can process future commands/input
        if (bt_available())
            while (bt_awaitAvailable())
                bt_read();
        
        size_t prefixLength = strlen(expectedResponsePrefix);
        // Check that the required prefix is present, and if not, return -1
        if (strncmp(buffer, expectedResponsePrefix, prefixLength) == 0) {
            // Copy the remaining part of the response into the provided response buffer
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

/*
 * -----------------------------------------------------------------------------------
 * Globals and interrupt service routine (ISR) for software UART transmitter/receiver:
 * -----------------------------------------------------------------------------------
 */

// Input data will be stored to/read from this buffer
volatile static uint8_t  uartInputBuffer[BT_UART_RX_BUFFER_LENGTH];
// The index of the "write" head of the buffer
volatile static uint8_t  uartBufferInputIndex;
// The index of the "read" head of the buffer
static uint8_t           uartBufferReadIndex;
// 1 if we're receiving data, 0 otherwise
volatile static uint8_t  uartReceiverBusy;
// 1 if we're transmitting data, 0 otherwise
volatile static uint8_t  uartTransmitterBusy;
// Counter to rectify the baud rate (since we're ticking at 3x baud rate)
volatile static uint8_t  uartTransmitterCounter;
// Number of transmission bits left to send
volatile static uint8_t  uartTxBitsRemaining;
// 10 bits long, so need 16-bit value instead of 8
volatile static uint16_t uartTxBitBuffer;
// Number of ticks since we last saw data (max of BT_UART_PACKET_WAIT_TICKS)
volatile static uint16_t uartPacketWaitTimer = 0;
// Number of ticks since we last performed a state check (max of BT_UART_STATE_CHECK_TICKS)
volatile static uint16_t uartStateCheckTimer = 0;
// Number of ticks since we last incremented the millisecond counter (max of BT_UART_MILLISECOND_TICKS)
volatile static uint16_t uartMillisecondCountTimer = 0;
// Each bit represents the status of the Bluetooth state (smaller positions indicate newer times)
volatile static uint16_t uartConnectionState = 0;
// Track previous state of the connection (so we can fire handlers if the next state does not match)
volatile static uint8_t  uartPrevConnected = 0;
// Track current state of the connection (so we can fire handlers)
volatile static uint8_t  uartConnected = 0;

// This ISR runs reach time the timer overflows, which happens at 3x the specified baud rate
ISR(BT_TIMER_INTERRUPT_VECTOR) {
    // 1 if we're waiting for the stop bit for a packet
    static uint8_t uartAwaitingStopBit = 0;
    // Tracks the current bit position in the receiving buffer
    static uint8_t uartReceiverMask;
    // Counter to rectify the baud rate (since we're ticking at 3x baud rate)
    static uint8_t uartReceiverCounter;
    // Number of bits left to be received in the current packet
    static uint8_t uartRxBitsRemaining;
    // Buffer to store the byte currently being constructed
    static uint8_t uartRxBitBuffer;

    // Temporary variable to store counters for operations
    uint8_t counter;

    // Send data from the output buffer (if there is data to send)
    if (uartTransmitterBusy) {
        counter = uartTransmitterCounter;
        if (--counter == 0) {
            // Send next bit in output buffer
            if (uartTxBitBuffer & 0x01)
                bt_uartSetTxHigh();
            else
                bt_uartSetTxLow();
            
            // Pop the bit off the buffer
            uartTxBitBuffer >>= 1;
            // Reset counter
            counter = 3;
            // If there aren't any bits left to send, set transmitter to ready
            if (--uartTxBitsRemaining == 0)
                uartTransmitterBusy = 0;
        }
        uartTransmitterCounter = counter;
    }

    // Read data off of the UART receiver pin into the input buffer
    if (uartAwaitingStopBit) {
        if (--uartReceiverCounter == 0) {
            // Tell receiver we're ready for the next byte
            uartAwaitingStopBit = 0;
            uartReceiverBusy = 0;
            // Insert received bit into input buffer
            uartInputBuffer[uartBufferInputIndex] = uartRxBitBuffer;
            // Increment buffer index (or wrap if at end)
            if (++uartBufferInputIndex >= BT_UART_RX_BUFFER_LENGTH)
                uartBufferInputIndex = 0;
            
            // Reset the UART packet wait timer
            uartPacketWaitTimer = 0;
        }
    } else {
        // If we're not currently reading a byte, wait
        // for the next start bit
        if (uartReceiverBusy == 0) {
            // If we receive the start bit, initialize the receiver values
            if (bt_uartGetRx() == 0) {
                uartReceiverBusy = 1;
                uartRxBitBuffer = 0;
                uartReceiverCounter = 4;
                uartRxBitsRemaining = BT_UART_RX_BITS;
                uartReceiverMask = 1;
                uartPacketWaitTimer = 0;
            } else {
                // Since we didn't receive the start bit, increment the packet wait timer
                // so we can determine if any more data is being sent
                if (uartPacketWaitTimer < BT_UART_PACKET_WAIT_TICKS)
                    uartPacketWaitTimer++;
            }
        } else {
            counter = uartReceiverCounter;
            if (--counter == 0) {
                // Reset counter
                counter = 3;

                // Receive the next bit and insert it into the buffer
                if (bt_uartGetRx())
                    uartRxBitBuffer |= uartReceiverMask;
                // Shift the receiver mask for the next bit
                uartReceiverMask <<= 1;
                
                // If we've received a full byte, wait for the stop bit
                if (--uartRxBitsRemaining == 0)
                    uartAwaitingStopBit = 1;
            }
            uartReceiverCounter = counter;
        }
    }

    // Increment the millisecond counter if 1ms has elapsed
    if (uartMillisecondCountTimer++ == BT_UART_MILLISECOND_TICKS) {
        // Reset timer
        uartMillisecondCountTimer = 0;

        // Increment the millisecond counter
        uartMillisecondCounter++;
    }

    // Check the status of the Bluetooth state if we've reached the threshold of the timer
    if (uartStateCheckTimer++ == BT_UART_STATE_CHECK_TICKS) {
        // Reset timer
        uartStateCheckTimer = 0;

        // Check the state of the module
        uartConnectionState = (uartConnectionState << 1) | (bt_uartGetState() != 0);

        // Check for changes in connection state (and if handlers are enabled, run them)
        uartPrevConnected = uartConnected;
        uartConnected = (uartConnectionState & 0x0F) == 0x0F;
        #if BT_ENABLE_CONNECTION_HANDLER
            if (!uartPrevConnected && uartConnected)
                BT_CONNECTION_HANDLER();
        #endif
        #if BT_ENABLE_DISCONNECTION_HANDLER
            if (uartPrevConnected && !uartConnected)
                BT_DISCONNECTION_HANDLER();
        #endif

        // Decrement the initial connection check counter if it's not at 0
        if (uartInitialConnectionCheckCountdown)
            uartInitialConnectionCheckCountdown--;
    }
}

/*
 * ---------------------------------------------------------------------
 * Internal utility functions for initializing the software UART stream:
 * ---------------------------------------------------------------------
 */

void bt_initializeUARTPins() {
    // Set TX pin to output, and RX and State pins to input
    BT_TX_DDR |= (1 << BT_TX_BIT);
    BT_RX_DDR &= ~(1 << BT_RX_BIT);
    BT_STATE_DDR &= ~(1 << BT_STATE_BIT);
}

void bt_initializeUARTTimer() {
    // Save the status register so we can restore it later
    uint8_t sregTemp = SREG;
    // Disable interrupts while the timer is initialized
    cli();

    // Setup the UART interrupt timer
    BT_TIMER_COMPARE_REGISTER = BT_TIMER_TOP;
    BT_TIMER_CONTROL_REGISTER_A = BT_TIMER_CONTROL_REGISTER_A_MASK | BT_TIMER_PRESCALER_REG_A_MASK;
    BT_TIMER_CONTROL_REGISTER_B = BT_TIMER_CONTROL_REGISTER_B_MASK | BT_TIMER_PRESCALER_REG_B_MASK;
    BT_TIMER_INTERRUPT_MASK_REGISTER |= BT_TIMER_INTERRUPT_ENABLE_MASK;
    // Set counter to 0
    BT_TIMER_COUNTER_REGISTER = 0;

    // Restore the status register
    SREG = sregTemp;
}

void bt_initializeUART() {
    // Set busy flags to false initially
    uartTransmitterBusy = 0;
    uartReceiverBusy = 0;

    // Turn on TX pin
    bt_uartSetTxHigh();

    // Initialize pins/timer used for UART
    bt_initializeUARTPins();
    bt_initializeUARTTimer();
}

/*
 * ------------------------------------------------------------
 * Utility functions for manipulating the software UART stream:
 * ------------------------------------------------------------
 */

uint8_t bt_connected() {
    // We're connected if the last 4 connection checks (across about 1 second) returned 1
    return uartConnected;
}

uint8_t bt_available() {
    // Check if the most-recently read byte is the most recent input
    return uartBufferInputIndex != uartBufferReadIndex;
}

uint8_t bt_awaitAvailable() {
    // Check if there is a bit currently available,
    // and if not, check if the receiver is currently
    // reading a byte.  If so, wait for that bit to
    // be received
    if (uartBufferInputIndex != uartBufferReadIndex) {
        return 1;
    } else {
        // Wait for the receiver to finish reading a byte (if it currently is) and wait for the configured amount of time
        // to verify there is no more data being sent
        while ((uartReceiverBusy || uartPacketWaitTimer < BT_UART_PACKET_WAIT_TICKS) && (uartBufferInputIndex == uartBufferReadIndex));

        // Now that the receiver is done or has read a bit, check again for availability
        return uartBufferInputIndex != uartBufferReadIndex;
    }
}

void bt_write(const uint8_t byte) {
    // Wait for the transmitter to finish its work
    while (uartTransmitterBusy); // TODO - timeout?

    // Set up transmitter to transmit the byte
    uartTransmitterCounter = 3;
    uartTxBitsRemaining = BT_UART_TX_BITS;
    // Transform the byte into a UART packet
    uartTxBitBuffer = (byte << 1) | 0x200;
    // Notify transmitter there is a byte available
    uartTransmitterBusy = 1;
}

uint8_t bt_read() {
    // If we haven't read any new characters, return \0
    if (uartBufferInputIndex == uartBufferReadIndex)
        return 0;

    // Pull the latest byte from the input buffer
    uint8_t in = uartInputBuffer[uartBufferReadIndex];
    // Increment the read index (wrapping if it exceeds the buffer size)
    if (++uartBufferReadIndex >= BT_UART_RX_BUFFER_LENGTH)
        uartBufferReadIndex = 0;
    
    // Return the byte
    return in; 
}

void bt_flush() {
    // Reset read indexes
    uartBufferInputIndex = 0;
    uartBufferReadIndex = 0;
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
    while (bt_awaitAvailable() && bufferIndex < bufferLength - 1) {
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
    if (bt_available())
        while (bt_awaitAvailable() && (input = bt_read()) != delimiter);

    return strlen(buffer);
}
