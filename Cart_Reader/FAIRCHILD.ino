//******************************************
// FAIRCHILD CHANNEL F MODULE
//******************************************
#if defined(ENABLE_FAIRCHILD)
// Fairchild Channel F
// Cartridge Pinout
// 22P (27P Width) 2.54mm pitch connector
//
//   TOP            BOTTOM
//  SIDE            SIDE
//        +-------+
//        |    == |
//        |     1 |- GND
//        |     2 |- GND
//        |     3 |- D0
//        |     4 |- D1
//        |     5 |- /INTREQ
//        |     6 |- ROMC0
//        |     7 |- ROMC1
//        |     8 |- ROMC2
//        |     9 |- D2
//        |    10 |- ROMC3
//        |    11 |- D3
//        |    == |
//        |    == |
//        |    == |
//        |    12 |- ROMC4
//        |    13 |- PHI
//        |    14 |- D4
//        |    15 |- WRITE
//        |    16 |- D5
//        |    17 |- D6
//        |    18 |- D7
//        |    19 |- VDD(+5V)
//        |    20 |- VDD(+5V)
//        |    21 |- NC
//        |    22 |- VGG(+12V)
//        |    == |
//        +-------+
//
//                                               TOP
//       +----------------------------------------------------------------------------------+
//       |                                                                                  |
// LEFT  |                                                                                  | RIGHT
//       | == 22 21 20 19 18 17 16 15 14 13 12 == == == 11 10  9  8  7  6  5  4  3  2  1 == |
//       +----------------------------------------------------------------------------------+
//                                              BOTTOM
//

// CONTROL PINS:
// PHI(PH3)     - SNES /CS
// /INTREQ(PH4) - SNES /IRQ
// WRITE(PH5)   - SNES /WR
// ROMC0(PF0)   - SNES A0
// ROMC1(PF1)   - SNES A1
// ROMC2(PF2)   - SNES A2
// ROMC3(PF3)   - SNES A3
// ROMC4(PF4)   - SNES A4

/******************************************
  Defines
 *****************************************/
#define PHI_HI PORTH |= (1 << 3)
#define PHI_LOW PORTH &= ~(1 << 3)
#define WRITE_HI PORTH |= (1 << 5)
#define WRITE_LOW PORTH &= ~(1 << 5)

byte FAIRCHILD[] = { 2, 3, 4, 6 };
byte fairchildlo = 0;  // Lowest Entry
byte fairchildhi = 3;  // Highest Entry

byte fairchildsize;

// EEPROM MAPPING
// 08 ROM SIZE

//******************************************
//  Menu
//******************************************
// Base Menu
static const char fairchildMenuItem4[] PROGMEM = "Read 16K";
static const char* const menuOptionsFAIRCHILD[] PROGMEM = { FSTRING_SELECT_CART, FSTRING_READ_ROM, FSTRING_SET_SIZE, fairchildMenuItem4, FSTRING_RESET };

void setup_FAIRCHILD() {
  // Request 5V
  setVoltage(VOLTS_SET_5V);

  // Set Address Pins to Output
  // Channel F uses A0-A4 [A5-A23 UNUSED]
  //A0-A7
  DDRF = 0xFF;
  //A8-A15
  DDRK = 0xFF;
  //A16-A23
  DDRL = 0xFF;

  // Set Control Pins to Output
  //       ---(PH0)   ---(PH1)   PHI(PH3) /INTREQ(PH4) WRITE(PH5) ---(PH6)
  DDRH |= (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);

  // Set TIME(PJ0) to Output (UNUSED)
  DDRJ |= (1 << 0);

  // Set Pins (D0-D7) to Input
  DDRC = 0x00;

  // Setting Unused Control Pins to HIGH
  //       ---(PH0)   ---(PH1)   PHI(PH3) /INTREQ(PH4) WRITE(PH5)   ---(PH6)
  PORTH |= (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);

  // Set Unused Data Pins (PA0-PA7) to Output
  DDRA = 0xFF;

  // Set Unused Pins HIGH
  PORTA = 0xFF;
  PORTK = 0xFF;       // A8-A15
  PORTL = 0xFF;       // A16-A23
  PORTJ |= (1 << 0);  // TIME(PJ0)

  checkStatus_FAIRCHILD();
  strcpy(romName, "FAIRCHILD");

  mode = CORE_FAIRCHILD;
}

