#include "crypto.h"

void ssl_init() {
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
}

void ssl_cleanup() {
	EVP_cleanup();
}

SSL_CTX *ssl_get_context() {
	const SSL_METHOD *method;
	SSL_CTX *ctx;
	method = TLS_client_method();
	ctx = SSL_CTX_new(method);
	if ( ctx == NULL ) {
		ERR_print_errors_fp(stderr);
		return NULL;
	}
	return ctx;
}

void display_cert(SSL* ssl) {
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl);

	if ( cert != NULL ) {
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	
		return;
	}

	printf("Info: No client certificates configured.\n");
}