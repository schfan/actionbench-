/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").  You may not use this file except
 * in compliance with the License.
 *
 * You can obtain a copy of the license at
 * src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing
 * permissions and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL
 * HEADER in each file and include the License file at
 * usr/src/OPENSOLARIS.LICENSE.  If applicable,
 * add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your
 * own identifying information: Portions Copyright [yyyy]
 * [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * getpeername test
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "libmicro.h"

#define	FIRSTPORT		12345

static int			sock = -1;

int
benchmark_init()
{
	(void) sprintf(lm_usage, "notes: measures getpeername()\n");
	lm_tsdsize = 0;

	return (0);
}

int
benchmark_initrun()
{
	int			j;
	int			opt = 1;
	int			result;
	socklen_t		size;
	struct hostent	*host;
	union {
		struct sockaddr_in in;
		struct sockaddr	sa;
	} adds;
	int			sock2, sock3;

	sock2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock2 == -1) {
		perror("socket");
		exit(1);
	}

	if (setsockopt(sock2, SOL_SOCKET, SO_REUSEADDR,
	    &opt, sizeof (int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if ((host = gethostbyname("localhost")) == NULL) {
		perror("gethostbyname");
		exit(1);
	}

	j = FIRSTPORT;
	for (;;) {
		(void) memset(&adds, 0, sizeof (struct sockaddr_in));
		adds.in.sin_family = AF_INET;
		adds.in.sin_port = htons(j++);
		(void) memcpy(&adds.in.sin_addr.s_addr, host->h_addr_list[0],
		    sizeof (struct in_addr));

		if (bind(sock2, &adds.sa,
		    sizeof (struct sockaddr_in)) == 0) {
			break;
		}

		if (errno != EADDRINUSE) {
			perror("bind");
			exit(1);
		}
	}

	if (listen(sock2, 5) == -1) {
		perror("listen");
		exit(1);
	}

	sock3 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock3 == -1) {
		perror("socket");
		exit(1);
	}

	if (fcntl(sock3, F_SETFL, O_NDELAY) == -1) {
		perror("fcntl");
		exit(1);
	}

	result = connect(sock3, &adds.sa,
	    sizeof (struct sockaddr_in));
	if ((result == -1) && (errno != EINPROGRESS)) {
		perror("connect");
		exit(1);
	}

	size = sizeof (struct sockaddr);
	sock = accept(sock2, &adds.sa, &size);
	if (sock == -1) {
		perror("accept");
		exit(1);
	}

	return (0);
}

/*ARGSUSED*/
int
benchmark(void *tsd, result_t *res)
{
	int			i;
	struct sockaddr		adds;
	socklen_t		size;

	for (i = 0; i < lm_optB; i++) {
		size = sizeof (struct sockaddr_in);
		if (getpeername(sock, (struct sockaddr *)&adds, &size) == -1) {
			perror("getpeername");
			exit(1);
			res->re_errors++;
		}
	}
	res->re_count = i;

	return (0);
}