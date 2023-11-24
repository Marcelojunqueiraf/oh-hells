COMPONENTES := $(wildcard Componentes/*/*.cpp)
SUBCOMPONENTES := $(wildcard Componentes/*/*/*.cpp)
UTILS := $(wildcard Utils/*/*.cpp)
FILES := $(wildcard */*.cpp)

all:
	g++ main.cpp ${COMPONENTES} ${UTILS} ${FILES} ${SUBCOMPONENTES} -o jogo `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL2main
	 ./jogo
