## Código foi feito no WSL2, por isso esta parte está bem diferente do exemplo
## Ninguém merece lidar com Windows para programar

FLAGS = allegro-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 allegro_image-5

CFLAGS = $(shell pkg-config $(FLAGS) --cflags)
LDFLAGS = $(shell pkg-config $(FLAGS) --libs)

all: invaders.exe

invaders.exe: invaders.c
	gcc invaders.c -o invaders.exe $(CFLAGS) $(LDFLAGS)

clean:
	rm -f invaders.exe
