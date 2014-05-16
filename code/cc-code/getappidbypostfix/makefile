CC = gcc
all:
	$(CC) -I/usr/include/mysql getappidbypostfix.c  -L/usr/lib -lmysqlclient -o getappidbypostfix
.PHONY: all clean
clean:
	$(RM) getappidbypostfix $(TARGETS)
