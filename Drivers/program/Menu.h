#ifndef MENU_H
#define MENU_H


void menu( void );
void menu1( void );
void menu2( void );
void menu3( void );
void menu4( void );
void printMenu0(void);


typedef struct menu{
    const char Title; // Pointer to menuTitle
    const char Items[]; // Pointer to item strings
    uint8_t numberOfItems; // Number of functions to choose
    uint8_t Top;	// Pointer to the top og the list
    uint8_t Sel;	// pointer to the selected function
    bool    selectNext; // Select the menu/function chosen in pSel
    bool    selectPrevious; // Select the previous Menu/function
}menu_t;

#endif // MENU_H