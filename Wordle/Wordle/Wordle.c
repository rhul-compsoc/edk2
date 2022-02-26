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

int running = 1;
int index = 0;

typedef struct {
    UINTN Signature;
    EFI_EVENT PeriodicTimer;
    EFI_EVENT OneShotTimer;
    //
    // Other device specific fields
    //
} EXAMPLE_DEVICE;

VOID
TimerHandler (
    IN EFI_EVENT  Event,
    IN VOID       *Context
)
{
  Print (L"Timer print!");

  index++;
  if (index > 10) {
    gBS->CloseEvent(Event);
  }
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
  EXAMPLE_DEVICE *Device;
  EFI_STATUS Status;

  gBS->CreateEvent (
      EVT_TIMER | EVT_NOTIFY_SIGNAL,  // Type
      TPL_NOTIFY,                     // NotifyTpl
      TimerHandler,                   // NotifyFunction
      Device,                         // NotifyContext
      &Device->PeriodicTimer          // Event
  );
  gBS->SetTimer (
      Device->PeriodicTimer,
      TimerPeriodic,
      EFI_TIMER_PERIOD_MILLISECONDS(100)
  );

  // Event loop
  while (running) {}
  return EFI_SUCCESS;
}
