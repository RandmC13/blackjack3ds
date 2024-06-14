#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include "3ds/romfs.h"
#include "3ds/svc.h"
#include "c2d/base.h"
#include "c2d/sprite.h"
#include "c2d/spritesheet.h"
#include "deck.h"
#include "main.h"

C2D_SpriteSheet cardsheet;

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

    //Load graphics
    cardsheet = C2D_SpriteSheetLoad("romfs:/gfx/cardsheet.t3x");
    if (!cardsheet) svcBreak(USERBREAK_PANIC);


    //Define colours
    u32 clrTable = C2D_Color32(53,101,77,1);

    //Create deck from spritesheet
    Deck *deck = generateDeck(52, &cardsheet);
    // shuffleDeck(deck);

    uint8_t cardnum = 0;
    // Main loop
    while (aptMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break;
        if (kDown & KEY_A) cardnum++;
        if (cardnum >= 52) cardnum = 0;

        C2D_SpriteSetPos(&deck->cards[cardnum].sprite, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        //Draw a frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, clrTable);
        C2D_SceneBegin(top);

        C2D_DrawSprite(&deck->cards[cardnum].sprite);

        //End frame
        C3D_FrameEnd(0);
    }

    //Destroy graphics
    C2D_SpriteSheetFree(cardsheet);

    //Destroy deck
    destroyDeck(deck);
    C3D_Fini();
    C2D_Fini();
    gfxExit();
    return 0;
}