void fairchildMenu() {
  convertPgm(menuOptionsFAIRCHILD, 5);
  uint8_t mainMenu = question_box(F("CHANNEL F MENU"), menuOptions, 5, 0);

  switch (mainMenu) {
    case 0:
      // Select Cart
      setCart_FAIRCHILD();
      wait();
      setup_FAIRCHILD();
      break;

    case 1:
      // Read ROM
      sd.chdir("/");
      readROM_FAIRCHILD();
      sd.chdir("/");
      break;

    case 2:
      // Set Size
      setROMSize_FAIRCHILD();
      break;

    case 3:
      // Read 16K
      sd.chdir("/");
      read16K_FAIRCHILD();
      sd.chdir("/");
      break;

    case 4:
      // reset
      resetArduino();
      break;
  }
}

//******************************************
// READ CODE
//******************************************

// Sean Riddle Dumper Routine
// clear PC0 with ROMC state 8
// loop 256 times
// fetch 16 bytes into buffer with ROMC state 0
// dump buffer to serial port
// clear PC0

// Clear PC0
void clearRegister_FAIRCHILD() {
  PHI_LOW;
  WRITE_LOW;
  PORTF = 0;  // ROMC3 LOW

  delay(2000);

  PHI_HI;
  WRITE_HI;
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  WRITE_LOW;
  PHI_HI;
  PORTF = 0;  // ROMC3 LOW
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  PORTF = 0;  // ROMC3 LOW
  PHI_HI;
  PORTF = 0x8;  // this puts us in ROMC state 8 - clear PC0
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  PORTF = 0x08;  // ROMC3 HIGH
  PHI_HI;
  PORTF = 0x08;  // ROMC3 HIGH
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  PHI_HI;
  WRITE_HI;
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  WRITE_LOW;
  PHI_HI;
  PORTF = 0;  // ROMC3 LOW
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  WRITE_LOW;
  PHI_HI;
  PORTF = 0;  // ROMC3 LOW
  NOP;
  NOP;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
  NOP;

  WRITE_LOW;
}

void setROMC_FAIRCHILD(uint8_t command) {
  PHI_LOW;
  WRITE_LOW;
  NOP;

  WRITE_HI;
  PHI_HI;
  NOP;

  PHI_LOW;
  NOP;
  NOP;

  WRITE_LOW;
  PHI_HI;
  NOP;
  NOP;

  // PWs = 4 PHI Cycles
  // PWl = 6 PHI Cycles
  for (int x = 0; x < 2; x++) {  // 2 PHI
    PHI_LOW;
    NOP;
    NOP;
    PHI_HI;
    NOP;
    NOP;
  }
  PORTF = command;  // ROMC3 = command

  for (int x = 0; x < 3; x++) {  // 4 PHI
    PHI_LOW;
    NOP;
    NOP;
    PHI_HI;
    NOP;
    NOP;
  }

  PHI_LOW;
  NOP;
  NOP;

  PHI_HI;
  WRITE_HI;
  NOP;

  PHI_LOW;
  NOP;
  NOP;

  PHI_HI;
  WRITE_LOW;
  NOP;

  PHI_LOW;
  NOP;
  NOP;
}

void setREAD_FAIRCHILD() {
  PHI_LOW;
  WRITE_LOW;
  NOP;

  WRITE_HI;
  PHI_HI;
  NOP;

  PHI_LOW;
  NOP;
  NOP;

  WRITE_LOW;
  PHI_HI;
  NOP;
  NOP;

  // PWs = 4 PHI Cycles
  // PWl = 6 PHI Cycles
  for (int x = 0; x < 2; x++) {  // 2 PHI
    PHI_LOW;
    NOP;
    NOP;
    PHI_HI;
    NOP;
    NOP;
  }
  PORTF = 0;  // ROMC3 = 0 = Fetch Data
}

