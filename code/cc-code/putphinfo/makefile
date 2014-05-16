CC = gcc
all:
	$(CC)  -I/usr/include/mysql   putphinfo.c  -L/usr/lib  -lmysqlclient  -o putphinfo 
.PHONY: all clean
clean:
	$(RM) putphinfo $(TARGETS)
