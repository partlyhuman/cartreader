/********************************************************************
                    Open Source Cartridge Reader
********************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_

/***** FIRMWARE CONFIGURATION **************************************

    Add or remove the "//" in front of items to toggle them.

    Disabled:
      //#define HW5

    Enabled:
      #define HW5

    Things in ** blocks like this are comments. Changing them doesn't
    affect the firmware that is flashed to your OSCR.

    If you only get a blank screen or "Press Button" message after
    flashing you have enabled too many modules.

********************************************************************/

/*==== HARDWARE VERSION ===========================================*/

/*
    Choose your hardware version:
*/

#define HW5
//#define HW4
//#define HW3
//#define HW2
//#define HW1
//#define SERIAL_MONITOR

/****/

/*==== HARDWARE MODULES ===========================================*/

/* [ Automatic Voltage Selection ---------------------------------- ]
    Enable this if you have the VSELECT module.
*/

//#define ENABLE_VSELECT

/****/

/* [ Clock Generator ---------------------------------------------- ]
    Enable this if you have the clock generator module. This will
    automatically be enabled if you selected HW2 or newer above.
*/

//#define clockgen_installed

/****/

/* [ Real Time Clock ---------------------------------------------- ]
    Enable this if you have the RTC module. You can configure the
    type later in this file.
*/

//#define RTC_installed

/****/

/*==== GAME SYSTEM CORES ==========================================*/

/* [ Atari 2600 --------------------------------------------------- ]
*/

//#define enable_ATARI

/****/

/* [ Atari 5200 --------------------------------------------------- ]
*/

//#define enable_5200

/****/

/* [ Atari 7800 --------------------------------------------------- ]
*/

//#define enable_7800

/****/

/* [ Benesse Pocket Challenge W ----------------------------------- ]
*/

//#define enable_PCW

/****/

/* [ C64 --------------------------------------------------- ]
*/

//#define enable_C64

/****/

/* [ ColecoVision ------------------------------------------------- ]
*/

//#define enable_COLV

/****/

/* [ Emerson Arcadia 2001 ----------------------------------------- ]
*/

//#define enable_ARC

/****/

/* [ Fairchild Channel F ------------------------------------------ ]
*/

//#define enable_FAIRCHILD

/****/

/* [ Flashrom Programmer for SNES repros -------------------------- ]
*/

//#define enable_FLASH
//#define enable_FLASH16

/****/

/* [ Game Boy (Color) and Advance --------------------------------- ]
*/

#define enable_GBX

/****/

/* [ Intellivision ------------------------------------------------ ]
*/

//#define enable_INTV

/****/

/* [ Neo Geo Pocket ----------------------------------------------- ]
*/

//#define enable_NGP

/****/

/* [ Nintendo 64 -------------------------------------------------- ]
*/

#define enable_N64

/****/

/* [ Nintendo Entertainment System/Family Computer ---------------- ]
*/

#define enable_NES

/****/

/* [ Magnavox Odyssey 2 ------------------------------------------- ]
*/

//#define enable_ODY2

/****/

/* [ MSX ------------------------------------------- ]
*/

//#define enable_MSX

/****/

/* [ PC Engine/TurboGrafx 16 -------------------------------------- ]
*/

//#define enable_PCE

/****/

/* [ Pokemon Mini -------------------------------------- ]
*/

//#define enable_POKE

/****/

/* [ Sega Master System/Mark III/Game Gear/SG-1000 ---------------- ]
*/

//#define enable_SMS

/****/

/* [ Sega Mega Drive/Genesis -------------------------------------- ]
*/

#define enable_MD

/****/

/* [ Super Famicom SF Memory Cassette ----------------------------- ]
*/

//#define enable_SFM

/****/

/* [ Super Famicom Satellaview ------------------------------------ ]
*/

//#define enable_SV

/****/

/* [ Super Famicom Sufami Turbo ----------------------------------- ]
*/

// #define enable_ST

/****/

/* [ Super Nintendo ----------------------------------------------- ]
*/

#define enable_SNES

/****/

/* [ Vectrex --------------------------------------------------- ]
*/

//#define enable_VECTREX

/****/

/* [ Virtual Boy -------------------------------------------------- ]
*/

