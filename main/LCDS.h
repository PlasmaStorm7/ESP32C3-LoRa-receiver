/************************************************************************/
/*																		*/
/*	LCDS.h	--	Declaration for LCDS library 	    				*/
/*																		*/
/************************************************************************/
/*	Author:		Monica Bot												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file declares functions for LCDS						*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	10/03/2011(MonicaB): created										*/
/*																		*/
/************************************************************************/
#if !defined(LCDS_H)
#define LCDS_H
//commands for the display list
#define ESC 					0x1B
#define BRACKET					0x5B //[
#define CURSOR_POS_CMD			0x48 //H
#define CURSOR_SAVE_CMD			0x73 //s
#define CURSOR_RSTR_CMD			0x75 //u
#define DISP_CLR_CMD			0x6A //j
#define ERASE_INLINE_CMD		0x4B //K
#define ERASE_FIELD_CMD			0x4E //N
#define LSCROLL_CMD				0x40 //@
#define RSCROLL_CMD				0x41 //A
#define RST_CMD					0x2A //*
#define DISP_EN_CMD				0x65 //e
#define DISP_MODE_CMD			0x68 //h
#define CURSOR_MODE_CMD			0x63 //c
#define TWI_SAVE_ADDR_CMD		0x61 //a
#define BR_SAVE_CMD				0x62 //b
#define PRG_CHAR_CMD			0x70 //p
#define SAVE_RAM_TO_EEPROM_CMD	0x74 //t
#define LD_EEPROM_TO_RAM_CMD	0x6C //l
#define DEF_CHAR_CMD			0x64 //d
#define COMM_MODE_SAVE_CMD		0x6D //m
#define EEPROM_WR_EN_CMD		0x77 //w
#define CURSOR_MODE_SAVE_CMD	0x6E //n
#define DISP_MODE_SAVE_CMD		0x6F //o

//access parameters for communication ports
#define	PAR_ACCESS_DSPI0			0
#define	PAR_ACCESS_DSPI1			1
#define	PAR_SPD_MAX				625000
/* ------------------------------------------------------------ */
/*					Errors Definitions							*/
/* ------------------------------------------------------------ */

#define LCDS_ERR_SUCCESS			0	// The action completed successfully
#define LCDS_ERR_ARG_ROW_RANGE		1	// The argument is not within 0, 2 range for rows
#define LCDS_ERR_ARG_COL_RANGE		2	// The argument is not within 0, 39 range
#define LCDS_ERR_ARG_ERASE_OPTIONS	3	// The argument is not within 0, 2 range for erase types
#define LCDS_ERR_ARG_BR_RANGE		4	// The argument is not within 0, 6 range
#define LCDS_ERR_ARG_TABLE_RANGE	5	// The argument is not within 0, 3 range for table selection
#define LCDS_ERR_ARG_COMM_RANGE		6	// The argument is not within 0, 7 range
#define LCDS_ERR_ARG_CRS_RANGE		7	// The argument is not within 0, 2 range for cursor modes
#define LCDS_ERR_ARG_DSP_RANGE		8	// The argument is not within 0, 3 range for display settings types
#define LCDS_ERR_ARG_POS_RANGE		9	// The argument is not within 0, 7 range for characters position in the memory

//other defines used for library functions
//#define MAX						150

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
//#include <DSPI.h>
//#include <inttypes.h>

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

// class LCDS {
// public:
// 	LCDS();
// 	//initializes the driver and configures the communication interface 
// 	void Begin(uint8_t accessType);
// 	//sets the enable/disable display options
// 	void DisplaySet(boolean setDisplay, boolean setBckl);
// 	//sets the cursor mode, with blink or not
// 	void CursorModeSet(boolean setCursor, boolean setBlink);
// 	//clears the display and returns the cursor home
// 	void DisplayClear();
// 	//writes a string at a specified position
// 	uint8_t WriteStringAtPos(uint8_t idxRow, uint8_t idxCol, char* strLn);  
// 	//scrolls the text to the left or right
// 	uint8_t DisplayScroll(boolean fDirection, uint8_t idxCol);
// 	//saves cursor position
// 	void SaveCursor();
// 	//restores cursor position
// 	void RestoreCursor();
// 	//wraps line at 16 or 40 characters 
// 	void DisplayMode(boolean charNumber);
// 	//erases within line
// 	uint8_t EraseInLine(uint8_t eraseParam);
// 	//erase a number of chars from the current line
// 	void EraseChars(uint8_t charsNumber);
// 	//resets the device
// 	void Reset();
// 	//saves the TWI address to a location in the EEPROM memory
// 	void SaveTWIAddr(uint8_t addrEeprom);
// 	//saves the baud rate value to a location in the EEPROM memory
// 	uint8_t SaveBR(uint8_t baudRate);
// 	//programs a character table into LCD
// 	uint8_t CharsToLcd(uint8_t charTable);
// 	//saves a RAM character table to EEPROM
// 	uint8_t SaveRamtoEeprom(uint8_t charTable);
// 	//loads a character table from EEPROM into RAM memory
// 	uint8_t LdEepromToRam(uint8_t charTable);
// 	//saves the communication mode to EEPROM
// 	uint8_t SaveCommToEeprom(uint8_t commSel);
// 	//enables the write operation to EEPROM memory
// 	void EepromWrEn();
// 	//saves the cursor mode into the EEPROM memory
// 	uint8_t SaveCursorToEeprom(uint8_t modeCrs);
// 	//saves the display mode into the EEPROM memory
// 	uint8_t SaveDisplayToEeprom(uint8_t modeDisp);
// 	//defines a character in the memory positioned at a specified location
// 	uint8_t DefineUserChar(uint8_t* strUserDef, uint8_t charPos);
// 	//displays a user defined char
// 	uint8_t DispUserChar(uint8_t* charPos, uint8_t charNumber, uint8_t idxRow, uint8_t idxCol);
// 	//sets the position of the cursor
// 	uint8_t SetPos(uint8_t idxRow, uint8_t idxCol);
// 	//builds the array format to be sent to the LCD
// 	void BuildUserDefChar(uint8_t* strUserDef, char* cmdStr);
//   private:
// 	//sends a character or a string of characters through SPI
// 	void SendBytes(unsigned char* dataChars, uint8_t lenght);
// 	uint8_t m_SSPin;
// 	uint8_t m_accessType;
// 	//DSPI *pdspi;
// };


#endif