SOURCES  	:= $(wildcard *.cpp)
OBJECTS  	:= $(subst .cpp,.o,$(SOURCES))
OBJ_PATH    := obj/
SRC_PATH	:= src/
LIBS 	    := /usr/lib/libIrrlicht.so
INCLUDE     := -I.
INCLUDE_IRR := -I /usr/include/irrlicht
CC			:= g++
NAME_EXE	:= Beast_Brawl

#Esto compila y crea los .o (aqui aun no entra en juego el linker)
%.o: %.cpp
	mkdir $(OBJ_PATH) ; $(CC) -o $(OBJ_PATH)$@ -c $^ $(INCLUDE) $(INCLUDE_IRR)

#Esto crea el ejecutable
$(NAME_EXE): $(OBJECTS)
	cd $(OBJ_PATH) && $(CC) -o ../$@ $^ $(INCLUDE) $(LIBS)


info:
	$(info $(SOURCES))
	$(info $(OBJECTS))

.PHONY: exe
exe:
	./$(NAME_EXE)

.PHONY: clean
clean:
	cd $(OBJ_PATH) ; rm -rf $(OBJECTS) ; cd .. ; rmdir $(OBJ_PATH) ; rm -rf $(NAME_EXE)

.PHONY: all
all:
	make clean ; make ; make exe