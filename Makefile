CC=clang

all: sandbox

common.o: common.c
	$(CC) -c -Wall -o common.o common.c

openAlertBox.o: openAlertBox.c
	$(CC) -c -Wall `pkg-config --cflags gtk+-3.0` -o openAlertBox.o openAlertBox.c

heuristics.o: heuristics.c
	$(CC) -c -Wall `pkg-config --cflags gtk+-3.0` -o heuristics.o heuristics.c

handleNotification.o: handleNotification.c
	$(CC) -c -Wall `pkg-config --cflags gtk+-3.0` -o handleNotification.o handleNotification.c

socketComm.o: socketComm.c
	$(CC) -c -Wall -o socketComm.o socketComm.c

installSeccompNotifier.o: installSeccompNotifier.c
	$(CC) -c -Wall -o installSeccompNotifier.o installSeccompNotifier.c

sandbox: sandbox.c socketComm.o installSeccompNotifier.o common.o handleNotification.o openAlertBox.o heuristics.o
	$(CC) `pkg-config --cflags gtk+-3.0` -o sandbox sandbox.c socketComm.o installSeccompNotifier.o common.o handleNotification.o openAlertBox.o heuristics.o `pkg-config --libs gtk+-3.0`

clean:
	rm -f sandbox *.o