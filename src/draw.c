#include "draw.h"
#include "c2d/sprite.h"
#include "c2d/spritesheet.h"
#include "main.h"
#include <stdint.h>

void loadCardSprite(Card *card, C2D_SpriteSheet *sheet) {
    if (card->sprite != NULL) return; //Don't fuck with the card if it is already storing data
    card->sprite = malloc(sizeof(C2D_Sprite));
    C2D_SpriteFromSheet(card->sprite, *sheet, card->spriteindex);
    C2D_SpriteSetCenter(card->sprite, 0.5f, 0.5f);
    C2D_SpriteScale(card->sprite, 0.99f, 0.99f);
};

void unloadCardSprite(Card *card) {
    free(card->sprite);
    card->sprite = NULL;
};

void drawHand(Hand *hand, C2D_SpriteSheet *sheet, float padY) {
    loadCardSprite(hand->cards[0], sheet);
    uint8_t cardWidth = hand->cards[0]->sprite->params.pos.w;
    uint8_t cardHeight = hand->cards[0]->sprite->params.pos.h;
    uint16_t handWidth = ( cardWidth * (hand->size + 1)) / 2;
    // x and y positions are the centre of the hand
    // padY measured from the base of the screen
    uint16_t handX = (TOP_SCREEN_WIDTH - handWidth + cardWidth) / 2;
    uint16_t handY = TOP_SCREEN_HEIGHT - (padY + (cardHeight / 2));

    for (uint8_t i = 0; i < hand->size; i++) {
        loadCardSprite(hand->cards[i], sheet);
        C2D_SpriteSetPos(hand->cards[i]->sprite, (handX + (i * (cardWidth / 2))), handY);
        C2D_DrawSprite(hand->cards[i]->sprite);
    };
};

void drawDealerHand(Hand *hand, C2D_SpriteSheet *cardsheet, C2D_SpriteSheet *backsheet, char dealerTurn, float padX, float padY) {
    // padX and padY measured from the top left corner
    loadCardSprite(hand->cards[0], cardsheet);
    uint8_t cardWidth = hand->cards[0]->sprite->params.pos.w;
    uint8_t cardHeight = hand->cards[0]->sprite->params.pos.h;
    // draw one card face down if it is not the dealer's turn yet
    if (dealerTurn) {
        for (uint8_t i = 0; i < hand->size; i++) {
            loadCardSprite(hand->cards[i], cardsheet);
            C2D_SpriteSetPos(hand->cards[i]->sprite, padX + ((cardWidth / 2)*(i+1)), padY + (cardHeight / 2));
            C2D_DrawSprite(hand->cards[i]->sprite);
        }
    } else {
        //Draw face down card
        C2D_Sprite back;
        C2D_SpriteFromSheet(&back, *backsheet, 0);
        C2D_SpriteSetCenter(&back, 0.5f, 0.5f);
        C2D_SpriteSetScale(&back, 0.99f, 0.99f);
        C2D_SpriteSetPos(&back, padX + (cardWidth / 2), padY + (cardHeight / 2));
        C2D_DrawSprite(&back);
        //Draw face up card
        loadCardSprite(hand->cards[1], cardsheet);
        C2D_SpriteSetPos(hand->cards[1]->sprite, padX + cardWidth, padY + (cardHeight / 2));
        C2D_DrawSprite(hand->cards[1]->sprite);
    }
}

void drawDeckPile(Deck *deck, C2D_SpriteSheet *backsheet, float offset, float padX, float padY) {
    C2D_Sprite back;
    C2D_SpriteFromSheet(&back, *backsheet, 0);
    C2D_SpriteSetCenter(&back, 0.5f, 0.5f);
    // padX and padY are from the top left corner and relative to the topmost card
    float x = (back.params.pos.w / 2) + padX;
    float y = padY + (back.params.pos.h / 2) + (offset * deck->size);
    C2D_SpriteSetPos(&back, x, y);
    C2D_SpriteScale(&back, 0.99f, 0.99f);

    for (uint8_t i = 0; i < (deck->size - deck->cardsDealt); i++) {
        C2D_DrawSprite(&back);
        C2D_SpriteMove(&back, offset, -offset);
    }
};
