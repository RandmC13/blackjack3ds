#include "hand.h"
#include <stdint.h>

void updateHandTotal(Hand *hand) {
    //Loop through cards in hand and add up total
    uint8_t total = 0;
    uint8_t numofaces = 0;

    for (uint8_t i = 0; i < hand->size; i++) {
        uint8_t rank = hand->cards[i]->rank;
        if (rank == 0) {
            total += 11;
            numofaces++;
        }else if (rank > 0 && rank < 10) {
            total += rank+1;
        }else if (rank >= 10) {
            total += 10;
        }
    };
    //If total is bust then try 1 for aces
    for (uint8_t i = 0; i < numofaces; i++) {
        if (total <= 21) break;
        total -= 10;
    };

    hand->total = total;
};

Hand *generateHand(Card *card1, Card *card2) {
    Hand *hand = malloc(sizeof(Hand));
    if (hand == NULL) return NULL;

    hand->size = 2;
    hand->cards = malloc(sizeof(Card*)*hand->size);
    if (hand->cards == NULL) return NULL;

    hand->cards[0] = card1;
    hand->cards[1] = card2;

    updateHandTotal(hand);

    return hand;
};

void destroyHand(Hand *hand) {
    free(hand->cards); // NOTE: Should be okay to not free individual card pointers as they are freed when the deck is destroyed
    free(hand);
};

//Reallocate memory for the cards array as the hand grows
void addCardToHand(Hand *hand, Card *card) {
    hand->size++;
    hand->cards = realloc(hand->cards, sizeof(Card*)*hand->size);
    if (hand->cards == NULL) return;

    hand->cards[hand->size-1] = card;

    updateHandTotal(hand);
};
