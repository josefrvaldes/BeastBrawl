COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
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
	INCLUDE_FMOD := -I ./include/fmod/core -I ./include/fmod/studio
	#INCLUDE_BULLET := -I./include/bullet -I./include
	CC			:= g++
else
	LIBS 	    := -L./lib/linux/irrlicht -lIrrlicht -L./lib/linux/fmod -lfmod -lfmodstudio -L./lib/linux/sdl -lSDL2
	LIBS		+= -Wl,-rpath=lib/linux/irrlicht -Wl,-rpath=lib/linux/fmod  -Wl,-rpath=lib/linux/sdl
	INCLUDE     := -I. 
	INCLUDE_IRR := -I /include/irrlicht/irrlicht.h
	INCLUDE_SDL := -I /include/sdl/sdl.h
	INCLUDE_FMOD := -I ./include/fmod/core -I ./include/fmod/studio
	#INCLUDE_BULLET := -I./include/bullet -I./include
	CREATE_SYMLINKS := bash symlinks.sh
	CC			:= g++
endif

SOURCES  	:= $(wildcard *.cpp)
OBJ_PATH    := obj
SRC_PATH	:= src

NAME_EXE	:= Beast_Brawl
CXXFLAGS 	+= -Wall -Wno-unknown-pragmas -std=c++17 # el no-unknown-pragmas es para que no salga el warning de los pragma region

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
	$(CC) -o $(NAME_EXE) $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(ALLCPPSOBJ)) $(INCLUDE) $(LIBS) 
	$(LINKING_TEXT_OK)
	$(JUMP_LINE)


#Esto compila y crea los .o (aqui aun no entra en juego el linker)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(PRUEBA_TEXT)
	$(COMPILING_TEXT) $<
	@$(CC) $(CXXFLAGS) -o $@ -c $^ $(INCLUDE) $(INCLUDE_IRR) $(INCLUDE_FMOD) $(INCLUDE_SDL)
	

$(OBJSUBDIRS):
	@mkdir -p $(OBJSUBDIRS)


info:
	$(info $(SUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCPPSOBJ))

.PHONY: exe
exe:
	$(CREATE_SYMLINKS)
	@sh ./$(NAME_EXE).sh

.PHONY: clean
clean:
	@rm -Rf $(OBJ_PATH)/ && rm -f $(NAME_EXE)
	$(ALL_CLEANED_TEXT)

.PHONY: all
all:
	@make clean ; make ; make exe
