CC = gcc
all:
	$(CC)  -I/usr/include/mysql putcttodb.c  -L/usr/lib -lmysqlclient  -o putcttodb 
.PHONY: all clean
clean:
	$(RM) putcttodb $(TARGETS)
