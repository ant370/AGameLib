 
INCLUDE = -I Libs/SDL2-2.0.9/x86_64-w64-mingw32/include/SDL2
INCLUDE +=  -I Headers 
INCLUDE +=  -I Libs/glew-2.1.0/include/GL

LOCALLIBDIR = -L C:\Users\antmm\LibJam\AGame\AGame\Libs\SDL2-2.0.9\x86_64-w64-mingw32\lib
LOCALLIBDIR += -L C:\Users\antmm\LibJam\AGame\AGame\Libs\glew-2.1.0\lib\Release\x64
LDFLAGS = $(LOCALLIBDIR) 
LDLIBS  = -lSDL2 -lSDL2main -lmingw32 -lglew32 -lopengl32 

CC = C:\msys64\mingw64\bin\gcc
SRC_DIR = Source
OBJ_DIR = OBJ
SRC_FILES = $(wildcard $(SRC_DIR)/**/*.c) 
OBJECTS:= $(patsubst %.c, %.o, $(SRC_FILES)) 
 
main.exe: $(OBJECTS)
	$(CC) -g -o main.exe  $^ $(LDFLAGS) $(LDLIBS)  $(INCLUDE) 

$(OBJECTS) :  %.o : %.c
	$(CC) -g  -c $< -o $@  $(INCLUDE)  $(LDLIBS) 
