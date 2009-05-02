Running FastCgiQt applications
------------------------------

Currently, the only tested method is via apache + mod_fastcgi; hopefully, it
should work with any spec-following implementation that binds the fastcgi socket
to STDIN_FILENO before spawning the executable.
