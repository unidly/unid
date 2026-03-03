/**
 * @file
 * @brief Handles an outgoing udp datagram.
 *
 * @copyright Copyright 2026, FhirFirst, LLC. All rights reserved.
 */

#include <conf.h>
#include <igmp.h>
#include <kernel.h>
#include <network.h>

/**
 * @brief Handles an outgoing udp datagram for the remote machine.
 *
 * When an application program generates UDP output, it transfers control to
 * the operating system and eventually calls udpsend to send the datagram.
 *
 * @param fip The remote machine IP address to send the datagram
 * @param fport The remote machine port
 * @param lport The local machine port
 * @param pep The datagram structure
 * @param datalen The number of octets in the data field
 * @param docksum A boolean indicating if cksum is set
 * @returns The return value of the ipsend call
 */
int udpsend(IPaddr fip, u_short fport, u_short lport, struct ep *pep,
            unsigned datalen, Bool docksum) {
  struct ip *pip = (struct ip *)pep->ep_data;
  struct udp *pudp = (struct udp *)pip->ip_data;
  struct route *prt;
  struct hg *phg;
  int ttl = IP_TTL;

  pudp->u_src = lport;
  pudp->u_dst = fport;
  pudp->u_len = U_HLEN + datalen;
  pudp->u_cksum = 0;
  udph2net(pudp);
  pep->ep_order &= ~EPO_UDP;
  if (docksum) {
    prt = rtget(fip, RTF_LOCAL);
    if (prt == NULL) {
      IpOutNoRoutes++;
      freebuf(pep);
      return SYSERR;
    }
    pip->ip_dst = fip;
    if (prt->rt_ifnum == NI_LOCAL)
      pip->ip_src = pip->ip_dst;
    else
      pip->ip_src = nif[prt->rt_ifnum].ni_ip;
    rtfree(prt);
    pudp->u_cksum = udpcksum(pep, U_HLEN + datalen);
    if (pudp->u_cksum == 0) pudp->u_cksum = ~0;
  }
  UdpOutDatagrams++;
  if (IP_CLASSD(fip)) {
    wait(HostGroup.hi_mutex);
    phg = hglookup(NI_PRIMARY, fip);
    if (phg)
      ttl = phg->hg_ttl;
    else
      ttl = 1;
    signal(HostGroup.hi_mutex);
  }
  return ipsend(fip, pep, U_HLEN + datalen, IPT_UDP, IPP_NORMAL, ttl);
}
