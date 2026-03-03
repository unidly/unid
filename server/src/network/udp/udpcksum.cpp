/**
 * @file
 * @brief Handles the checksum creation for a UDP datagram
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/**
 * @brief Handles the datagram checksum of a UDP datagram
 *
 * The UDP checksum covers the UDP datagram plus a pseudo-header that
 * includes the IP source and destination addresses, the UDP length, and the
 * UDP type identifier. This function does not construct a pseudo-header in
 * memory, but picks up individual fields from the IP header and includes them
 * in the checksum calculation.
 *
 * @param pep Pointer to pep structure
 * @param len The length of the data part of the UDP datagram
 * @returns The checksum value
 */
unsigned short udpcksum(struct ep *pep, int len) {
  struct ip *pip = (struct ip *)pep->ep_data;
  struct udp *pudp = (struct udp *)pip->ip_data;
  unsigned short *sptr;
  unsigned long ucksum = 0;
  int i;

  sptr = (unsigned short *)&pip->ip_src;
  /* 2*IP_ALEN octets = IP_ALEN shorts... */
  /* they are in net order.		*/
  for (i = 0; i < IP_ALEN; ++i) ucksum += *sptr++;
  sptr = (unsigned short *)pudp;
  ucksum += hs2net(IPT_UDP + len);
  if (len % 2) {
    ((char *)pudp)[len] = 0; /* pad */
    len += 1;                /* for the following division */
  }
  len >>= 1; /* convert to length in shorts */

  for (i = 0; i < len; ++i) ucksum += *sptr++;
  ucksum = (ucksum >> 16) + (ucksum & 0xffff);
  ucksum += (ucksum >> 16);

  return (short)(~ucksum & 0xffff);
}
