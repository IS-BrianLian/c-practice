#include <asm/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdbool.h>

#define IFNM_LEN (30)
static int nl_socket;

int readEvent (char *ifnm, bool *linkUp) {
	int status;
	char buf[4096];
	struct iovec iov = { buf, sizeof buf };
	struct sockaddr_nl snl;
	struct msghdr msg = { (void *) &snl, sizeof snl, &iov, 1, NULL, 0, 0 };
	struct nlmsghdr *h;
	struct ifinfomsg *ifi;

	status = recvmsg (nl_socket, &msg, 0);

	if (status < 0) {
		/* Socket non-blocking so bail out once we have read everything */
		if (errno == EWOULDBLOCK || errno == EAGAIN)
		return 0;

		/* Anything else is an error */
		printf ("read_netlink: Error recvmsg: %d\n", status);
		perror ("read_netlink: Error: ");
		return status;
	}

	if (status == 0) {
		printf ("read_netlink: EOF\n");
	 }

	// We need to handle more than one message per 'recvmsg'
	for (h = (struct nlmsghdr *) buf; NLMSG_OK (h, (unsigned int) status); h = NLMSG_NEXT (h, status)) {
			// Finish reading
			if (h->nlmsg_type == NLMSG_DONE) {
				return 0;
			}
			// Message is some kind of error
			if (h->nlmsg_type == NLMSG_ERROR) {
				printf ("read_netlink: Message is an error - decode TBD\n");
				return -1;
			}
			if (h->nlmsg_type == RTM_NEWLINK) {
				memset(ifnm, 0, sizeof(char) * IFNM_LEN);
				ifi = NLMSG_DATA (h);
				if_indextoname(ifi->ifi_index, ifnm);
				*linkUp = (ifi->ifi_flags & IFF_RUNNING) ? true : false;
			}
	}

	return 0;
}

int openSocket() {
	struct sockaddr_nl addr;

	nl_socket = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (nl_socket < 0) {
		printf ("Socket Open Error!\n");
		exit (1);
	}

	memset ((void *) &addr, 0, sizeof (addr));

	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid ();
	addr.nl_groups = RTMGRP_LINK;

	if (bind (nl_socket, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
		printf ("Socket bind failed!\n");
		exit (1);
	}

	return nl_socket;
}

int linkChangeEv (char *ifnm, bool *linkUp) {
	int retval;
	fd_set rfds;

	FD_ZERO (&rfds);
	FD_CLR (nl_socket, &rfds);
	FD_SET (nl_socket, &rfds);
	// struct timeval tv;
	// tv.tv_sec = 100;
	// tv.tv_usec = 0;
	retval = select (FD_SETSIZE, &rfds, NULL, NULL, NULL);
	if (retval == -1) {
		printf ("Error select()\n");
	} else if (retval) {
		readEvent (ifnm, linkUp);
	}

	return 0;
}


int useExample () {
	char ifnm[IFNM_LEN];
	bool linkUp;

	openSocket();
	while(1) {
		linkChangeEv(ifnm, &linkUp);
		printf ("NETLINK::%s is Link %s\n", ifnm, linkUp? "Up":"Down");
	}

	return 0;
}
