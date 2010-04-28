INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

COMPILERFLAGS = -Wall -O3
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
#LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
#LIBRARIES = -lX11 -lglut -lGL -lGLU -lm
LIBRARIES = -lX11 -lglut -lGL -lpthread

EXTENTION = cpp
OUTPUT = bin/ogl

POGEL_PHYSICS = pogel/classes/physics/physics.o pogel/classes/physics/solid_class.o pogel/classes/physics/dynamics_class.o pogel/classes/physics/simulation_class.o pogel/classes/physics/singularity_class.o pogel/classes/physics/fountain_class.o pogel/classes/physics/microcosm_class.o
POGEL_SUPPORT = pogel/classes/image_class.o pogel/classes/quat_class.o pogel/classes/matrix_class.o pogel/classes/point_class.o pogel/classes/triangle_class.o pogel/classes/bounding_class.o
POGEL_OBJECT = pogel/classes/object_class.o pogel/classes/object_funcs.o pogel/classes/fractal_class.o pogel/classes/sprite_class.o

POGEL = pogel/pogel.o $(POGEL_SUPPORT) $(POGEL_OBJECT) $(POGEL_PHYSICS)

OTHER = main.o condtree.o window.o 

OBJ =  $(POGEL) $(OTHER)

SRC := $(patsubst %.o,%.$(EXTENTION),$(OBJ))

all: $(SRC) $(OBJ) $(OUTPUT)

clean:
	rm $(OBJ)

%(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -o $@

$(OUTPUT) : $(OBJ)
	$(CC) $(LIBDIR) $(OBJ) $(LIBRARIES) -o $(OUTPUT)

run:all
	./run.sh

edit:$(SRC)
	gedit $(SRC)

