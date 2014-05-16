CC = g++
all:
	$(CC) -I/usr/include/mysql -L/usr/lib -lmysqlclient main.cpp app.cpp -ldl -o clickapp
.PHONY: all clean
clean:
	$(RM) clickapp $(TARGETS)
