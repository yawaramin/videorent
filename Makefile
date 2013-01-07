CC=gcc -g
GTK_LIBS=`pkg-config --libs gtk+-2.0`
GTK_CFLAGS=`pkg-config --cflags gtk+-2.0`
RMFILE=rm $1;

all : videorent videorent_test

check : videorent_test
	gtester videorent_test

clean :
	$(foreach f,*.gch *.o videorent videorent_test,$(call RMFILE,$(f)))

videorent : video.o rental.o videorent.o
	$(CC) -Wall -o videorent rental.o videorent.o $(GTK_LIBS)

videorent_test : video.o rental.o videorent.o videorent_test.o
	$(CC) -Wall -o videorent_test video.o rental.o videorent.o videorent_test.o $(GTK_LIBS)

videorent_test.o : videorent.h videorent_test.c
	$(CC) -Wall -c videorent.h videorent_test.c $(GTK_CFLAGS)

video.o : video.h video.c
	$(CC) -Wall -c video.h video.c $(GTK_CFLAGS)

rental.o : rental.h rental.c
	$(CC) -Wall -c rental.h rental.c $(GTK_CFLAGS)

videorent.o : video.h rental.h videorent.h videorent.c
	$(CC) -Wall -c video.h rental.h videorent.h videorent.c $(GTK_CFLAGS)
