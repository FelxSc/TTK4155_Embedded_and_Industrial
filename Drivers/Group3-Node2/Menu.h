#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

void menu1( void );
void menu2( void );
void menu3( void );
void menu4( void );
//void printMenu0(void);
void MainMenu(void);

/*
typedef struct menu{
    char *Title; // Pointer to menuTitle
    uint8_t numberOfItems; // Number of functions to choose
    uint8_t Top;	// Pointer to the top og the list
    uint8_t Sel;	// pointer to the selected function
    bool    selectNext; // Select the menu/function chosen in pSel
    bool    selectPrevious; // Select the previous Menu/function
	char *Items[]; // Pointer to item strings
}menu_t;*/

// Menu structure as the LeftChild-RightSibling (LCRS)

typedef struct{
	char* title;
	struct menu_t* parent;
	struct menu_t* firstChild;
	struct menu_t* rightSibling;
}menu_t;

typedef enum{
	MAIN_MENU, MENU1, MENU2, MENU3
	} selectedMenu_t;

void menuInit( void );
selectedMenu_t selectMenu();
void drawMenu(menu_t* menu);



#endif // MENU_H