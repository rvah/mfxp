#include "data.h"
#include "net.h"

ssize_t data_read_socket(struct site_info *site, void *buf, size_t len, bool force_plaintext) {
	if(site->use_tls && !force_plaintext) {
		return net_socket_secure_recv(site->data_secure_fd, buf, len);
	} else {
		return net_socket_recv(site->data_socket_fd, buf, len, 0);
	}
}

ssize_t data_write_socket(struct site_info *site, const void *buf, size_t len, bool force_plaintext) {
	if(site->use_tls && !force_plaintext) {
		return net_socket_secure_send(site->data_secure_fd, buf, len);
	} else {
		return net_socket_send(site->data_socket_fd, buf, len, 0);
	}
}
