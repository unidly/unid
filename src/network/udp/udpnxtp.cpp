/**
 * @file
 * @brief Returns a local free port number.
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>

 /**
  * @brief Finds and returns an available port number.
  *
  * @param void
  * @returns The port number
  */
unsigned short udpnxtp() {
  static unsigned short lastport = ULPORT;
  Bool inuse = TRUE;
  struct upq *pup;
  int i;

  while (inuse) {
    lastport++;
    if (lastport == 0) lastport = ULPORT;
    inuse = FALSE;
    for (i = 0; !inuse && i < UPPS; i++) {
      pup = &upqs[i];
      inuse = pup->up_valid && pup->up_port == lastport;
    }
  }
  return lastport;
}
