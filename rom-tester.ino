// SPDX-License-Identifier: MIT

/*
 * Universal ROM Tester - Outputs a checksum of your ROM contents
 * 
 * Depends on https://rweather.github.io/arduinolibs/crypto.html,
 * which can be found under the name "Crypto" in the Library
 * Manager of the Arduino IDE. Version 0.4.0 has been tested.
 *
 * (c) Dennis Marttinen 2022
 * Licensed under the MIT license
 */
 
#include <SHA256.h>

// ------------------------------

/*
 * 1541 ROM (ATMega 2560 pinout)
 * Pin 12 (Vss) is wired to GND
 * Pin 18 (CS) is wired to GND
 * Pin 20 (OE) is wired to GND
 * Pin 24 (Vcc) is wired to +5V
*/

// Address Pins
const uint8_t addressPins[] = {
  47, 45, 43, 41, 39, 37, 35, 33, 32, 34, 38, 50, 36
};

// Data Pins
const uint8_t dataPins[] = {
  49, 51, 53, 48, 46, 44, 42, 40
};

// The number of addressable memory locations
const uint32_t addressCount = 8192;

// ------------------------------

#define HASH_SIZE 32
#define BLOCK_SIZE 1024
#define NX(x) (sizeof(x) / sizeof((x)[0]))

const size_t NA = NX(addressPins);
const size_t ND = NX(dataPins);

// Set Address pins to output
void setupAddressPins() {
  for (size_t i = 0; i < NA; i++) {
    pinMode(addressPins[i], OUTPUT);
  }
}

// Set Data pins to input
void setDataPins() {
  for (size_t i = 0; i < ND; i++) {
    pinMode(dataPins[i], INPUT);
  }
}

SHA256 hash;

// Initial setup of pins and serial monitor
void setup() {
  // Initialize all pins
  setupAddressPins();
  setDataPins();
  
  // Initialize Serial Port
  Serial.begin(115200);
  Serial.println("Universal ROM Tester by Dennis Marttinen");
  Serial.print(NA);
  Serial.print('/');
  Serial.print(ND);
  Serial.println(" address/data pin(s) configured");
}

// Set the address pins to match the specified address
void setAddressBits(size_t address) {
  for (size_t i = 0; i < NA; i++) {
    digitalWrite(addressPins[i], bitRead(address, i));
  }
}

// Read data from the specified memory address
size_t readData(size_t address) {
  // Set address bits
  setAddressBits(address);
  
  // Wait for the logic to be stabilized
  //delay(1);

  // Read each data bit one by one
  size_t result = 0;
  for (size_t i = 0; i < ND; i++) {
    bitWrite(result, i, digitalRead(dataPins[i]));
  }
  return result;
}

void readBlock(size_t address, uint8_t* target, size_t count) {
  for (size_t i = 0; i < count; i++) {
    target[i] = readData(address + i) & 0xff;
  }
}

#define hex(b) ('0' + (b) + ((b) / 10) * 7)

void printHash(uint8_t* hash, size_t size) {
  for (size_t i = 0; i < size; i++) {
    Serial.print((char) hex(hash[i] >> 4 & 0xf));
    Serial.print((char) hex(hash[i] & 0xf));
  }
}

void loop() {
  uint8_t value[HASH_SIZE];
  uint8_t block[BLOCK_SIZE];

  Serial.println("Initializing hash");
  hash.reset();

  for (size_t address = 0; address != addressCount;) {
    size_t count = min(sizeof(block), addressCount - address);
    Serial.print("Reading ");
    Serial.print(count);
    Serial.println(" bytes");
    
    readBlock(address, block, count);
    hash.update(block, count);
    address += count;
  }

  Serial.println("Finalizing hash");
  hash.finalize(&value, sizeof(value));

  Serial.println("Done! ROM checksum:");
  printHash(value, sizeof(value));
  Serial.println();

  // Nothing more to do, so loop indefinitely - or until Reset is pressed
  while (1) {};
}
