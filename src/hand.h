#ifndef HAND_H_GUARD
#define HAND_H_GUARD

#include "deck.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct Hand_s {
    Card **cards; //Array of pointers to cards in hand
    uint8_t size; //Number of cards in hand
    uint8_t total; //Hand 'score'
} Hand;

Hand *generateHand(Card *card1, Card *card2); //Generate hand instance
void destroyHand(Hand *hand); //Free hand pointers
void addCardToHand(Hand *hand, Card *card); //Add a card ref to a hand
void updateHandTotal(Hand *hand); //Calculate hand total 

#endif
