#ifndef MENU_H
#define MENU_H

#include <stdbool.h>



typedef enum{
	MAIN_MENU, PLAYGAME, OPTIONS, HIGSCORES, DIFFICULTY, BRIGHTNESS, TEST1, TEST2, TEST3, TEST4
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
selectedMenu_t selectMenu();
void drawMenu(menu_t* menu);


#endif // MENU_H