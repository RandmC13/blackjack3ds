#ifndef MAIN_H_GUARD
#define MAIN_H_GUARD

#include "3ds/types.h"
#include "hand.h"

#define TOP_SCREEN_WIDTH 400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240

char playerTurn(u32 keys, Hand *playerHand, Deck *deck);
char dealerTurn(char *counter, Hand *dealerHand, Deck *deck);
char getResult(Hand *playerHand, Hand *dealerHand);
void prepareGame(Deck *deck, Hand **playerHand, Hand **dealerHand, char *gameState, char *gameTurn);

#endif