uint8_t readData_FAIRCHILD() {
  for (int x = 0; x < 3; x++) {  // 4 PHI
    PHI_LOW;
    NOP;
    NOP;
    PHI_HI;
    NOP;
    NOP;
  }

  PHI_LOW;
  NOP;
  NOP;

  PHI_HI;
  WRITE_HI;
  NOP;

  PHI_LOW;
  NOP;
  NOP;

  PHI_HI;
  WRITE_LOW;
  NOP;

  uint8_t ret = PINC;  // read databus into buffer

  PHI_LOW;
  NOP;
  NOP;

  return ret;
}

void readROM_FAIRCHILD() {
  createFolderAndOpenFile("FAIRCHILD", "ROM", romName, "bin");

  unsigned long cartsize = FAIRCHILD[fairchildsize] * 0x400;
  uint8_t blocks = cartsize / 0x200;
  setROMC_FAIRCHILD(0x8);  // Clear PC0
  setREAD_FAIRCHILD();

  // ROM Start Bytes
  // 0x55,0x08 - desert fox, muehle, space war, tic-tac-toe (all 2K)
  // 0x55,0x2B - most carts
  // 0x55,0xAA - alien invasion (4K)
  // 0x55,0xBB - video whizball (3K)
  for (uint16_t y = 0; y < 0x4800; y++) {
    uint8_t startbyte = readData_FAIRCHILD();
    if (startbyte == 0x55) {  // Start Byte
      sdBuffer[0] = startbyte;
      startbyte = readData_FAIRCHILD();
      if ((startbyte == 0x08) || (startbyte == 0x2B) || (startbyte == 0xAA) || (startbyte == 0xBB)) {
        sdBuffer[1] = startbyte;
        for (int w = 2; w < 512; w++) {
          startbyte = readData_FAIRCHILD();
          sdBuffer[w] = startbyte;
        }
        myFile.write(sdBuffer, 512);
        delay(1);  // Added delay
        for (int z = 1; z < blocks; z++) {
          if (cartsize == 0x0C00) {  // 3K
            // Skip SRAM Code for 3K Carts - Tested with Hangman 3K
            // Hangman uses an F21022PC 1K SRAM Chip at 0x0400
            // SRAM is NOT Battery Backed so contents change
            // Chips are organized: 1K ROM + 1K SRAM + 1K ROM + 1K ROM
            if (z == 2) {
              for (int x = 0; x < 0x0A00; x++) {  // Skip 1K SRAM at 0x0400
                readData_FAIRCHILD();
              }
            }
          } else if (cartsize == 0x1000) {  // 4K
            // Skip BIOS/Blocks Code for 4K Carts - Tested with Alien Invasion/Pro Football
            // Alien Invasion/Pro Football both use a DM74LS02N (Quad 2-Input NOR Gate) with two 2K ROM Chips
            uint16_t offset = z * 0x200;
            for (uint16_t x = 0; x < 0x800 + offset; x++) {  // Skip BIOS/Previous Blocks
              readData_FAIRCHILD();
            }
          }
          for (int w = 0; w < 512; w++) {
            uint8_t temp = readData_FAIRCHILD();
            sdBuffer[w] = temp;
          }
          myFile.write(sdBuffer, 512);
          delay(1);  // Added delay
        }
        break;
      }
    }
  }
  myFile.close();

  printCRC(fileName, NULL, 0);

  println_Msg(FS(FSTRING_EMPTY));
  print_STR(press_button_STR, 1);
  display_Update();
  wait();
}

void read16K_FAIRCHILD() { // Read 16K Bytes
  createFolderAndOpenFile("FAIRCHILD", "ROM", romName, "bin");

  unsigned long cartsize = FAIRCHILD[fairchildsize] * 0x400;
  for (uint16_t y = 0; y < 0x20; y++) {
    if (cartsize == 0x1000) {  // 4K
      // Skip BIOS/Blocks Code for 4K Carts - Tested with Alien Invasion/Pro Football
      // Alien Invasion/Pro Football both use a DM74LS02N (Quad 2-Input NOR Gate) with two 2K ROM Chips
      // IF CASINO POKER DOES NOT DUMP PROPERLY USING READROM
      // TEST BY SETTING ROM SIZE TO 2K AND 4K THEN COMPARE 16K DUMPS
      uint16_t offset = y * 0x200;
      for (uint16_t x = 0; x < 0x800 + offset; x++) {  // Skip BIOS/Previous Blocks
        readData_FAIRCHILD();
      }
    }
    for (int w = 0; w < 512; w++) {
      uint8_t temp = readData_FAIRCHILD();
      sdBuffer[w] = temp;
    }
    myFile.write(sdBuffer, 512);
    delay(1);  // Added delay
  }
  myFile.close();

  printCRC(fileName, NULL, 0);

  println_Msg(FS(FSTRING_EMPTY));
  print_STR(press_button_STR, 1);
  display_Update();
  wait();
}

