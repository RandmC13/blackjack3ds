#ifndef DECK_H_GUARD
#define DECK_H_GUARD

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef enum Suits {
    SPADES,
    CLUBS,
    DIAMONDS,
    HEARTS
} Suit;

typedef struct Card_s {
    uint8_t rank; //0 = Ace, 1-9 = No. Cards, 10,11,12 = J,Q,K
    Suit suit; //Use enum to define this
} Card;

typedef struct Deck_s {
    Card *cards; //Pointer to array of card objects
    uint8_t size; //Size of deck array (will always be 52 but left as a variable to allow for wackyness)
    uint8_t cardsDealt; //Keep track of where in the array to get the next card
} Deck;

//Function definitions

Deck *generateDeck(uint8_t size);
void destroyDeck(Deck *deck);
void shuffleDeck(Deck *deck);
char *getRank(Card *card);
char *getSuit(Card *card);

#endif
