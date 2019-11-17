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
	$(CC) -o $(NAME_EXE) $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(ALLCPPSOBJ)) $(INCLUDE) $(LIBS)
	

#Esto compila y crea los .o (aqui aun no entra en juego el linker)
%.o: %.cpp
	$(CC) $(CXXFLAGS) -o $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$@) -c $^ $(INCLUDE) $(INCLUDE_IRR)


$(OBJSUBDIRS):
	mkdir -p $(OBJSUBDIRS)


info:
	$(info $(SUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCPPSOBJ))

.PHONY: exe
exe:
	./$(NAME_EXE)

.PHONY: clean
clean:
	rm -Rf $(OBJ_PATH)/ && rm -f $(NAME_EXE)

.PHONY: all
all:
	make clean ; make ; make exe