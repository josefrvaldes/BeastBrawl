COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
JUMP_LINE		   := @echo


ifdef DEBUG
	CXXFLAGS += -g
else
	CXXFLAGS += -O4
endif


LIBS 	    	+= -L./lib/linux/glfw -lglfw3 -lGL -lX11 -lXrandr -lXi -ldl -Wl,-rpath=./lib/linux/glfw
LIBS 	    	+= -L./lib/linux/glew -lGLEW -Wl,-rpath=./lib/linux/glew
LIBS 	    	+= -L./lib/linux/assimp -lassimp5.0.0 -Wl,-rpath=lib/linux/assimp
LIBS			+= -L./lib/linux/freeType2 -lfreetype -Wl,-rpath=lib/linux/freeType2
INCLUDE     	:= -I./include -I../include -I./include/freeType2
CC				:= g++

CREATE_SYMLINKS := bash symlinks.sh
JOIN_ASSIMP := bash join_assimp.sh


SOURCES  	:= $(wildcard *.cpp)
OBJ_PATH    := ../obj/CLEngine
SRC_PATH	:= src

NAME_EXE	:= CLEngine
CXXFLAGS 	+= -Wall -Wno-unknown-pragmas  -std=c++17 -pthread # el no-unknown-pragmas es para que no salga el warning de los pragma region

ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCPPSOBJ	:= $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(ALLCPPS))
SUBDIRS		:= $(shell find src/ -type d)
OBJSUBDIRS  := $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SUBDIRS))

ifdef CACHE
	CC := ccache g++
	CXXFLAGS += -fuse-ld=gold
else
	CC := g++
endif


#Esto crea el ejecutable
$(NAME_EXE): $(OBJSUBDIRS) $(ALLCPPSOBJ)
	$(JOIN_ASSIMP)
	$(CREATE_SYMLINKS)
	$(COMPILING_TEXT_OK)
	$(JUMP_LINE)
	$(LINKING_TEXT)
	$(CC) -o $(NAME_EXE) $(ALLCPPSOBJ) $(INCLUDE) $(LIBS) $(CXXFLAGS)
	$(LINKING_TEXT_OK)
	$(JUMP_LINE)


#Esto compila y crea los .o (aqui aun no entra en juego el linker)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(PRUEBA_TEXT)
	$(COMPILING_TEXT) $<
	@$(CC) $(CXXFLAGS) -o $@ -c $^ $(INCLUDE)
	

$(OBJSUBDIRS):
	@mkdir -p $(OBJSUBDIRS)


info:
	$(info $(SUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCPPSOBJ))

.PHONY: exe
exe:
	./$(NAME_EXE)

.PHONY: clean
clean:
	@rm -Rf $(OBJ_PATH)/ && rm -f $(NAME_EXE)
	$(ALL_CLEANED_TEXT)


.PHONY: all
all:
	@make clean ; make ; make exe
