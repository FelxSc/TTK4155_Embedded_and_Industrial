
const char repeatSection[11] = {4,4,1,3,1,1,1,3,1,1,4}

const uint16_t PROGMEM GameOfThronesTime[][] = {
	{500, 500, 250, 250, 1},		// reapeat 4
	{500, 500, 250, 250, 1},		// reapeat 4
	{500, 500, 250, 250, 500},		// reapeat 1
	{500, 250, 250, 500, 1},		// reapeat 3
	{500, 250, 250, 1000, 1000},	// reapeat 1
	{1000, 250, 250, 1000, 1000},	// reapeat 1
	{250, 250, 500, 500, 1},		// reapeat 1
	{250, 250, 500, 1, 	1},			// reapeat 3
	{1000, 1000, 250, 250, 1000},	// reapeat 1
	{1000, 250, 250, 500, 1},		// reapeat 1
	{500, 250, 250, 500, 1},		// reapeat 3
}

const uint16_t PROGMEM GameOfThronesNote[][] = {
	{NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_F4},
	{NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_F4},
	{NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_D4},
	{NOTE_G3, NOTE_AS3,	NOTE_C4, NOTE_D4, NOTE_D4},
	
	{NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_F4},
	{NOTE_AS3, NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3},
	{NOTE_DS4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_C4},
	
	{NOTE_GS3, NOTE_AS3, NOTE_C4, NOTE_F3, NOTE_F3},
	{NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4},
	{NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_D4, NOTE_D4},
	{NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_D4}

}

const char repeatSection[1] = {1}

void ShootSound(void)
{
	enableBuzzer();
	for(int i = 5000; i > 4900; i -=10)
	{
		setBuzzerFrequency(i)
	}
	disableBuzzer();
}