Running FastCgiQt applications
------------------------------

There are two interfaces that FastCgiQt can use, configured with --configure-httpd.

These are:

- FCGI-UNIX: Works with Apache, via mod_fastcgi or mod_fcgid. This is what's in the
	FastCGI specification. When this interface is configured (the default), it
	expects a unix socket to be bound to file number zero.

- FCGI-TCP: Works with lighttpd and cherokee. When this, and a corresponding port
	number are configured, your application will listen on all IP addresses, on
	the specified port.
