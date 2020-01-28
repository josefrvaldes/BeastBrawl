COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
CLENGINE_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [CLEngine cleaned succesfully]\033[0m"
SRC_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [SRC cleaned succesfully]\033[0m"
#EXECUTING_TEXT     := @echo -e "\033[0m \033[5;32m Executing...\033[0m"
JUMP_LINE		   := @echo


ifdef DEBUG
	CXXFLAGS += -g
else
	CXXFLAGS += -O3
endif

ifdef WINDOWS
	LIBS 	    := -L./lib/windows/irrlicht -lIrrlicht -L./lib/windows/fmod -lfmod -lfmodstudio
	INCLUDE     := -I. 
	INCLUDE_IRR := -I /include/irrlicht/irrlicht.h
	#INCLUDE_BOOST := -I /include/boost/irrlicht.h
	INCLUDE_FMOD := -I ./include/fmod/core -I ./include/fmod/studio
	#INCLUDE_BULLET := -I./include/bullet -I./include
	CC			:= g++
else
	#LIBS		:= -L/usr/lib32 -lX11
	LIBS 	    += -L./lib/linux/irrlicht -lIrrlicht -L./lib/linux/fmod -lfmod -lfmodstudio -L./lib/linux/glew -lGLEW -lGL -L./lib/linux/assimp -lassimp
	LIBS		+= -Wl,-rpath=lib/linux/irrlicht -Wl,-rpath=lib/linux/fmod -Wl,-rpath=lib/linux/assimp
	INCLUDE     := -I. 
	INCLUDE_IRR := -I /include/irrlicht/irrlicht.h
	INCLUDE_FMOD := -I ./include/fmod/core -I ./include/fmod/studio
	INCLUDE_GLEW := -I /include/glew/glew.h
	INCLUDE_ASSIMP 	:= -I./include/assimp
	#INCLUDE_BULLET := -I./include/bullet -I./include
	CREATE_SYMLINKS := bash symlinks.sh
	CC			:= g++
endif


SOURCES  	:= $(wildcard *.cpp)
OBJ_PATH    := obj/src
SRC_PATH	:= src

NAME_EXE	:= Beast_Brawl
CXXFLAGS 	+= -Wall -Wno-unknown-pragmas -std=c++17 -pthread # el no-unknown-pragmas es para que no salga el warning de los pragma region

ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCPPSOBJ	:= $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(ALLCPPS))
SUBDIRS		:= $(shell find src/ -type d)
OBJSUBDIRS  := $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SUBDIRS))



#Esto crea el ejecutable
$(NAME_EXE): $(OBJSUBDIRS) $(ALLCPPSOBJ)
	$(CREATE_SYMLINKS)
	$(COMPILING_TEXT_OK)
	$(JUMP_LINE)
	$(LINKING_TEXT)
	$(CC) -o $(NAME_EXE) $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(ALLCPPSOBJ)) $(INCLUDE) $(LIBS) $(CXXFLAGS)
	$(LINKING_TEXT_OK)
	$(JUMP_LINE)


#Esto compila y crea los .o (aqui aun no entra en juego el linker)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(PRUEBA_TEXT)
	$(COMPILING_TEXT) $<
	@$(CC) $(CXXFLAGS) -o $@ -c $^ $(INCLUDE) $(INCLUDE_IRR) $(INCLUDE_FMOD) $(INCLUDE_GLEW) $(INCLUDE_ASSIMP)
	

$(OBJSUBDIRS):
	@mkdir -p $(OBJSUBDIRS)

.PHONY: gl
gl:
	$(MAKE) -C CLEngine/

info:
	$(info $(SUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCPPSOBJ))

.PHONY: exe_gl
exe_gl:
	@CLEngine/CLEngine

.PHONY: exe
exe:
	$(CREATE_SYMLINKS)
	@sh ./$(NAME_EXE).sh

.PHONY: clean_all
clean_all:
	@make clean_gl ; make clean

.PHONY: clean_gl
clean_gl:
	@rm -Rf obj/CLEngine || rm -f $(NAME_EXE)
	$(CLENGINE_CLEANED_TEXT)

.PHONY: clean
clean:
	@rm -Rf obj/src || rm -f $(NAME_EXE)
	$(SRC_CLEANED_TEXT)

.PHONY: all
all:
	@make clean_all ; make ; make gl ; make exe
