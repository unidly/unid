/**
 * @file
 * @brief Converts to network byte order
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/**
 * @brief A utility function to convert from host to network byte order
 *
 * @param pupd Pointer to udp data structure
 * @returns network ordered structure
 */
void udph2net(struct udp *pudp) {
  pudp->u_src = hs2net(pudp->u_src);
  pudp->u_dst = hs2net(pudp->u_dst);
  pudp->u_len = hs2net(pudp->u_len);
}
