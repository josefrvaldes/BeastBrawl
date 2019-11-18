COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
#EXECUTING_TEXT     := @echo -e "\033[0m \033[5;32m Executing...\033[0m"
JUMP_LINE		   := @echo


SOURCES  	:= $(wildcard *.cpp)
OBJ_PATH    := obj
SRC_PATH	:= src
LIBS 	    := /usr/lib/libIrrlicht.so
INCLUDE     := -I.
INCLUDE_IRR := -I /usr/include/irrlicht
CC			:= g++
NAME_EXE	:= Beast_Brawl
CXXFLAGS 	+= -g -Wall -std=c++14

ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCPPSOBJ	:= $(patsubst %.cpp,%.o,$(ALLCPPS))
SUBDIRS		:= $(shell find src/ -type d)
OBJSUBDIRS  := $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SUBDIRS))


#Esto crea el ejecutable
$(NAME_EXE): $(OBJSUBDIRS) $(ALLCPPSOBJ)
	$(COMPILING_TEXT_OK)
	$(JUMP_LINE)
	$(LINKING_TEXT)
	$(CC) -o $(NAME_EXE) $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(ALLCPPSOBJ)) $(INCLUDE) $(LIBS)
	$(LINKING_TEXT_OK)
	$(JUMP_LINE)


#Esto compila y crea los .o (aqui aun no entra en juego el linker)
%.o: %.cpp
	$(PRUEBA_TEXT)
	$(COMPILING_TEXT) $<
	@$(CC) $(CXXFLAGS) -o $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$@) -c $^ $(INCLUDE) $(INCLUDE_IRR) 
	

$(OBJSUBDIRS):
	@mkdir -p $(OBJSUBDIRS)


info:
	$(info $(SUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCPPSOBJ))

.PHONY: exe
exe:
	@./$(NAME_EXE)

.PHONY: clean
clean:
	@rm -Rf $(OBJ_PATH)/ && rm -f $(NAME_EXE)
	$(ALL_CLEANED_TEXT)

.PHONY: all
all:
	@make clean ; make ; make exe