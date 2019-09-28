#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

void menu1( void );
void menu2( void );
void menu3( void );
void menu4( void );
//void printMenu0(void);
void MainMenu(void);


typedef struct menu{
    char *Title; // Pointer to menuTitle
    uint8_t numberOfItems; // Number of functions to choose
    uint8_t Top;	// Pointer to the top og the list
    uint8_t Sel;	// pointer to the selected function
    bool    selectNext; // Select the menu/function chosen in pSel
    bool    selectPrevious; // Select the previous Menu/function
	char *Items[]; // Pointer to item strings
}menu_t;

void selectMenu(menu_t* menu);
void drawMenu(menu_t* menu);

#endif // MENU_H