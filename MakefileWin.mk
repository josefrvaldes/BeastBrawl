COMPILING_TEXT 	   := @echo -e "\033[0m \033[0;33m Compiling:\033[0m"
COMPILING_TEXT_OK  := @echo -e "\033[0m \033[0;32m [All compiled succesfully]\033[0m"
LINKING_TEXT       := @echo -e "\033[0m \033[0;33m Linking...\033[0m"
LINKING_TEXT_OK    := @echo -e "\033[0m \033[0;32m [Linked succesfully]\033[0m"
ALL_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [Cleaned succesfully]\033[0m"
CLENGINE_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [CLEngine cleaned succesfully]\033[0m"
SRC_CLEANED_TEXT   := @echo -e "\033[0m \033[0;32m [SRC cleaned succesfully]\033[0m"
#EXECUTING_TEXT     := @echo -e "\033[0m \033[5;32m Executing...\033[0m"
JUMP_LINE		   := @echo



SOURCES  	:= $(wildcard *.cpp)

NAME_EXE	:= Beast_Brawl.exe
CXXFLAGS 	+= -Wall -Wno-unknown-pragmas -std=gnu++1z -static-libgcc -static-libstdc++ # el no-unknown-pragmas es para que no salga el warning de los pragma region
																										 # el -fuse-ld=gold es para el ccache
																										 # si pongo -std=c++17 falla M_PI, poniendo -std=gnu++17 funciona bien en windows

# Objs del juego

OBJ_PATH    := obj/src
SRC_PATH	:= src

ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCPPSOBJ	:= $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(ALLCPPS))
SUBDIRS		:= $(shell find src/ -type d)
OBJSUBDIRS  := $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SUBDIRS))

# Objs de CLEngine

OBJ_CLE_PATH    := obj/CLEngine
SRC_CLE_PATH  	:= CLEngine/src
MAIN_CLE		:= obj/CLEngine/main.o

ALLCPPS_CLE    	:= $(shell find CLEngine/src/ -type f -iname *.cpp)							# Busca todos los archivos '.cpp' a partir de la carpeta seleccionada
ALLCPPSOBJ_CLE  := $(patsubst $(SRC_CLE_PATH)/%.cpp,$(OBJ_CLE_PATH)/%.o,$(ALLCPPS_CLE))		# Cambia los 'src' por 'obj' y los '.cpp' por '.o'
ALLCPPSOBJ_CLE  := $(filter-out $(MAIN_CLE),$(ALLCPPSOBJ_CLE))								# >>> Filtra el main.o de CLEngine y lo quita de la variable
SUBDIRS_CLE    	:= $(shell find CLEngine/src/ -type d)										# Busca y obtiene los subdirectorios de ClEngine/src
OBJSUBDIRS_CLE  := $(patsubst $(SRC_CLE_PATH)%,$(OBJ_CLE_PATH)%,$(SUBDIRS_CLE))				# Cambia los 'src' por 'obj'


ifdef DEBUG
	CXXFLAGS += -g
else
	CXXFLAGS += -O3
endif


# gráficos
LIBS 	    	+= -L./CLEngine/lib/windows/glfw -lglfw3 -lglfw3dll -lopengl32 -lgdi32 -luser32 -lcomdlg32 -lpthread -Wl,-rpath=./CLEngine/lib/windows/glfw
LIBS 	    	+= -L./CLEngine/lib/windows/glew -lglew32 -Wl,-rpath=./CLEngine/lib/windows/glew
LIBS 	    	+= -L./CLEngine/lib/windows/assimp -lassimp -Wl,-rpath=./CLEngine/lib/windows/assimp
LIBS			+= -L./CLEngine/lib/windows/freeType2 -lfreetype -Wl,-rpath=./CLEngine/lib/windows/freeType2

# irrlicht
LIBS 	    	+= -L./lib/windows/irrlicht -lIrrlicht -Wl,-rpath=lib/windows/irrlicht

# fmod
LIBS 	    	+= -L./lib/windows/fmod -lfmod -lfmodL -lfmodstudio -lfmodstudioL -Wl,-rpath=lib/windows/fmod

# asio 
LIBS 			+= -lws2_32 -lwsock32 # necesarias para asio

INCLUDE     := -I./include -I./src -I./include/fmod/core -I./include/fmod/studio -I./include/freeType2
CREATE_SYMLINKS := 
CC			:= x86_64-w64-mingw32-g++-posix # sin posix no podemos utilizar thread ni mutex
#CC			:= x86_64-w64-mingw32-g++



#Esto crea el ejecutable
$(NAME_EXE): $(OBJSUBDIRS) $(ALLCPPSOBJ) $(OBJSUBDIRS_CLE) $(ALLCPPSOBJ_CLE)
	$(CREATE_SYMLINKS)
	$(COMPILING_TEXT_OK)
	$(JUMP_LINE)
	$(LINKING_TEXT)
	$(CC) -o $(NAME_EXE) $(ALLCPPSOBJ) $(ALLCPPSOBJ_CLE) $(INCLUDE) $(LIBS) $(CXXFLAGS) 
	$(LINKING_TEXT_OK)
	$(JUMP_LINE)


#Esto compila y crea los .o (aqui aun no entra en juego el linker)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(PRUEBA_TEXT)
	$(COMPILING_TEXT) $<
	@$(CC) $(CXXFLAGS) -o $@ -c $^ $(INCLUDE)
	

$(OBJSUBDIRS):
	@mkdir -p $(OBJSUBDIRS)

.PHONY: gl
gl:
	$(MAKE) -C CLEngine/

info:
	$(info $(ALLCPPSOBJ_CLE))

.PHONY: exe_gl
exe_gl:
	@sh ./CLEngine.sh

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
