#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdlib.h>



typedef enum{
	MAIN_MENU, GAME, OPTIONS, GRAPHICS, MUSIC, PLAYGAME, DIFFICULTY, EASY, NORMAL, HARD, HIGHSCORE, BRIGHTNESS, CHANGEFONT, PIDTUNER, MOTORSPEED, CREDITS, RESETSCORE, ZELDA
} selectedMenu_t;

// Menu structure as the LeftChild-RightSibling (LCRS)
typedef struct{
	char* title;
	selectedMenu_t* ID;
	struct menu_t* parent;
	struct menu_t* firstChild;
	struct menu_t* rightSibling;
}menu_t;



void menuInit( void );
void selectMenu( void );
void drawMenu(menu_t* menu);


#endif // MENU_H