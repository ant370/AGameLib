 
INCLUDE = -I Libs/SDL2-2.0.9/x86_64-w64-mingw32/include/SDL2
INCLUDE +=  -I Headers 
INCLUDE +=  -I Libs/glew-2.1.0/include/GL

LOCALLIBDIR = -L C:\Users\antmm\LibJam\AGame\AGame\Libs\SDL2-2.0.9\x86_64-w64-mingw32\lib
LOCALLIBDIR += -L C:\Users\antmm\LibJam\AGame\AGame\Libs\glew-2.1.0\lib\Release\x64
LDFLAGS = $(LOCALLIBDIR) 
LDLIBS  = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglew32


SRC_DIR = Source
OBJ_DIR = OBJ
SRC_FILES = $(wildcard $(SRC_DIR)/**/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c ,$(OBJ_DIR)/%.o, $(SRC_FILES)) 
  
  
subdirs := $(wildcard */)
sources := $(wildcard $(addsuffix *.c,$(subdirs)))
objects := $(patsubst %.c,%.o,$(sources))

print:
	$(info VAR is $(SRC_FILES))

main.exe: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^ $(INCLUDE) $(LDLIBS)

$(objects) : %.o : %.c
	gcc  $(CXXFLAGS)  -c -o $@ $< $(INCLUDE)   $(LDLIBS)

	 
  
 