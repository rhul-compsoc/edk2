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

#define REFRESH_PERIOD 1000

#define CLICK_RATE 1000
#define GRANDMA_RATE 200
#define GRANDMA_COST 50000
#define DIGGER_RATE 500
#define DIGGER_COST 100000
#define DOWNLOAD_RATE 1000
#define DOWNLOAD_COST 150000
#define DESO_RATE 0
#define DESO_COST 1000000

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

struct gameState {
    long cookies;
    int grandmaCount;
    int diggerCount;
    int downloadCount;
    int desoCount;
};

void layoutInitialise(struct gameState *state) {
   Print(L"  EFI Clicker (Registered)  |1  O   Grandma @ %d millicookies/s\n", GRANDMA_RATE);
   Print(L"----------------------------|  -|-# Costs %d\n", GRANDMA_COST / 1000);
   Print(L"                            |  / \\  Owned %d (%d cookies/s)\n", state->grandmaCount, state->grandmaCount * GRANDMA_RATE / 1000);
   Print(L"%19d Cookies | ------------------------------------------------\n", state->cookies / 1000);
   Print(L"                            |2   /| Digger @ %d millicookies/s\n", DIGGER_RATE);
   Print(L"----------------------------| []/ O Costs %d\n", DIGGER_COST / 1000);
   Print(L"                            | ###   Owned %d (%d cookies/s)\n", state->diggerCount, state->diggerCount * DIGGER_RATE / 1000);
   Print(L"         , - ~ - ,          | ------------------------------------------------\n");
   Print(L"     , '           ' ,      |3  A   'Download Cookies' @ %d mbit/s\n", DOWNLOAD_RATE);
   Print(L"   ,     #    £        ,    |  /X\\  Costs %d\n", DOWNLOAD_COST / 1000);
   Print(L"  ,                     ,   | /XXX\\ Owned %d (%d cookies/s)\n", state->downloadCount, state->downloadCount * DOWNLOAD_RATE / 1000);
   Print(L" ,          ,     &      ,  | ------------------------------------------------\n");
   Print(L" ,     [                 ,  |4 /-\\  DESO Coins @ %d cookies/s\n", DESO_RATE / 1000);
   Print(L" ,             @       ` ,  | | $ |  Costs %d\n", DESO_COST / 1000);
   Print(L"  ,      ]              ,   |  \\-/  Owned %d (%d cookies/s)\n", state->desoCount, state->desoCount * DESO_RATE / 1000);
   Print(L"   ,             ;     ,    |                \n");
   Print(L"     ,     =        , '     |                \n");
   Print(L"       ' - , _ ,  '         |                \n");
   Print(L"                            |                \n");
   Print(L"                            |                \n");
   Print(L"   Press [SPACE] to click   |                \n");
   Print(L"                            |                \n");
   Print(L"                            |                \n");
   Print(L"                            |abcdefghijklmnop\n");
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
  key.ScanCode = SCAN_NULL;

  struct gameState state;
  state.cookies = 0;
  state.grandmaCount = 0;
  state.diggerCount = 0;
  state.downloadCount = 0;
  state.desoCount = 0;

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

  int eventType;
  int running = 1;
  do {
    gBS->WaitForEvent(2, events, &eventType);

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
      }
    } else if (eventType == 1) {
      // Perform a tick, and redraw the screen
      tick (&state);
      layoutInitialise(&state);
    }
  } while (key.ScanCode != SCAN_END);

  return EFI_SUCCESS;
}
