/** @file
  This sample application bases on HelloWorld PCD setting
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/Timer.h>
#include <Library/UefiBootServicesTableLib.h>

#define REFRESH_PERIOD 20
#define CLICK_RATE 1000
#define FIXED_POINT_POS 1000
#define RANDOM_MESSAGE_COUNT 10

#define GRANDMA_RATE 200
#define GRANDMA_COST 50000
#define DIGGER_RATE 500
#define DIGGER_COST 100000
#define DOWNLOAD_RATE 1000
#define DOWNLOAD_COST 150000
#define DESO_RATE 0
#define DESO_COST 1000000
#define NETHERITE_RATE 5000
#define NETHERITE_COST 500000
#define BLAHAJ_RATE 15000
#define BLAHAJ_COST 1000000

struct gameState {
    long cookies;
    int grandmaCount;
    int diggerCount;
    int downloadCount;
    int desoCount;
    int netheriteCount;
    int blahajCount;
};

void moveCursorTo(int x, int y) {
  gST->ConOut->SetCursorPosition(gST->ConOut, x - 1, y - 1);
}

void setColour(int colour) {
  gST->ConOut->SetAttribute (gST->ConOut, colour);
}

void resetColour() {
  setColour (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
}

void printAtWithColour(const CHAR16 *format, int colour, int x, int y) {
  setColour (colour);
  moveCursorTo (x, y);
  Print (format);
  resetColour();
}

void drawSidebar() {
  moveCursorTo(1, 1);

  setColour(EFI_WHITE | EFI_BACKGROUND_BROWN);
  Print(L"                            \n");
  Print(L"         EFI Clicker        \n");
  Print(L"                            \n");
  setColour(EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY);
  Print(L"                            \n");
  Print(L"                    Cookies \n");
  Print(L"                            \n");
  setColour(EFI_BROWN | EFI_BACKGROUND_BLACK);
  Print(L"                            \n");
  Print(L"         , - ~ - ,          \n");
  Print(L"     , '           ' ,      \n");
  Print(L"   ,     #    £        ,    \n");
  Print(L"  ,                     ,   \n");
  Print(L" ,          ,     &      ,  \n");
  Print(L" ,     [                 ,  \n");
  Print(L" ,             @       ` ,  \n");
  Print(L"  ,      ]              ,   \n");
  Print(L"   ,             ;     ,    \n");
  Print(L"     ,     =        , '     \n");
  Print(L"       ' - , _ ,  '         \n");
  Print(L"                            \n");
  setColour(EFI_WHITE | EFI_BACKGROUND_BROWN);
  Print(L"                            \n");
  Print(L" [Space] to click on cookie \n");
  Print(L" [1-6] to buy a property    \n");
  Print(L" [End] to resume boot       \n");
  setColour (EFI_BLACK | EFI_BACKGROUND_CYAN);
  Print(L" Shareware version of EFI Clicker - Learn more at https://compsoc.dev/efi      \n");
  resetColour ();
}

void drawMainBar() {
  printAtWithColour(L"Grandmas", EFI_WHITE, 38, 1);
  printAtWithColour(L"Digger", EFI_WHITE, 38, 5);
  printAtWithColour(L"Web2.0", EFI_WHITE, 38, 9);
  printAtWithColour(L"1000 DeSo Cookies", EFI_WHITE, 38, 13);
  printAtWithColour(L"Netherite Pickaxe", EFI_WHITE, 38, 17);
  printAtWithColour(L"Blåhaj", EFI_WHITE, 38, 21);

  printAtWithColour (L"[]/", EFI_RED, 32, 6);
  printAtWithColour (L"/|", EFI_RED, 35, 5);
  printAtWithColour (L"[ $ ]", EFI_GREEN, 32, 14);
  printAtWithColour (L"   ", EFI_BACKGROUND_BROWN, 33, 17);
  printAtWithColour (L" ", EFI_BACKGROUND_BROWN, 36, 18);

  printAtWithColour (L" /\\_ ", EFI_LIGHTGRAY, 32, 21);
  printAtWithColour (L"~~~~~", EFI_CYAN | EFI_BACKGROUND_BLUE, 32, 22);
  printAtWithColour (L"     ", EFI_BACKGROUND_BLUE, 32, 23);
}


void layoutInitialise() {
  gST->ConOut->EnableCursor(gST->ConOut, 0);

  Print(L"                             1   O   Grandmas - Unemployed thanks to Greggs\n");
  Print(L"                                -|-# Costs %d (%d millicookies/s)\n", GRANDMA_COST / FIXED_POINT_POS, GRANDMA_RATE);
  Print(L"                                / \\  Owned £££££ (£££££ cookies/s)\n");
  Print(L"\n");
  Print(L"                             2    /| Digger - Dig to Earth's molten cookie core\n");
  Print(L"                               []/ O Costs %d (%d millicookies/s)\n", DIGGER_COST / FIXED_POINT_POS, DIGGER_RATE);
  Print(L"                               ###   Owned £££££ (£££££ cookies/s)\n");
  Print(L"\n");
  Print(L"                             3|FIRE| Web2.0 - Now with EU Cookie Pop-ups!\n");
  Print(L"                              |FOX | Costs %d (%d millicookies/s)\n", DOWNLOAD_COST / FIXED_POINT_POS, DOWNLOAD_RATE);
  Print(L"                             /####/  Owned £££££ (£££££ cookies/s)\n");
  Print(L"\n");
  Print(L"                             4       %d DeSo Cookies - 100%% of interviewed\n", DESO_COST / FIXED_POINT_POS);
  Print(L"                               [ $ ] MLH and HUK reps have no comment on Web3.0\n");
  Print(L"                                     You own £££££ worthless tokens.\n");
  Print(L"\n");
  Print(L"                             5  ===  Netherite Pickaxe with Fortune III\n");
  Print(L"                                 / | Costs %d (%d cookies/s)\n", NETHERITE_COST / FIXED_POINT_POS, NETHERITE_RATE / FIXED_POINT_POS);
  Print(L"                                /    Owned £££££ (£££££ cookies/s)\n");
  Print(L"\n");
  Print(L"                             6       Blåhaj\n");
  Print(L"                                     Costs %d (%d cookie/s)\n", BLAHAJ_COST / FIXED_POINT_POS, BLAHAJ_RATE / FIXED_POINT_POS);
  Print(L"                                     Owned £££££ (£££££ cookie/s)\n");
  Print(L"\n");

  drawSidebar ();
  drawMainBar();
}

void clearScreen() {
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  gST->ConOut->ClearScreen(gST->ConOut);
}

void drawCount(int x, int y, int count, int rate) {
  gST->ConOut->SetCursorPosition(gST->ConOut, x, y);
  Print (L"%5d", count);
  gST->ConOut->SetCursorPosition(gST->ConOut, x + 7, y);
  Print (L"%5d", count * rate / FIXED_POINT_POS);
}

void redrawScreen(struct gameState *state) {
  gST->ConOut->SetAttribute (gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLUE);

  // Draw global count
  moveCursorTo (2, 5);
  Print (L"%18d", state->cookies / FIXED_POINT_POS);

  drawCount (43, 2, state->grandmaCount, GRANDMA_RATE);
  drawCount (43, 6, state->diggerCount, DIGGER_RATE);
  drawCount (43, 10, state->downloadCount, DOWNLOAD_RATE);

  gST->ConOut->SetCursorPosition(gST->ConOut, 45, 14);
  Print (L"%5d", state->desoCount);

  drawCount (43, 18, state->netheriteCount, NETHERITE_RATE);
  drawCount (43, 22, state->blahajCount, BLAHAJ_RATE);

  gST->ConOut->SetAttribute (gST->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
}

/**
 * Calculate the multiplier every time the player
 * presses the spacebar
 * @param state The game state
 */
