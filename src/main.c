#include <3ds.h>
#include <citro2d.h>
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
    C3D_RenderTarget *bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    //Load graphics
    cardsheet = C2D_SpriteSheetLoad("romfs:/gfx/cardsheet.t3x");
    if (!cardsheet) svcBreak(USERBREAK_PANIC);
    back = C2D_SpriteSheetLoad("romfs:/gfx/back.t3x");
    if (!back) svcBreak(USERBREAK_PANIC);

    //Define colours
    u32 clrTable = C2D_Color32(53,101,77,1);

    //Create deck from spritesheet
    Deck *deck = generateDeck(52);
    shuffleDeck(deck);
    // Create hand from deck
    Card *card1 = dealCard(deck);
    Card *card2 = dealCard(deck);
    Hand *hand = generateHand(card1, card2);

    // Main loop
    while (aptMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break;
        if (kDown & KEY_A) addCardToHand(hand, dealCard(deck));

        //Begin a frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        //Clear screens
        C2D_TargetClear(top, clrTable);
        C2D_TargetClear(bottom, clrTable);

        //Draw top screen
        C2D_SceneBegin(top);
        drawDeckPile(deck, &back, (TOP_SCREEN_WIDTH - 50), 50);
        drawHand(hand, &cardsheet, 20.0f);

        //Draw bottom screen
        C2D_SceneBegin(bottom);
        //End frame
        C3D_FrameEnd(0);
    }

    //Destroy graphics
    C2D_SpriteSheetFree(cardsheet);

    //Destroy deck
    destroyDeck(deck);

    //Destroy hands
    destroyHand(hand);

    C3D_Fini();
    C2D_Fini();
    gfxExit();
    return 0;
}
