#include "deck.h"
#include "c2d/sprite.h"
#include "draw.h"
#include <stdint.h>

Deck *generateDeck(uint8_t size) {
    Deck *deck = malloc(sizeof(Deck));
    if (deck == NULL) return NULL;

    deck->size = size;
    deck->cardsDealt = (uint8_t)0;

    deck->cards = malloc(sizeof(Card)*size);
    if (deck->cards == NULL) return NULL;

    // WARN: FOR THE TIME BEING, THIS CODE ASSUMES THE DECK IS 52 CARDS
    
    //Populate deck with cards in order
    uint8_t i = 0;
    for (Suit suit=SPADES; suit <= HEARTS; suit++) {
        for (uint8_t rank=0; rank < 13; rank++) {
            deck->cards[i].suit = suit;
            deck->cards[i].rank = rank;
            deck->cards[i].sprite = NULL;
            deck->cards[i].spriteindex = ((13 * suit) + rank);

            i++;
        }
    }

    return deck;
};

void destroyDeck(Deck *deck) {
    for (uint8_t i = 0; i < deck->size; i++) unloadCardSprite(&deck->cards[i]);
    free(deck->cards);
    free(deck);
};


void shuffleDeck(Deck *deck) {
    //Fisher-Yates Shuffle
    srand(time(NULL)); //Seed random num gen
    for (uint8_t i = 0; i < (deck->size)-1; i++) {
        int r = rand() % (deck->size - i) + i; // i <= r < n
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[r];
        deck->cards[r] = temp;
    }
}

Card *dealCard(Deck *deck) {
    Card *card = &deck->cards[deck->cardsDealt];
    deck->cardsDealt++;
    if (deck->cardsDealt > deck->size) exit(1); //Should never happen just to prevent something going horribly wrong
    return card;
};

char *getSuit(Card *card) {
    switch (card->suit) {
    case SPADES:
        return "Spades";
    case CLUBS:
        return "Clubs";
    case DIAMONDS:
        return "Diamonds";
    case HEARTS:
        return "Hearts";
    default:
        return NULL;
    }
};

char *getRank(Card *card) {
    char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    return ranks[card->rank];
};