void spacebarPressed(struct gameState *state) {
  int multiplier = CLICK_RATE;
  state->cookies += multiplier;
}

/**
 * Calculate the new number of cookies the player
 * should have every game tick
 * @param state The game state
 */
void tick(struct gameState *state) {
  long sum = 0;
  sum += state->grandmaCount * GRANDMA_RATE;
  sum += state->diggerCount * DIGGER_RATE;
  sum += state->downloadCount * DOWNLOAD_RATE;
  sum += state->desoCount * DESO_RATE;
  sum += state->netheriteCount * NETHERITE_RATE;
  sum += state->blahajCount * BLAHAJ_RATE;
  sum *= REFRESH_PERIOD;
  sum /= 1000;
  state->cookies += sum;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS EFIAPI
UefiMain (IN
          EFI_HANDLE ImageHandle, IN
          EFI_SYSTEM_TABLE *SystemTable) {
  UINT32 Index;
  EFI_STATUS Status;
  EFI_EVENT screenUpdateLoop;
  EFI_INPUT_KEY key;
  VOID *Interface;

  key.ScanCode = SCAN_NULL;

  struct gameState state;
  state.cookies = 0;
  state.grandmaCount = 0;
  state.diggerCount = 0;
  state.downloadCount = 0;
  state.desoCount = 0;
  state.netheriteCount = 0;
  state.blahajCount = 0;

  int randomMessage = 0;

  // Clear the screen
  clearScreen();

  gBS->CreateEvent (
      EVT_TIMER,
      0,
      NULL,
      NULL,
      &screenUpdateLoop
  );
  gBS->SetTimer (
      screenUpdateLoop,
      TimerPeriodic,
      EFI_TIMER_PERIOD_MILLISECONDS(REFRESH_PERIOD)
  );

  EFI_EVENT events[2];
  events[0] = gST->ConIn->WaitForKey;
  events[1] = screenUpdateLoop;

  layoutInitialise();

  int eventType;
  do {
    gBS->WaitForEvent(2, events, &eventType);

    randomMessage = (randomMessage + 1) % RANDOM_MESSAGE_COUNT;

    if (eventType == 0) {
      gST->ConIn->ReadKeyStroke(gST->ConIn, &key);

      if (key.UnicodeChar == ' ') {
        spacebarPressed(&state);
      } else if (key.UnicodeChar == '1') {
        if (state.cookies >= GRANDMA_COST) {
          state.cookies -= GRANDMA_COST;
          state.grandmaCount++;
        }
      } else if (key.UnicodeChar == '2') {
        if (state.cookies >= DIGGER_COST) {
          state.cookies -= DIGGER_COST;
          state.diggerCount++;
        }
      } else if (key.UnicodeChar == '3') {
        if (state.cookies >= DOWNLOAD_COST) {
          state.cookies -= DOWNLOAD_COST;
          state.downloadCount++;
        }
      } else if (key.UnicodeChar == '4') {
        if (state.cookies >= DESO_COST) {
          state.cookies -= DESO_COST;
          state.desoCount++;
        }
      } else if (key.UnicodeChar == '5') {
        if (state.cookies >= NETHERITE_COST) {
          state.cookies -= NETHERITE_COST;
          state.netheriteCount++;
        }
      } else if (key.UnicodeChar == '6') {
        if (state.cookies >= BLAHAJ_COST) {
          state.cookies -= BLAHAJ_COST;
          state.blahajCount++;
        }
      }
    } else if (eventType == 1) {
      // Perform a tick, and redraw the screen
      tick (&state);
      redrawScreen (&state);
    }
  } while (key.ScanCode != SCAN_END);

  clearScreen();

  if (randomMessage == 0) Print(L"You accidentally tripped over the END button and died\n");
  if (randomMessage == 1) Print(L"Sir Lord Sugar releases the E-Mailer 4000\n");
  if (randomMessage == 2) Print(L"You decide to get into the Cupcake industry\n");
  if (randomMessage == 3) Print(L"You stopped wasting your time and got back to work\n");
  if (randomMessage == 4) Print(L"You question the integrity of the presentation statements\n");
  if (randomMessage == 5) Print(L"You decide to work for HackathonsUK instead of wasting time\n");
  if (randomMessage == 6) Print(L"You decide to work for Major League Hacking instead of wasting time\n");
  if (randomMessage == 7) Print(L"The government uncovers cookie-tax-evasion scandal; Go straight to jail; Do not pass GO; Do not collect 200 Cookies\n");
  if (randomMessage == 8) Print(L"You decide to convert the company into a co-operative\n");
  if (randomMessage == 9) Print(L"You sell your remaining stocks of Cookie Ltd.\n");

  gST->ConOut->EnableCursor(gST->ConOut, 1);
  return EFI_SUCCESS;
}
