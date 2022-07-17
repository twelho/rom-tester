# rom-tester

The Universal ROM Tester (`rom-tester`) is an Arduino-based testing tool for TTL (5 volt) compatible ROM modules, such as the lower and upper ROMs found in a 1541. It is derived from and augments my [`sram-tester`](https://github.com/twelho/sram-tester) as a tool to help with debugging old computer hardware.

`rom-tester` reads the contents of a ROM module and generates a SHA256 checksum that can be compared to the checksum of a known-good ROM dump. Reference ROM dumps can often be found online, for example the contents of the ROMs used in the 1541 floppy disk drive can be found [here](http://www.zimmers.net/anonftp/pub/cbm/firmware/drives/new/1541/).

# Usage

`rom-tester` should run on most 8-bit Arduino hardware with sufficient pins to communicate with the ROM module of your choice, but I have only verified its functionality on an Arduino Mega 2560. If you are running out of memory (on boards with < 2K RAM), decrease the `BLOCK_SIZE` value to read the ROM in smaller chunks.

> `rom-tester` depends on the [Arduino Cryptography Library](https://rweather.github.io/arduinolibs/crypto.html), which can be found under the name "Crypto" in the Library Manager of the Arduino IDE. Version 0.4.0 has been tested and is confirmed working.

Before flashing the sketch to your Arduino, create a profile for your configuration by editing the following constants defined in the beginning of the file:

- `addressPins`: The Arduino pins that correspond to the address pins of the ROM in the order expected by the ROM (starting from e.g. `A0`, check the datasheet for your ROM).
- `dataPins`: The Arduino pins that correspond to the data pins of the ROM in the order expected by the ROM (starting from e.g. `D0`, check the datasheet for your ROM).
- `addressCount`: The number of addressable memory locations in your ROM model (i.e. its capacity at its native data bus width).

There is an example configuration for testing the lower/upper ROM of a 1541 with a Mega 2560 already in place.

Connect your ROM module to the Arduino and flash the sketch using the Arduino IDE. The test begins automatically, and can be followed over serial at a baud rate of 115200. The SHA-256 checksum of the ROM contents will be output after the test has finished. You can reset the Arduino (e.g. via the button) to start a new test (e.g. after swapping SRAM modules).

Compare the checksum output by this program against the checksum of a known-good ROM dump. If you have the ROM dump, you can easily generate the checksum with the command `sha256sum <filename>` under Linux (look up the equivalent for other operating systems). If the checksums match, your ROM is in perfect condition. If not, then your ROM *might* be bad, check below.

## Help, my checksums don't match!

Make sure to reset the Arduino and run the program a couple of times to make sure that the **checksums of back-to-back runs match**. If they don't, it is more likely that your wiring/connections are bad instead of the ROM being bad. Reseat the ROM module and double-check your wiring, then try again.

# Authors

- Dennis Marttinen ([@twelho](https://github.com/twelho))

# License

[MIT](LICENSE)
