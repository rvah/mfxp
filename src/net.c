#include "net.h"
#include <openssl/ssl.h>

/*
 * ----------------
 *
 * Private functions
 *
 * ----------------
 */


static void sigchld_handler(int s) {
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

static ssize_t default_socket_receiver(
		int fd, void *buf, size_t n, int flags) {
	return recv(fd, buf, n, flags);
}

static ssize_t (*socket_receiver)(int, void *, size_t, int) =
		default_socket_receiver;


static ssize_t default_socket_sender(
		int fd, const void *buf, size_t n, int flags) {
	return send(fd, buf, n, flags);
}

static ssize_t (*socket_sender)(int, const void *, size_t, int) =
	default_socket_sender;

static int default_socket_secure_receiver(SSL *ssl, void *buf, int num) {
	return SSL_read(ssl, buf, num);
}

static int (*socket_secure_receiver)(SSL *, void *, int) =
		default_socket_secure_receiver;

static int default_socket_secure_sender(SSL *ssl, const void *buf, int num) {
	return SSL_write(ssl, buf, num);
}

static int (*socket_secure_sender)(SSL *, const void *, int) =
	default_socket_secure_sender;

static int32_t default_socket_closer(int32_t fd) {
	return close(fd);
}

static int32_t (*socket_closer)(int32_t) = default_socket_closer;

static int32_t default_socket_opener(char *address, char *port) {
	int32_t sockfd;
	struct addrinfo hints, *servinfo, *p;
	int32_t rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
		log_w("getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(
				p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			log_w("error client: socket\n");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			socket_closer(sockfd);
			log_w("error client: connect\n");
			continue;
		}

		break;
	}

	if (p == NULL) {
		log_w("socket: failed to connect\n");
		return -1;
	}

	inet_ntop(p->ai_family,
			net_get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	log_w("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	return sockfd;
}

static int32_t (*socket_opener)(char *, char *) = default_socket_opener;

static int default_socket_secure_opener(SSL *ssl) {
	return SSL_connect(ssl);
}

static int (*socket_secure_opener)(SSL *) = default_socket_secure_opener;
/*
 * ----------------
 *
 * Public functions
 *
 * ----------------
 */

// get sockaddr, IPv4 or IPv6:
void *net_get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int32_t net_open_server_socket(char *port) {
	int32_t sockfd;
	struct addrinfo hints, *servinfo, *p;
	int32_t rv;
	struct sigaction sa;
	int yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		log_w("getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(
				p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			log_w("error server: socket\n");
			continue;
		}

		if(setsockopt(
				sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == 1) {
			log_w("error server: setsockopt\n");
			return -1;
		}

		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			socket_closer(sockfd);
			log_w("server: bind\n");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if(p == NULL) {
		log_w("server: failed to bind\n");
		return -1;
	}

	if(listen(sockfd, CONN_BACKLOG) == -1) {
		log_w("server: err listen\n");
		return -1;
	}

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		log_w("server: sigaction err\n");
		return -1;
	}

	return sockfd;
}

int32_t net_open_socket(char *address, char *port) {
	return socket_opener(address, port);
}

int net_open_socket_secure(SSL *ssl) {
	return socket_secure_opener(ssl);
}

int32_t net_close_socket(int32_t fd) {
	return socket_closer(fd);
}

ssize_t net_socket_recv(int fd, void *buf, size_t n, int flags) {
	return socket_receiver(fd, buf, n, flags);
}

ssize_t net_socket_send(int fd, const void *buf, size_t n, int flags) {
	return socket_sender(fd, buf, n, flags);
}

int net_socket_secure_recv(SSL *ssl, void *buf, int num) {
	return socket_secure_receiver(ssl, buf, num);
}

int net_socket_secure_send(SSL *ssl, const void *buf, int num) {
	return socket_secure_sender(ssl, buf, num);
}

void net_set_socket_opener(int32_t (*opener)(char *, char *)) {
	socket_opener = opener;
}

void net_set_socket_secure_opener(int (*secure_opener)(SSL *)) {
	socket_secure_opener = secure_opener;
}

void net_set_socket_closer(int32_t (*closer)(int32_t)) {
	socket_closer = closer;
}

void net_set_socket_receiver(ssize_t (*receiver)(int, void *, size_t, int)) {
	socket_receiver = receiver;
}

void net_set_socket_sender(ssize_t (*sender)(int, const void *, size_t, int)) {
	socket_sender = sender;
}

void net_set_socket_secure_receiver(int (*receiver)(SSL *, void *, int)) {
	socket_secure_receiver = receiver;
}

void net_set_socket_secure_sender(int (*sender)(SSL *, const void *, int)) {
	socket_secure_sender = sender;
}
