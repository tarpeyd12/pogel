INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

COMPILERFLAGS = -Wall -O3
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
#LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
#LIBRARIES = -lX11 -lglut -lGL -lGLU -lm
LIBRARIES = -lX11 -lglut -lGLU -lGL

EXTENTION = cpp
OUTPUT = bin/ogl

LIBOUT = bin/libpogel

POGEL_PHYSICS = pogel/classes/physics/physics.o pogel/classes/physics/solid_class.o pogel/classes/physics/dynamics_class.o pogel/classes/physics/simulation_class.o pogel/classes/physics/singularity_class.o pogel/classes/physics/fountain_class.o pogel/classes/physics/microcosm_class.o
POGEL_SUPPORT = pogel/classes/image_class.o pogel/classes/quat_class.o pogel/classes/matrix_class.o pogel/classes/point_class.o pogel/classes/triangle_class.o pogel/classes/bounding_class.o
POGEL_OBJECT = pogel/classes/object_class.o pogel/classes/object_funcs.o pogel/classes/fractal_class.o pogel/classes/sprite_class.o

POGEL = pogel/pogel.o $(POGEL_SUPPORT) $(POGEL_OBJECT) $(POGEL_PHYSICS)

OTHER = main.o window.o 

TESTOBJS = condtree.o cosm.o lightning.o orbit.o simulation.o tree.o
#scene.o simulation_2.o distchk.o in.o 

#OBJ =  $(POGEL) $(OTHER) lightning.o
OBJ =  $(OTHER) lightning.o

all: lib bins test

clean: clean_pogel clean_lib clean_test clean_bins


$(OTHER) : $(patsubst %.o,%.cpp,$(OTHER))
	@echo "\033[32mCompiling File: \033[34m\"$@\"\033[31m"
	@$(CC) $(CFLAGS) -c -o $@ $(patsubst %.o,%.cpp,$@)
#	@echo "\033[0m"

$(POGEL) : $(patsubst %.o,%.cpp,$(POGEL))
	@echo "\033[32mCompiling File: \033[34m\"$@\"\033[31m"
	@$(CC) $(CFLAGS) -c -o $@ $(patsubst %.o,%.cpp,$@)
#	@echo "\033[0m"

pogel: $(POGEL)
	@echo "\033[0m"

clean_pogel:
	@echo "Removing the Compiled Code Files ..."
	@rm $(POGEL)


#for compiling a seperate lib file

POGELSRC := $(patsubst %.o,%.$(EXTENTION),$(POGEL))

POGELOBJ : $(POGELSRC)
	@echo "\033[32mCompiling File: \033[34m\"$@\"\033[31m"
	@$(CC) $(CFLAGS) -c -o $@ $(patsubst %.o,%.cpp,$@)

LIBPOGELOBJ := $(POGELSRC:%.cpp=%.o)

$(LIBOUT) : $(LIBPOGELOBJ)
	@echo "\n\033[32mBuilding Library \033[34m\"$@\"\033[31m"
	@ar cq $(LIBOUT).a $(LIBPOGELOBJ)
#	ld -G $(LIBPOGELOBJ).so -o $@

lib: $(POGELSRC) $(POGELOBJ) $(LIBOUT)
	@echo "\033[0m"

clean_lib: 
	@echo "Removing the Library File ..."
	@rm $(LIBOUT).a



# for the stuff Im working on

test: $(SRC) $(OBJ) $(OUTPUT)
	@echo "\033[0m"

SRC := $(patsubst %.o,%.$(EXTENTION),$(OBJ))

%(OBJ) : $(SRC)
	@echo "\033[32mCompiling File: \033[34m\"$@\"\033[31m"
	@$(CC) $(CFLAGS) -c -o $@ $(patsubst %.o,%.cpp,$@)

$(OUTPUT) : $(OBJ)
	@echo "\033[32mBuilding Binary: \033[34m\"$@\"\033[31m"
	@$(CC) $(LIBDIR) -L./bin/ $(OBJ) $(LIBRARIES) -lpogel -o $@
#	$(CC) $(LIBDIR) $(OBJ) $(LIBRARIES) -o $@

clean_test:
	@echo "Removing Main Test Binary ..."
	@rm $(OBJ) $(OUTPUT)



# make all the test files into seperate rograms

TESTBINARYS := $(patsubst %.o,%,$(TESTOBJS))
BINSRC := $(patsubst %.o,%.$(EXTENTION),$(TESTOBJS))


$(TESTOBJS) : $(BINSRC)
	@echo "\033[32mCompiling File: \033[34m\"$@\"\033[31m"
	@$(CC) $(CFLAGS) -c -o $@ $(patsubst %.o,%.cpp,$@)

$(TESTBINARYS) : $(TESTOBJS) $(OTHER) lib
	@echo "\033[32mBuilding Binary: \033[34m\"$@\"\033[31m"
	@$(CC) $(LIBDIR) -L./bin/ $(OTHER) $(patsubst %,%.o,$@) $(LIBRARIES) -lpogel -o bin/$@

bins: lib $(TESTOBJS) $(TESTBINARYS) $(OTHER)
	@echo "\033[0m"

clean_bins:
	@echo "Removing Test Binary Files ..."
	@rm $(TESTOBJS) $(patsubst %,bin/%,$(TESTBINARYS))




# for running the test code
run: test
	./run.sh

edit:$(SRC)
	gedit $(SRC)

resetcolor:
	@echo "\033[0m"

