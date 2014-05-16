CC = gcc
all:
	$(CC)  -I/usr/include/mysql   downpkg.c sendfile.c  -L/usr/lib -lmysqlclient  -o downpkg 
.PHONY: all clean
clean:
	$(RM) downpkg $(TARGETS)
