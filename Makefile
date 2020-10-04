APP := game

CFLAGS+=-I/usr/include/allegro5 -std=c99

ifdef PACKBIN
	CFLAGS+=-DPACKBIN
endif

ifdef RELEASE
	CFLAGS+= -Wl,-s -O3
	CLIBS=-Wl,-s
else
	CFLAGS+=-DDEBUG -O0 -g -Wall
endif

# Some, just for this example.
LIBS:= -lphysfs -lallegro_physfs -lallegro -lallegro_image -lm

CC:=gcc

SOURCE := $(wildcard *.c)

OBJ := $(patsubst %.c,%.o,$(SOURCE))


$(APP): $(OBJ)
	$(CC) $(OBJ) $(CLIBS) $(LIBS) -o $(APP)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

clean:
	rm *.o $(APP)

