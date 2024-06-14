#ifndef DRAW_H_GUARD
#define DRAW_H_GUARD

#include <citro2d.h>
#include <stdlib.h>
#include "deck.h"
#include "hand.h"

void loadCardSprite(Card *card, C2D_SpriteSheet *sheet);
void unloadCardSprite(Card *card);
void drawHand(Hand *hand, C2D_SpriteSheet *sheet, float pady);
void drawDeckPile(Deck *deck, C2D_SpriteSheet *decksheet, float x, float y);

#endif
