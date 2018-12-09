all: httpd

httpd: httpd.c response.c
	gcc httpd.c -o httpd -lpthread -lpcre -include response.h response.c
