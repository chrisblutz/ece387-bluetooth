# Bluetooth 4.0 BLE (HM-11) - AVR Library

## Features
- Software UART implementation to provide serial communication with the Bluetooth module
  - Includes functions for determining data availability and connection status
- Functions for module configuration (including setting module name, PIN code, etc.)
- Utility functions for sending/receiving strings and other data types

## Using the Library

To use this library, copy the `.h` and `.c` files in the `lib/` directory to your project's library directory.  Include `bluetooth.h` in your project to access the API functions, and ensure that `bluetooth.c` is included in your build command(s).

### Configuring Pins and Options

The `bluetooth_settings.h` file contains the user-configurable settings for transmitter/receiver pins and other options.  By default, these are set up for a ATmega328P with a UART baud rate of 9600, using timer 0 for interrupts, and using pin 2 for receiving and pin 3 for transmitting.

See [the wiki page](https://github.com/chrisblutz/ece387-bluetooth/wiki/Documentation#library-settings) for a description of the different options.

### Configuring the Bluetooth Module

This library offers functions to configure the Bluetooth module (to set the name, PIN, etc.), but once the module has been configured once, it does not need to be configured again.  So, a program specifically designed to configure the Bluetooth module may be helpful, to avoid including unnecessary code in a final product.

For example, the following can be used to set up and run one-time configurations for a Bluetooth module:

```c
#include "bluetooth.h"

int main() {
    bt_setup();

    bt_setModuleName("ModuleName");
    bt_setModulePIN("123456");

    return 0;
}
```

See [the wiki page](https://github.com/chrisblutz/ece387-bluetooth/wiki/Documentation#configuration-functions) for a description of the different available configuration functions.

### Transmitting/Receiving Data

Once the module is configured, it can be used to communicate with remote devices using the various reading/writing functions.

#### Basic I/O

The most basic type of reading/writing that the library provides is byte-based.  The three main useful functions are:
- `uint8_t bt_available()` - checks if a byte is currently available
- `void bt_write(uint8_t)` - writes a byte
- `uint8_t bt_read()` - reads a byte

For example, the following code transmits several bytes through the Bluetooth connection, and then loops forever, handling any data that is sent back.

```c
#include "bluetooth.h"

int main() {
    bt_setup();

    bt_write('A');
    bt_write('B');
    bt_write('C');

    while (1) {
      if (bt_available()) {
        uint8_t in = bt_read();
        // Handle input stored in "in"
      }
    }

    return 0;
}
```

#### Advanced I/O

The library provides several utility functions for sending other types of objects as well:
- `bt_readString()`/`bt_writeString()` - reads/writes strings
- `bt_readInt32()`/`bt_writeInt32()` - reads/writes 32-bit signed integers
- `bt_readUInt32()`/`bt_writeUInt32()` - reads/writes 32-bit unsigned integers
- `bt_readInt16()`/`bt_writeInt16()` - reads/writes 16-bit signed integers
- `bt_readUInt16()`/`bt_writeUInt16()` - reads/writes 16-bit unsigned integers

See [the wiki page](https://github.com/chrisblutz/ece387-bluetooth/wiki/Documentation#uart-and-io) for a description of the different available write and read functions.

*Note: Configuration commands will not work while the module is connected to a remote device, as the mechanisms used to send these commands to the module is the same as the one used to send data to the remote device.  However, using the `bt_test()` function while connected to a remote device will cause a disconnection, and you will need to reconnect the devices, due to the functionality of the underlying `AT` command it uses.*

## Acknowledgements

The software UART code is based on/modified from [this repository](https://github.com/blalor/avr-softuart).  To this code, I have:
- Improved the coherence and legibility of constant and macro names (especially ones that are user-configurable)
- Added logic to detect consecutive byte transmissions (useful for transmissions consisting of multiple bytes where there is no consistent termination point)
- Added logic to determine if a remote device is connected (and allow handlers to run when one is connected/disconnected)
- Added logic to transmit/receive additional data types (like strings and integers)
