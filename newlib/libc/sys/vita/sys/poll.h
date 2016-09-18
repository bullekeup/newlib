/*
 * Copyright (c) 2016, BuLLeKeUp
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
#ifndef _SYS_POLL_H_
#define _SYS_POLL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>

/* Event types that can be polled for.  These bits may be set in `events'
   to indicate the interesting event types; they will appear in `revents'
   to indicate the status of the file descriptor.  */

/* Values that have no equivalent in VITA SDK are set to their
   best equivalent */
#define POLLIN		0x001		/* There is data to read.  */
#define POLLRDNORM  POLLIN
#define POLLRDBAND  POLLIN
#define POLLPRI     POLLIN

#define POLLOUT		0x002		/* Writing now will not block.  */
#define POLLWRNORM  POLLOUT
#define POLLWRBAND  POLLOUT

#define POLLERR     0x004
#define POLLHUP     0x008
#define POLLNVAL    0x010

typedef uint_32_t nfds_t;

struct pollfd {
   int   fd;         /* file descriptor */
   unsigned short events;     /* requested events */
   unsigned short revents;    /* returned events */
};

int poll(struct pollfd fds[], nfds_t nfds, int timeout);

#ifdef __cplusplus
}
#endif

#endif
