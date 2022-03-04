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

See [the wiki page]() for a description of the different options.

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

See [the wiki page]() for a description of the different available configuration functions.

### Transmitting/Receiving Data

Once the module is configured, it can be used to communicate with remote devices using the various reading/writing functions.  For example, to send the string `Hello, world!` to a remote device and wait for a response, the following program could be used:

```c
int main() {
    bt_setup();

    bt_writeString("Hello, world!\n");

    char buffer[20];
    bt_readString('\n', buffer, 20);

    return 0;
}
```

See [the wiki page]() for a description of the different write and read functions.

*Note: Configuration commands will not work while the module is connected to a remote device, as the mechanisms used to send these commands to the module is the same as the one used to send data to the remote device.  However, using the `bt_test()` function while connected to a remote device will cause a disconnection, and you will need to reconnect the devices, due to the functionality of the underlying `AT` command it uses.*

## Acknowledgements

The software UART code is based on/modified from [this repository](https://github.com/blalor/avr-softuart).  To this code, I have:
- Improved the coherence and legibility of constant and macro names (especially ones that are user-configurable)
- Added logic to detect consecutive byte transmissions (useful for transmissions consisting of multiple bytes where there is no consistent termination point)
- Added logic to determine if a remote device is connected (and allow handlers to run when one is connected/disconnected)
- Added logic to transmit/receive additional data types (like strings)
