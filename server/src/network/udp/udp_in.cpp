/**
 * @file
 * @brief Handles an incoming udp datagram.
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ports.h>
#include <proc.h>

/**
 * @brief Handles an incoming udp datagram for the local machine.
 *
 *  This function first checks to see whether the sender supplied the optional
 *  checksum (if nonzero) abd validates the checksum if present. If present
 *  and valid, the udpnet2h function is used to convert the data to the
 *  proper host order.
 *
 *  After converting to host order, the function demultiplexes the datagram and
 *  and searches the set of queues to find one for the destination UDP port.
 *
 *  If not full, the function 
 *
 * @param pni Pointer to pni structure
 * @param pep Pointer to pep structure
 * @returns host ordered structure
 */
int udp_in(struct netif *pni, struct ep *pep) {
  struct ip *pip = (struct ip *)pep->ep_data;
  struct udp *pudp = (struct udp *)pip->ip_data;
  struct upq *pup;
  unsigned short dst;
  int i;

  if (pudp->u_cksum && udpcksum(pep, net2hs(pudp->u_len))) {
    freebuf(pep);
    return SYSERR; /* checksum error */
  }
  udpnet2h(pudp); /* convert UDP header to host order */
  dst = pudp->u_dst;
  wait(udpmutex);
  for (i = 0; i < UPPS; i++) {
    pup = &upqs[i];
    if (pup->up_port == dst) {
      /* drop instead of blocking on psend */
      if (pcount(pup->up_xport) >= UPPLEN) {
        signal(udpmutex);
        freebuf(pep);
        UdpInErrors++;
        return SYSERR;
      }
      psend(pup->up_xport, (WORD)pep);
      UdpInDatagrams++;
      if (!isbadpid(pup->up_pid)) {
        send(pup->up_pid, OK);
        pup->up_pid = BADPID;
      }
      signal(udpmutex);
      return OK;
    }
  }
  signal(udpmutex);
  UdpNoPorts++;
  icmp(ICT_DESTUR, ICC_PORTUR, pip->ip_src, pep, 0);
  return OK;
}

int udpmutex;
