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
    uint8_t cardWidth = hand->cards[0]->sprite->params.pos.w;
    uint8_t cardHeight = hand->cards[0]->sprite->params.pos.h;
    uint16_t handWidth = ((cardWidth * hand->size) + cardWidth) / 2;
    //x and y positions are the centre of the hand
    uint16_t handX = (TOP_SCREEN_WIDTH - handWidth + cardWidth) / 2;
    uint16_t handY = TOP_SCREEN_HEIGHT - (padY + (cardHeight / 2));

    for (uint8_t i = 0; i < hand->size; i++) {
        loadCardSprite(hand->cards[i], sheet);
        C2D_SpriteSetPos(hand->cards[i]->sprite, (handX + (i * (cardWidth / 2))), handY);
        C2D_DrawSprite(hand->cards[i]->sprite);
    };
};

void drawDeckPile(Deck *deck, C2D_SpriteSheet *backsheet, float x, float y) {
    C2D_Sprite back;
    C2D_SpriteFromSheet(&back, *backsheet, 0);
    C2D_SpriteSetCenter(&back, 0.5f, 0.5f);
    C2D_SpriteSetPos(&back, x, y);
    C2D_SpriteScale(&back, 0.99f, 0.99f);

    float offset = 0.18f;
    for (uint8_t i = 0; i < (deck->size - deck->cardsDealt); i++) {
        C2D_DrawSprite(&back);
        C2D_SpriteMove(&back, offset, -offset);
    }
};
