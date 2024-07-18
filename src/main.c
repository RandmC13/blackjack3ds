#include <3ds.h>
#include <citro2d.h>
#include "3ds/console.h"
#include "3ds/gfx.h"
#include "3ds/services/gspgpu.h"
#include "3ds/svc.h"
#include "c2d/base.h"
#include "c2d/sprite.h"
#include "c2d/spritesheet.h"
#include "c2d/text.h"
#include "deck.h"
#include "hand.h"
#include "main.h"
#include "draw.h"

//Variables for graphics
C2D_SpriteSheet cardsheet;
C2D_SpriteSheet back;
C2D_SpriteSheet tableassets;
C2D_SpriteSheet topui;
C2D_Sprite assets;
C2D_Sprite topuiSprite;

//Text objects
C2D_TextBuf scoresBuf;
C2D_Text scoresText[2]; //Order is player then dealer

//Hand objects
Hand *hand;
Hand *dealerHand;

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

    //Load graphics
    cardsheet = C2D_SpriteSheetLoad("romfs:/gfx/cardsheet.t3x");
    if (!cardsheet) svcBreak(USERBREAK_PANIC);
    back = C2D_SpriteSheetLoad("romfs:/gfx/back.t3x");
    if (!back) svcBreak(USERBREAK_PANIC);
    tableassets = C2D_SpriteSheetLoad("romfs:/gfx/tableassets.t3x");
    if (!tableassets) svcBreak(USERBREAK_PANIC);
    topui = C2D_SpriteSheetLoad("romfs:/gfx/topui.t3x");
    if (!topui) svcBreak(USERBREAK_PANIC);

    //Configure table assets
    C2D_SpriteFromSheet(&assets, tableassets, 0);
    C2D_SpriteSetCenter(&assets, 0.5f, 0.5f);
    C2D_SpriteSetPos(&assets, TOP_SCREEN_WIDTH/2, TOP_SCREEN_HEIGHT/2);
    //Configure ui elements
    C2D_SpriteFromSheet(&topuiSprite, topui, 0);
    C2D_SpriteSetCenter(&topuiSprite, 0.0f, 0.0f);
    char topuiWidth = topuiSprite.params.pos.w;
    char topuiHeight = topuiSprite.params.pos.h;

    //Define colours
    u32 clrTable = C2D_Color32(53,101,77,255);

    //Generate text buffers
    scoresBuf = C2D_TextBufNew(19);

    //Create deck from spritesheet and shuffle it
    Deck *deck = generateDeck(52);
    shuffleDeck(deck);

    // Grab card dimensions
    loadCardSprite(&deck->cards[0], &cardsheet);
    cardHeight = deck->cards[0].sprite->params.pos.h;
    cardWidth = deck->cards[0].sprite->params.pos.w;
    unloadCardSprite(&deck->cards[0]);

    //Define variables for game logic
    char gameState; //1 = Game ongoing, 0 = no running game
    char gameTurn; //0 = player's turn, 1 = dealer's turn, 2 = calculate result, 3 = display result
    char dealerCounter = 0; //Time between dealer's moves
    char gameOutcome = 0; //0 = player wins, 1 = dealer wins, 2 = push

    //Prepare variables for a new game
    prepareGame(deck, &hand, &dealerHand, &gameState, &gameTurn);

    //If player's starting hand is blackjack then cut straight to dealer's turn
    if (hand->total == 21) gameTurn = 1;

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

        // TODO: Add animations for drawing cards and such
        // - Animation for turning over dealer's face down card?
        //
        // TODO: Add extra rules like splitting, double down, insurance, etc.
        //
        if (kDown & KEY_START) break;

        //--- GAME LOGIC ---//

        switch (gameState) {
            case 1:
                //Game running
                //Identify who's turn it is
                switch (gameTurn) {
                    case 0:
                        //Player's turn
                        gameTurn = playerTurn(kDown, hand, deck);
                        break;
                    case 1:
                        //Dealer's turn
                        if (dealerTurn(&dealerCounter, dealerHand, deck)) gameTurn = 2;
                        break;
                    case 2:
                        //Determine the result
                        gameOutcome = getResult(hand, dealerHand);
                        //Instantly move to next phase in displaying results
                        gameTurn = 3;
                        break;
                    case 3:
                        //Display result stage
                        //A = New game, B = Clear Table
                        if (kDown & KEY_A) {
                            //Prepare variables for a new game
                            destroyHand(hand);
                            destroyHand(dealerHand);
                            resetDeck(deck);
                            prepareGame(deck, &hand, &dealerHand, &gameState, &gameTurn);
                        }
                        if (kDown & KEY_B) gameState = 0;
                        break;
                }
                break;
            case 0:
                //A = new game
                if (kDown & KEY_A) {
                    //Prepare variables for a new game
                    destroyHand(hand);
                    destroyHand(dealerHand);
                    resetDeck(deck);
                    prepareGame(deck, &hand, &dealerHand, &gameState, &gameTurn);
                }
                //No game running
                break;
    }

    //--- DRAWING GRAPHICS ---//

        //Begin a frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        //Clear screens
        C2D_TargetClear(top, clrTable);
        // C2D_TargetClear(bottom, clrTable);

        //Draw top screen
        C2D_SceneBegin(top);
        //Draw table markings
        C2D_DrawSprite(&assets);

        //Draw cards
        drawDeckPile(deck, &back, deckOffset, deckPad, deckPad);
        //Draw hands and score if game is ongoing
        if (gameState) {
            drawHand(hand, &cardsheet, handPadding);
            drawDealerHand(dealerHand, &cardsheet, &back, gameTurn, dealerX, dealerY); //Draw dealer's hand in line with the deck for aestheticness
            //Draw player's score
            drawScore(&scoresText[0], &scoresBuf, hand->total, 0, 2.0f, 83.0f, &topuiSprite, topuiWidth, topuiHeight);
            //Draw dealer's score
            if (gameTurn) drawScore(&scoresText[1], &scoresBuf, dealerHand->total , 1, 2.0f, 83.0f, &topuiSprite, topuiWidth, topuiHeight);
        };

        //Draw bottom screen
        // C2D_SceneBegin(bottom);
        // TODO: Add chip pile at bottom screen that changes in size with wins/losses (have accurate chip colour denominations)
        // TODO: Add betting area where chips are moved when a bet is placed

        //End frame
        C3D_FrameEnd(0);
    }

    //Destroy graphics
    C2D_SpriteSheetFree(cardsheet);
    C2D_SpriteSheetFree(back);
    C2D_SpriteSheetFree(tableassets);
    C2D_SpriteSheetFree(topui);
    //Destroy text
    C2D_TextBufDelete(scoresBuf);

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

