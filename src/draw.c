#include "draw.h"
#include "c2d/sprite.h"
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

void drawHand(Hand *hand, C2D_SpriteSheet *sheet, float x, float y) {
    uint8_t cardWidth = hand->cards[0]->sprite->params.pos.w;

    for (uint8_t i = 0; i < hand->size; i++) {
        loadCardSprite(hand->cards[i], sheet);
        C2D_SpriteSetPos(hand->cards[i]->sprite, (x + ((float)i * (float)cardWidth) / 2), y);
        C2D_DrawSprite(hand->cards[i]->sprite);
    };
};
