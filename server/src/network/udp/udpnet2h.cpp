/**
 * @file
 * @brief Converts to host byte order
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/**
 * @brief A utility function to convert from network to host byte order
 *
 * @param pupd Pointer to udp data structure
 * @returns host ordered structure
 */
void udpnet2h(struct udp *pudp) {
  pudp->u_src = net2hs(pudp->u_src);
  pudp->u_dst = net2hs(pudp->u_dst);
  pudp->u_len = net2hs(pudp->u_len);
}