//--- Game Related Functions ---//

//Reset variables for a new game
void prepareGame(Deck *deck, Hand **playerHand, Hand **dealerHand, char *gameState, char *gameTurn) {
    //Deal 4 cards and distribute them alternatingly
    Card *startingCards[] = {NULL,NULL,NULL,NULL};
    for (uint8_t i = 0; i < 4; i++) startingCards[i] = dealCard(deck);

    // Create player's hand from deck
    *playerHand = generateHand(startingCards[0], startingCards[2]);
    //Create dealer's hand from deck
    *dealerHand = generateHand(startingCards[1], startingCards[3]);

    //Set variables
    *gameState = 1;
    *gameTurn = 0;
}

//Returns 0 if player's turn continues, 1 if it should move to the dealer or 2 if the player instantly loses (they go bust)
char playerTurn(u32 keys, Hand *playerhand, Deck *deck) {
    // TODO: Add double down and splitting
    //
    //A = Hit, B = stand

    //Check if player is bust (or has blackjack)
    if (playerhand->total == 21) return 1;
    if (playerhand->total > 21) return 2;

    //If player wants to stand end the turn
    if (keys & KEY_B) return 1;

    if (keys & KEY_A) addCardToHand(playerhand, dealCard(deck));

    //Move turn forwards
    return 0;
}

//Returns 0 if dealer's turn continues or 1 if it must end
char dealerTurn(char *counter, Hand *dealerHand, Deck *deck) {
    //If dealer has 17 or greater they must stand
    if (dealerHand->total >= 17) return 1;

    //Only deal card once counter is high enough (adds gap between cards being dealt)
    if (*counter >= 50) {
        //Deal card
        addCardToHand(dealerHand, dealCard(deck));
        *counter = 0;
        return 0;
    }
    (*counter)++;
    return 0;
}

//Returns 0 if player wins, 1 if dealer wins and 2 if its a push
char getResult(Hand *playerHand, Hand *dealerHand) {
    //If player is bust they instantly lose
    if (playerHand->total > 21) return 1;

    // WARN: This needs to be changed eventually as real blackjack doesn't work like this
    // If player has 21 they win
    if (playerHand->total == 21) return 0;
    //if dealer is bust then player wins
    if (dealerHand->total > 21) return 0;
    //Check for a push
    if (playerHand->total == dealerHand->total) return 2;
    //Check if player has the bigger hand
    if (playerHand->total > dealerHand->total) return 0;
    //If flow has reached this far the dealer's hand must be better
    return 1;
}
