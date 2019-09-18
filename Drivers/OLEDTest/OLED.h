
#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200

void OledInit(void);

void OledReset(void);
void OledHome(void);
void OledGotoLine(void);
void OledGotoColumn(void);
void OledClearLine(void);
void OledClearLine(void);
void OledPos(void);
void OledPrint(uint8_t*);

void write_c(uint8_t command);	// write commands to OLED
void write_d(uint8_t data);		// write data to OLED