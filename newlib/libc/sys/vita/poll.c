/*

Copyright (C) 2016, Sunguk Lee

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <psp2/kernel/threadmgr.h>
#include <psp2/net/net.h>
#include <psp2/types.h>

#include "vitadescriptor.h"

#define SCE_ERRNO_MASK 0xFF

#define MAX_EVENTS 255

#define LOCK_ARRAY_LENGTH   (MAX_OPEN_FILES / 8) + ((MAX_OPEN_FILES % 8) ? 1 : 0)
#define BIT_SET(arr, idx)	(arr)[(idx)/8] |= (1<<((idx)%8))
#define IS_SET(arr, idx)	((arr)[(idx)/8] & (1<<((idx)%8))) != 0

int poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	if (nfds == 0) {
		//sceKernelDelayThread(timeout);
		return 0;
	}

	uint8_t locked[LOCK_ARRAY_LENGTH] = {0};

	int i;
	int res;
	int eid = sceNetEpollCreate("", 0);
	if (eid < 0) {
		res = (int)eid;
		goto exit;
	}
	for (i = 0; i < nfds; i++) {
        if (fds[i].fd == 0) {
			fds[i].revents = 0;
        }

		SceNetEpollEvent ev = {0};
        ev.data.fd = fds[i].fd;
		
		if (fds[i].events && POLLIN) {
			ev.events |= SCE_NET_EPOLLIN;
		}
		if (fds[i].events && POLLOUT) {
			ev.events |= SCE_NET_EPOLLOUT;
		}
		ev.events |= SCE_NET_EPOLLERR;
		ev.events |= SCE_NET_EPOLLHUP;
		ev.events |= SCE_NET_EPOLLDESCID;

		fds[i].revents = 0;
		DescriptorTranslation *fdmap = __vita_fd_grab(fds[i].fd);
		if (fdmap == NULL) {
			continue;
		}
		if (fdmap->type != VITA_DESCRIPTOR_SOCKET) {
			__vita_fd_drop(fdmap);
			continue;
		}
		BIT_SET(locked, i);

		sceNetEpollControl(eid, SCE_NET_EPOLL_CTL_ADD, fdmap->sce_uid, &ev);
	}
	SceNetEpollEvent events[MAX_EVENTS] = {0};
    
    int nev = sceNetEpollWait(eid, events, MAX_EVENTS, 0);
    if (nev == 0)
		nev = sceNetEpollWait(eid, events, MAX_EVENTS, timeout);

	if (nev < 0) {
		res = nev;
		goto clean;
	}

	res = 0;
    int y;

	for (i = 0; i < nev; i++) {
		if (events[i].events) {
            for (y = 0; y < nfds; y++) {
                if (events[i].data.fd == fds[y].fd) {
					if (events[i].events & SCE_NET_EPOLLIN) {
						if (fds[i].events & POLLIN)
							fds[i].revents = fds[i].revents | POLLIN;
					}
					if (events[i].events & SCE_NET_EPOLLOUT) {
						if (fds[i].events & POLLOUT)
							fds[i].revents = fds[i].revents | POLLOUT;
					}
					if (events[i].events & SCE_NET_EPOLLERR) {
						fds[i].revents = fds[i].revents | POLLERR;
					}
					if (events[i].events & SCE_NET_EPOLLHUP) {
						fds[i].revents = fds[i].revents | POLLHUP;
					}
					if (events[i].events & SCE_NET_EPOLLDESCID) {
						fds[i].revents = fds[i].revents | POLLNVAL;
					}
					res++;
					break;
				}
			}
		}
	}

clean:
	sceNetEpollDestroy(eid);
exit:
	for (i = 0; i < nfds; i++) {
		if (IS_SET(locked, i) && __vita_fdmap[i] != NULL) {
			__vita_fd_drop(__vita_fdmap[i]);
		}
	}

	if (res < 0) {
		errno = res & SCE_ERRNO_MASK;
		return -1;
	}
	return res;
}
