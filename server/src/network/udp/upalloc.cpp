/**
 * @file
 * @brief Allocates a free queue
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ports.h>
#include <proc.h>

/**
 * @brief Allocates a free queue index to the caller.
 *
 * The function searches for a free queue entry, fills in the fields
 * creates a Xinu port to serve as the queue of incoming dtgrams, and
 * returns the endex of the entry to the caller
 *
 * @param void
 * @returns The index of the entry to the caller
 */
int upalloc(void) {
  struct upq *pup;
  int i;

  wait(udpmutex);
  for (i = 0; i < UPPS; i++) {
    pup = &upqs[i];
    if (!pup->up_valid) {
      pup->up_valid = TRUE;
      pup->up_port = 0;
      pup->up_pid = BADPID;
      pup->up_xport = pcreate(UPPLEN);
      signal(udpmutex);
      return i;
    }
  }
  signal(udpmutex);
  return SYSERR;
}

struct upq upqs[UPPS];
