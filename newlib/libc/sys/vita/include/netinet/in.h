/*
 * Copyright (c) 2016, Sunguk Lee
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _NETINET_IN_H_
#define _NETINET_IN_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <psp2/net/net.h>

#define IPPROTO_IP		SCE_NET_IPPROTO_IP
#define IPPROTO_ICMP	SCE_NET_IPPROTO_ICMP
#define IPPROTO_IGMP	SCE_NET_IPPROTO_IGMP
#define IPPROTO_TCP		SCE_NET_IPPROTO_TCP
#define IPPROTO_UDP		SCE_NET_IPPROTO_UDP

typedef uint32_t in_addr_t;
typedef uint16_t in_port_t;

/* Definitions of the bits in an Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */

#define IN_CLASSA(a)            ((((in_addr_t)(a)) & 0x80000000) == 0)
#define IN_CLASSA_NET           0xff000000
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          (0xffffffff & ~IN_CLASSA_NET)
#define IN_CLASSA_MAX           128

#define IN_CLASSB(a)            ((((in_addr_t)(a)) & 0xc0000000) == 0x80000000)
#define IN_CLASSB_NET           0xffff0000
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          (0xffffffff & ~IN_CLASSB_NET)
#define IN_CLASSB_MAX           65536

#define IN_CLASSC(a)            ((((in_addr_t)(a)) & 0xe0000000) == 0xc0000000)
#define IN_CLASSC_NET           0xffffff00
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          (0xffffffff & ~IN_CLASSC_NET)

#define IN_CLASSD(a)            ((((in_addr_t)(a)) & 0xf0000000) == 0xe0000000)
#define IN_MULTICAST(a)         IN_CLASSD(a)

#define IN_EXPERIMENTAL(a)      ((((in_addr_t)(a)) & 0xe0000000) == 0xe0000000)
#define IN_BADCLASS(a)          ((((in_addr_t)(a)) & 0xf0000000) == 0xf0000000)

/* Address to accept any incoming messages.  */
#define INADDR_ANY              ((in_addr_t) 0x00000000)
/* Address to send to all hosts.  */
#define INADDR_BROADCAST        ((in_addr_t) 0xffffffff)
/* Address indicating an error return.  */
#define INADDR_NONE             ((in_addr_t) 0xffffffff)

/* Network number for local host loopback.  */
#define IN_LOOPBACKNET          127
/* Address to loopback in software to local host.  */
#ifndef INADDR_LOOPBACK
# define INADDR_LOOPBACK        ((in_addr_t) 0x7f000001) /* Inet 127.0.0.1.  */
#endif

/* Defines for Multicast INADDR.  */
#define INADDR_UNSPEC_GROUP     ((in_addr_t) 0xe0000000) /* 224.0.0.0 */
#define INADDR_ALLHOSTS_GROUP   ((in_addr_t) 0xe0000001) /* 224.0.0.1 */
#define INADDR_ALLRTRS_GROUP    ((in_addr_t) 0xe0000002) /* 224.0.0.2 */
#define INADDR_MAX_LOCAL_GROUP  ((in_addr_t) 0xe00000ff) /* 224.0.0.255 */

struct in_addr {
	in_addr_t s_addr;
} in_addr;

struct sockaddr_in {
	uint8_t			sin_len;
	sa_family_t		sin_family;
	in_port_t		sin_port;
	struct in_addr	sin_addr;
	in_port_t		sin_vport;
	char			sin_zero[6];
} sockaddr_in;

struct ip_mreq {
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};

struct ip_mreq_source {
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
    struct in_addr imr_sourceaddr;
};

/* IPv6 support */
struct in6_addr {
	union {
		uint8_t __u6_addr8[16];
		uint16_t __u6_addr16[8];
		uint32_t __u6_addr32[4];
	} __in6_u;
#define s6_addr                __in6_u.__u6_addr8
#define s6_addr16              __in6_u.__u6_addr16
#define s6_addr32              __in6_u.__u6_addr32
};

extern const struct in6_addr in6addr_any;        /* :: */
extern const struct in6_addr in6addr_loopback;   /* ::1 */
#define IN6ADDR_ANY_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
#define IN6ADDR_LOOPBACK_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }

struct sockaddr_in6{
	in_port_t sin6_port;        /* Transport layer port # */
	uint32_t sin6_flowinfo;     /* IPv6 flow information */
	struct in6_addr sin6_addr;  /* IPv6 address */
	uint32_t sin6_scope_id;     /* IPv6 scope-id */
};

struct ipv6_mreq {
	struct in6_addr ipv6mr_multiaddr;
	unsigned int ipv6mr_interface;
};

/* Multicast group request.  */
struct group_req {
	uint32_t gr_interface;
	struct sockaddr_storage gr_group;
};

struct group_source_req {
	uint32_t gsr_interface;
	struct sockaddr_storage gsr_group;
	struct sockaddr_storage gsr_source;
};

/* Full-state filter operations.  */
struct ip_msfilter {
	struct in_addr imsf_multiaddr;
	struct in_addr imsf_interface;
	uint32_t imsf_fmode;
	uint32_t imsf_numsrc;
	struct in_addr imsf_slist[1];
};

#define IP_MSFILTER_SIZE(numsrc) (sizeof (struct ip_msfilter) \
                                  - sizeof (struct in_addr)                   \
                                  + (numsrc) * sizeof (struct in_addr))

struct group_filter {
	uint32_t gf_interface;
	struct sockaddr_storage gf_group;
	uint32_t gf_fmode;
	uint32_t gf_numsrc;
	struct sockaddr_storage gf_slist[1];
};

#define GROUP_FILTER_SIZE(numsrc) (sizeof (struct group_filter) \
                                   - sizeof (struct sockaddr_storage)         \
                                   + ((numsrc)                                \
                                      * sizeof (struct sockaddr_storage)))

#define ntohs __builtin_bswap16
#define htons __builtin_bswap16
#define ntohl __builtin_bswap32
#define htonl __builtin_bswap32

#define IN6_IS_ADDR_MULTICAST(a) (((const uint8_t *) (a))[0] == 0xff)

#define IN6_IS_ADDR_MC_NODELOCAL(a) \
        (IN6_IS_ADDR_MULTICAST(a)                                             \
         && ((((const uint8_t *) (a))[1] & 0xf) == 0x1))

#define IN6_IS_ADDR_MC_LINKLOCAL(a) \
        (IN6_IS_ADDR_MULTICAST(a)                                             \
         && ((((const uint8_t *) (a))[1] & 0xf) == 0x2))

#define IN6_IS_ADDR_MC_SITELOCAL(a) \
        (IN6_IS_ADDR_MULTICAST(a)                                             \
         && ((((const uint8_t *) (a))[1] & 0xf) == 0x5))

#define IN6_IS_ADDR_MC_ORGLOCAL(a) \
        (IN6_IS_ADDR_MULTICAST(a)                                             \
         && ((((const uint8_t *) (a))[1] & 0xf) == 0x8))

#define IN6_IS_ADDR_MC_GLOBAL(a) \
        (IN6_IS_ADDR_MULTICAST(a)                                             \
         && ((((const uint8_t *) (a))[1] & 0xf) == 0xe))

struct in6_pktinfo {
	struct in6_addr ipi6_addr;  /* src/dst IPv6 address */
	unsigned int ipi6_ifindex;  /* send/recv interface index */
};

/* IPv6 MTU information.  */
struct ip6_mtuinfo {
	struct sockaddr_in6 ip6m_addr; /* dst address including zone ID */
	uint32_t ip6m_mtu;             /* path MTU in host byte order */
};

#endif
