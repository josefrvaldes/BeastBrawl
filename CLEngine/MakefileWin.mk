COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
JUMP_LINE		   := @echo


ifdef DEBUG
	CXXFLAGS += -g
else
	CXXFLAGS += -O3
endif

LIBS 	    	+= -L./lib/windows/glfw -lglfw3 -lglfw3dll -lopengl32 -lgdi32 -luser32 -lcomdlg32 -lpthread -Wl,-rpath=./lib/windows/glfw
LIBS 	    	+= -L./lib/windows/glew -lglew32 -Wl,-rpath=lib/windows/glew
LIBS 	    	+= -L./lib/windows/assimp -lassimp -Wl,-rpath=lib/windows/assimp
LIBS			+= -L./lib/windows/freeType2 -lfreetype -Wl,-rpath=lib/windows/freeType2
LIBS 			+=  -lws2_32 -lwsock32

INCLUDE     	:= -I./include -I../include -I./include/freeType2

#CC			:= x86_64-w64-mingw32-g++
CC			:= x86_64-w64-mingw32-g++-posix

SOURCES  	:= $(wildcard *.cpp)
OBJ_PATH    := ../obj/CLEngine
SRC_PATH	:= src

NAME_EXE	:= CLEngine.exe
CXXFLAGS 	+= -Wall -Wno-unknown-pragmas -std=gnu++1z -pthread # el no-unknown-pragmas es para que no salga el warning de los pragma region
																										 # el -fuse-ld=gold es para el ccache
																										 # si pongo -std=c++17 falla M_PI, poniendo -std=gnu++17 funciona bien en windows
			# -static-libstdc++ -static-libgcc -static -pthread 

			
ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCPPSOBJ	:= $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(ALLCPPS))
SUBDIRS		:= $(shell find src/ -type d)
OBJSUBDIRS  := $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SUBDIRS))



#Esto crea el ejecutable
$(NAME_EXE): $(OBJSUBDIRS) $(ALLCPPSOBJ)
	$(COMPILING_TEXT_OK)
	$(JUMP_LINE)
	$(LINKING_TEXT)
	$(CC) -o $(NAME_EXE) $(ALLCPPSOBJ) $(INCLUDE) $(LIBS) $(CXXFLAGS) -static-libstdc++ -static-libgcc
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
