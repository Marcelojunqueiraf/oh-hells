COMPONENTES := $(wildcard Componentes/*/*.cpp)
SUBCOMPONENTES := $(wildcard Componentes/*/*/*.cpp)
UTILS := $(wildcard Utils/*/*.cpp)
STATES := $(wildcard States/*/*.cpp)
FILES := $(wildcard */*.cpp)

all:
	g++ -std=c++17 -g main.cpp ${COMPONENTES} ${UTILS} $(STATES) ${FILES} ${SUBCOMPONENTES} -o jogo `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL2main
	 ./jogo

# # O compilador
# COMPILER = g++
# # Comando para remover pastas
# RMDIR = rm -rdf
# # Comando para remover arquivos
# RM = rm -f

# # "Flags" para geração automática de dependências
# DEP_FLAGS = -M -MT $@ -MT $(BIN_PATH)/$(*F).o -MP -MF $@
# # Bibliotecas a serem linkadas
# LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
# # Caminho dos includes
# INC_PATHS = -I$(INC_PATH) $(addprefix -I,$(SDL_INC_PATH))
# # INC_PATHS += $(addprefix -I,$(sort $(dir $(wildcard Componentes/*))))
# # INC_PATHS += $(addprefix -I,$(sort $(dir $(wildcard Componentes/*/*))))
# # INC_PATHS += $(addprefix -I,$(sort $(dir $(wildcard Utils/*))))
# # INC_PATHS += $(addprefix -I,$(sort $(dir $(wildcard */))))

# INCLUDES    += -I /driverlib

# # Diretivas de compilação
# FLAGS = -Wall -pedantic -Wextra -Wno-unused-parameter -Werror=init-self
# # Diretivas extras para debug
# DFLAGS = -ggdb -O0 -DDEBUG
# # Diretivas extras para release
# RFLAGS = -O3 -mtune=native

# INC_PATH = .
# SRC_PATH = .
# BIN_PATH = .
# DEP_PATH = .

# CPP_COMPONENTES := $(wildcard Componentes/*/*.cpp)
# CPP_SUBCOMPONENTES := $(wildcard Componentes/*/*/*.cpp)
# CPP_UTILS := $(wildcard Utils/*/*.cpp)
# CPP_OTHERS := $(wildcard */*.cpp)

# # INC_COMPONENTES := $(wildcard Componentes/*/*.h)
# # INC_SUBCOMPONENTES := $(wildcard Componentes/*/*/*.h)
# # INC_UTILS := $(wildcard Utils/*/*.h)
# # INC_OTHERS := $(wildcard */*.h)

# # Uma lista de arquivos por extensão:
# CPP_FILES = $(wildcard $(SRC_PATH)/*.cpp) ${CPP_COMPONENTES} ${CPP_SUBCOMPONENTES} ${CPP_UTILS} ${CPP_OTHERS} 
# # INC_FILES = $(wildcard $(INC_PATH)/*.h) $(INC_COMPONENTES) $(INC_SUBCOMPONENTES) $(INC_UTILS) $(INC_OTHERS)
# # FILE_NAMES = $(sort $(CPP_FILES:.cpp=))
# #  $(INC_FILES:.h=))
# DEP_FILES = $(CPP_FILES:.cpp=.d)
# OBJ_FILES = $(CPP_FILES:.cpp=.o)

# # DEP_FILES = $(addprefix $(DEP_PATH)/,$(addsuffix .d,$(FILE_NAMES)))
# # OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))

# # Nome do executável
# EXEC = main

# # Windows ----------------------------------------------------------------
# ifeq ($(OS),Windows_NT)
# RMDIR = rd /s /q
# RM = del /q

# SDL_PATHS = C:/SDL2/x86_64-w64-mingw32 C:/Tools/mysys64/mingw64 C:\SDL2-2.0.5\x86_64-w64-mingw32

# SDL_INC_PATH = $(addsuffix /include,$(SDL_PATHS))
# LINK_PATH = $(addprefix -L,$(addsuffix /lib,$(SDL_PATHS)))
# FLAGS += -mwindows
# DFLAGS += -mconsole
# LIBS := -lmingw32 -lSDL2main $(LIBS)

# # Nome do executável
# EXEC = $(EXEC).exe

# else

# 	UNAME_S := $(shell uname -s)

# # Mac ----------------------------------------------------------------
# ifeq ($(UNAME_S), Darwin)

# LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf

# endif
# endif

# # Regra geral
# all: $(EXEC)

# # Gera o executável
# $(EXEC): $(OBJ_FILES)
# 	$(COMPILER) -o $@ $^ $(LINK_PATH) $(LIBS) $(FLAGS)

# #Gera os arquivos objetos
# %.o: %.d
# 	$(COMPILER) $(INC_PATHS) $(addprefix $(SRC_PATH)/,$(<:.d=.cpp)) -c $(FLAGS) -o $@

# # Gera os arquivos de dependência
# %.d: %.cpp
# 	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS) $(FLAGS)

# clean:
# 	-$(RMDIR) $(DEP_PATH)
# 	-$(RMDIR) $(BIN_PATH)
# 	$(RM) $(EXEC)

# .SECONDEXPANSION:
# 	-include $$(DEP_FILES)

# .PRECIOUS: $(DEP_FILES)
# .PHONY: release debug clean folders help

# release: FLAGS += $(RFLAGS)
# release: $(EXEC)

# debug: FLAGS += $(DFLAGS)
# debug: $(EXEC)

# folders:
# ifeq ($(OS), Windows_NT)
# 	@if NOT exists$(DEP_PATH) ( mkdir $(DEP_PATH) )
# 	@if NOT exists$(BIN_PATH) ( mkdir $(BIN_PATH) )
# 	@if NOT exists$(INC_PATH) ( mkdir $(INC_PATH) )
# 	@if NOT exists$(SRC_PATH) ( mkdir $(SRC_PATH) )
# else
# 	@mkdir -p $(DEP_PATH) $(BIN_PATH) $(INC_PATH) $(SRC_PATH)
# endif

# # Regra para debug
# print-% : @echo $* = $($*) 

# help:
# ifeq ($(OS), Windows_NT)
# 	@echo.
# endif
# 	@echo Available targets:
# 	@echo - release: Builds the release version
# 	@echo - debug: Builds the debug version
# 	@echo - clean: Cleans generated FILE_NAMES
# 	@echo - folders: Generate project directories
# 	@echo - help: Shows this help
# ifeq ($(OS), Windows_NT)
# 	@echo.
# endif