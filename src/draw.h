#ifndef DRAW_H_GUARD
#define DRAW_H_GUARD

#include <citro2d.h>
#include <stdlib.h>
#include "deck.h"
#include "hand.h"

void loadCardSprite(Card *card, C2D_SpriteSheet *sheet);
void unloadCardSprite(Card *card);
void drawHand(Hand *hand, C2D_SpriteSheet *sheet, float padY);
void drawDealerHand(Hand *hand, C2D_SpriteSheet *cardsheet, C2D_SpriteSheet *backsheet, char dealerTurn, float padX, float padY);
void drawDeckPile(Deck *deck, C2D_SpriteSheet *decksheet, float offset, float padX, float padY);

#endif
