CC = gcc
all:
	$(CC)  -I/usr/include/mysql   subtocsinfo.c  -L/usr/lib -lmysqlclient  -o subtocsinfo 
.PHONY: all clean
clean:
	$(RM) subtocsinfo $(TARGETS)