//******************************************
// ROM SIZE
//******************************************

#if (defined(ENABLE_OLED) || defined(ENABLE_LCD))
void printRomSize_FAIRCHILD(int index) {
    display_Clear();
    print_Msg(FS(FSTRING_ROM_SIZE));
    println_Msg(FAIRCHILD[index]);
}
#endif

void setROMSize_FAIRCHILD() {
  byte newfairchildsize;
#if (defined(ENABLE_OLED) || defined(ENABLE_LCD))
  display_Clear();
  if (fairchildlo == fairchildhi)
    newfairchildsize = fairchildlo;
  else {
    newfairchildsize = navigateMenu(fairchildlo, fairchildhi, &printRomSize_FAIRCHILD);

    display.setCursor(0, 56);  // Display selection at bottom
  }
  print_Msg(FS(FSTRING_ROM_SIZE));
  print_Msg(FAIRCHILD[newfairchildsize]);
  println_Msg(F("K"));
  display_Update();
  delay(1000);
#else
  if (fairchildlo == fairchildhi)
    newfairchildsize = fairchildlo;
  else {
setrom:
    String sizeROM;
    for (int i = 0; i < (fairchildhi - fairchildlo + 1); i++) {
      Serial.print(F("Select ROM Size:  "));
      Serial.print(i);
      Serial.print(F(" = "));
      Serial.print(FAIRCHILD[i + fairchildlo]);
      Serial.println(F("K"));
    }
    Serial.print(F("Enter ROM Size: "));
    while (Serial.available() == 0) {}
    sizeROM = Serial.readStringUntil('\n');
    Serial.println(sizeROM);
    newfairchildsize = sizeROM.toInt() + fairchildlo;
    if (newfairchildsize > fairchildhi) {
      Serial.println(F("SIZE NOT SUPPORTED"));
      Serial.println(FS(FSTRING_EMPTY));
      goto setrom;
    }
  }
  Serial.print(F("ROM Size = "));
  Serial.print(FAIRCHILD[newfairchildsize]);
  Serial.println(F("K"));
#endif
  EEPROM_writeAnything(8, newfairchildsize);
  fairchildsize = newfairchildsize;
}

void checkStatus_FAIRCHILD() {
  EEPROM_readAnything(8, fairchildsize);
  if (fairchildsize > 3) {
    fairchildsize = 0;
    EEPROM_writeAnything(8, fairchildsize);
  }

#if (defined(ENABLE_OLED) || defined(ENABLE_LCD))
  display_Clear();
  println_Msg(F("CHANNEL F READER"));
  println_Msg(FS(FSTRING_CURRENT_SETTINGS));
  println_Msg(FS(FSTRING_EMPTY));
  print_Msg(FS(FSTRING_ROM_SIZE));
  print_Msg(FAIRCHILD[fairchildsize]);
  println_Msg(F("K"));
  display_Update();
  wait();
#else
  Serial.print(F("CURRENT ROM SIZE: "));
  Serial.print(FAIRCHILD[fairchildsize]);
  Serial.println(F("K"));
  Serial.println(FS(FSTRING_EMPTY));
#endif
}

//******************************************
// CART SELECT CODE
//******************************************
void setCart_FAIRCHILD() {
  //go to root
  sd.chdir();

  byte gameSize;

  // Select starting letter
  //byte myLetter = starting_letter();

  // Open database
  if (myFile.open("fairchildcart.txt", O_READ)) {
    // seek_first_letter_in_database(myFile, myLetter);

    if(checkCartSelection(myFile, &readDataLineSingleDigit, &gameSize)) {
      EEPROM_writeAnything(8, gameSize);
    }
  } else {
    print_FatalError(FS(FSTRING_DATABASE_FILE_NOT_FOUND));
  }
}
#endif
//******************************************
// End of File
//******************************************
