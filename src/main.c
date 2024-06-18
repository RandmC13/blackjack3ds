#include <3ds.h>
#include <citro2d.h>
#include "3ds/console.h"
#include "3ds/gfx.h"
#include "3ds/services/gspgpu.h"
#include "3ds/svc.h"
#include "c2d/base.h"
#include "c2d/sprite.h"
#include "c2d/spritesheet.h"
#include "deck.h"
#include "hand.h"
#include "main.h"
#include "draw.h"

C2D_SpriteSheet cardsheet;
C2D_SpriteSheet back;
C2D_SpriteSheet tableassets;
C2D_Sprite assets;
float cardHeight;
float cardWidth;

int main(int argc, char **argv)
{
    //Init libs
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    Result rc = romfsInit();
    if (rc) svcBreak(USERBREAK_PANIC);

    //Create screens
    C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    // C3D_RenderTarget *bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    consoleInit(GFX_BOTTOM, NULL);

    //Load graphics
    cardsheet = C2D_SpriteSheetLoad("romfs:/gfx/cardsheet.t3x");
    if (!cardsheet) svcBreak(USERBREAK_PANIC);
    back = C2D_SpriteSheetLoad("romfs:/gfx/back.t3x");
    if (!back) svcBreak(USERBREAK_PANIC);
    tableassets = C2D_SpriteSheetLoad("romfs:/gfx/tableassets.t3x");
    if (!tableassets) svcBreak(USERBREAK_PANIC);

    //Configure table assets
    C2D_SpriteFromSheet(&assets, tableassets, 0);
    C2D_SpriteSetCenter(&assets, 0.5f, 0.5f);
    C2D_SpriteSetPos(&assets, TOP_SCREEN_WIDTH/2, TOP_SCREEN_HEIGHT/2);

    //Define colours
    u32 clrTable = C2D_Color32(53,101,77,255);

    //Create deck from spritesheet
    Deck *deck = generateDeck(52);
    shuffleDeck(deck);

    // Grab card dimensions
    loadCardSprite(&deck->cards[0], &cardsheet);
    cardHeight = deck->cards[0].sprite->params.pos.h;
    cardWidth = deck->cards[0].sprite->params.pos.w;
    unloadCardSprite(&deck->cards[0]);

    //Deal 4 cards and distribute them alternatingly
    Card *startingCards[] = {NULL,NULL,NULL,NULL};
    for (uint8_t i = 0; i < 4; i++) startingCards[i] = dealCard(deck);
    // Create player's hand from deck
    Hand *hand = generateHand(startingCards[0], startingCards[2]);
    //Create dealer's hand from deck
    Hand *dealerHand = generateHand(startingCards[1], startingCards[3]);
    char dealerTurn = 0;

    //Define variables for drawing hands and the deck
    float deckPad = 10.0f;
    float deckOffset = 0.18f;
    float handPadding = 5.0f;
    float dealerPadding = 10.0f;

    //Calculating absolute position for drawing dealer hand next to deck
    float dealerX = deckPad + cardWidth + (deckOffset*deck->size) + dealerPadding;
    float dealerY = (deckPad + (deckOffset * deck->size) + 2.0f); // HACK: Don't ask about the +2 it just works

    // Main loop
    while (aptMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break;
        if (kDown & KEY_A && hand->size <= 12) addCardToHand(hand, dealCard(deck));
        if (kDown & KEY_B && dealerHand->size <= 12) {
            dealerTurn = !dealerTurn;
        }

        printf("\x1b[15;6HDealer's score Is: %d", dealerHand->total);
        printf("\x1b[16;6HPlayer's score Is: %d", hand->total);
        //Begin a frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        //Clear screens
        C2D_TargetClear(top, clrTable);
        // C2D_TargetClear(bottom, clrTable);

        //Draw top screen
        C2D_SceneBegin(top);
        //Draw table assets
        C2D_DrawSprite(&assets);

        //Draw cards
        drawDeckPile(deck, &back, deckOffset, deckPad, deckPad);
        drawHand(hand, &cardsheet, handPadding);
        drawDealerHand(dealerHand, &cardsheet, &back, dealerTurn, dealerX, dealerY); //Draw dealer's hand in line with the deck for aestheticness

        //Draw bottom screen
        // C2D_SceneBegin(bottom);
        //End frame
        C3D_FrameEnd(0);
    }

    //Destroy graphics
    C2D_SpriteSheetFree(cardsheet);
    C2D_SpriteSheetFree(back);
    C2D_SpriteSheetFree(tableassets);

    //Destroy hands
    destroyHand(hand);
    destroyHand(dealerHand);

    //Destroy deck
    destroyDeck(deck);

    C3D_Fini();
    C2D_Fini();
    gfxExit();
    return 0;
}
