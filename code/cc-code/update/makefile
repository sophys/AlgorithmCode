CC = gcc
all:
	$(CC)  -I/usr/include/mysql   update.c  -L/usr/lib -lmysqlclient  -o update 
.PHONY: all clean
clean:
	$(RM) update $(TARGETS)