//#define enable_VBOY

/****/

/* [ Watara Supervision ------------------------------------------- ]
*/

//#define enable_WSV

/****/

/* [ WonderSwan and Benesse Pocket Challenge v2 ------------------- ]
*/

//#define enable_WS

/****/

/* [ Super A'can -------------------------------------------------- ]
*/

//#define enable_SUPRACAN

/****/

/* [ Casio Loopy -------------------------------------------------- ]
*/

#define enable_LOOPY

/****/

/*==== FIRMWARE OPTIONS ===========================================*/

/* [ LCD: Background Color ---------------------------------------- ]
    Set the backlight color of the LCD if you have one.

    PARAMETERS:
      Green, Red, Blue
*/

#define background_color 20, 100, 40

/****/

/* [ 3.3V Stability Fix (3V3FIX) ---------------------------------- ]
    Enable this if you are having stability issues when using 3.3V,
    works best with VSELECT.

    If not using VSELECT, always turn the cart reader on with the
    voltage switch set to 5V and switch to 5V before selecting a
    cartridge from the menu.
*/

//#define ENABLE_3V3FIX

/****/

/* [ Updater ------------------------------------------------------ ]
    Disable this if you don't plan to/want to use the firmware
    updater utility. This setting is ignored on hardware versions
    other than HW5 and HW3.
*/

//#define ENABLE_UPDATER

/****/

/* [ Self Test ---------------------------------------------------- ]
    Tests for shorts and other issues in your OSCR build.
*/

#define enable_selftest

/****/

/* [ Logging ------------------------------------------------------ ]
    Write all info to OSCR_LOG.txt in root dir
*/

#define global_log

/****/

/* [ RTC: IC Type ------------------------------------------------- ]
    When the RTC module is installed, choose the type here. This
    setting is ignored if the RTC option is not enabled.
*/

#define DS3231
//#define DS1307

/****/

/* [ SNES Core/CLOCKGEN: Read Clock Generator Calibration Data ---- ]
    Toggle clock calibration menu and whether or not to use calibration data from snes_clk.txt
*/

//#define clockgen_calibration
//#define use_clockgen_calibration

/****/

/* [ MegaDrive/Genesis Core: Compatibility Settings --------------- ]
    Allows you to create a text file on the SD card called
    "mdconf.txt" which you should place the following into:

    [segaSram16bit] N

    Where N is:
      0: Output each byte once. Not supported by emulators. (default)
      1: Duplicate each byte. Usable by Kega Fusion.
      2: Same as 1 + pad with 0xFF so that the file size is 64KB.
*/

//#define use_md_conf

/*
    Alternatively, define it here by uncommenting and changing the
    following line. Setting both allows you to change the default.
*/

//#define DEFAULT_VALUE_segaSram16bit 0

/****/

/* [ N64 Core: Fast CRC ------------------------------------------- ]
    Toggle so the CRC for N64 Roms will be calculated during dumping
    from memory instead of after dumping from SD card, not compatible
    with all Cart Readers
*/

//#define fastcrc

/****/

/* [ N64 Core: Log Summary ---------------------------------------- ]
    Enable to save a n64log.txt file with rom info in /N64/ROM
*/

//#define savesummarytotxt

/****/

/*==== PROCESSING =================================================*/

/*
              You probably shouldn't change this stuff!
*/

#if (defined(HW4) || defined(HW5))
#define enable_LCD
#define enable_neopixel
#define enable_rotary
//#define rotate_counter_clockwise
#define clockgen_installed
#define fastcrc
#define ws_adapter_v2
#endif

#if (defined(HW2) || defined(HW3))
#define enable_OLED
#define enable_Button2
#define clockgen_installed
#define CA_LED
#define fastcrc
#endif

#if defined(HW1)
#define enable_OLED
//#define clockgen_installed
//#define fastcrc
#endif

#if defined(SERIAL_MONITOR)
#define enable_serial
//#define clockgen_installed
//#define fastcrc
#endif

/* Firmware updater only works with HW3 and HW5 */
#if !(defined(HW5) || defined(HW3))
#undef ENABLE_UPDATER
#endif

/* End of settings */

#endif /* CONFIG_H_ */
