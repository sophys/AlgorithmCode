CC = g++
all:
	$(CC) -I/usr/include/mysql -L/usr/lib -lmysqlclient main.c fun.c app.cpp -ldl -o clickpriapp
.PHONY: all clean
clean:
	$(RM) clickpriapp $(TARGETS)
