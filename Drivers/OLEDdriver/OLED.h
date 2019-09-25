
#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200
#define MAX_PAGES 0x8
#define MAX_COLUMNS 0x80
#define MAX_ROWS 0x40
#define FONTSIZE 8

void OLEDInit(void);

void OLEDReset(void);
void OLEDHome(void);
void OLEDGotoLine(uint8_t line);
void OLEDGotoColumn(uint8_t column);
void OLEDClearLine(uint8_t line);
void OLEDClearAll(void);
void OLEDClearColumn(void);
void OLEDPos(void);
void OLEDPrint(unsigned char);

void write_c(uint8_t command);	// write commands to OLED
void write_d(char data);		// write data to OLED