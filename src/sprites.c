#include "sprites.h"

//Returns pointer to array of sprites from spritesheet
C2D_Sprite *spritesFromSheet(C2D_SpriteSheet *sheet) {
    size_t count = C2D_SpriteSheetCount(*sheet);
    C2D_Sprite *sprites = malloc(sizeof(C2D_Sprite)*count);
    for (size_t i = 0; i < count; i++) {
        C2D_SpriteFromSheet(&sprites[i], *sheet, i);
        C2D_SpriteSetCenter(&sprites[i], 0.5f, 0.5f);
    }
    return sprites;
};

void destroySprites(C2D_Sprite *sprites) {
    free(sprites);
};
