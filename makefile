INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

COMPILERFLAGS = -Wall -O3
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
#LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
#LIBRARIES = -lX11 -lglut -lGL -lGLU -lm
LIBRARIES = -lX11 -lglut -lGLU -lGL -lpthread

EXTENTION = cpp
OUTPUT = bin/ogl

LIBOUT = bin/libpogel.a

POGEL_SIMULATION = pogel/classes/physics/simulation_class.o pogel/classes/physics/simulation_class_collisions.o pogel/classes/physics/simulation_class_collisions_reaction.o
POGEL_PHYSICS = pogel/classes/physics/physics.o pogel/classes/physics/solid_class.o pogel/classes/physics/dynamics_class.o $(POGEL_SIMULATION) pogel/classes/physics/singularity_class.o pogel/classes/physics/fountain_class.o pogel/classes/physics/microcosm_class.o
POGEL_SUPPORT = pogel/classes/image_class.o pogel/classes/view_class.o pogel/classes/quat_class.o pogel/classes/matrix_class.o pogel/classes/point_class.o pogel/classes/triangle_class.o pogel/classes/bounding_class.o
POGEL_OBJECT = pogel/classes/object_class.o pogel/classes/object_funcs.o pogel/classes/fractal_class.o pogel/classes/sprite_class.o

POGEL = pogel/pogel.o $(POGEL_SUPPORT) $(POGEL_OBJECT) $(POGEL_PHYSICS)

OTHER = main.o window.o 

TESTOBJS = condtree.o cosm.o lightning.o orbit.o simulation.o tree.o scene.o simulation_2.o distchk.o in.o sprite.o texrend.o orbittexrend.o simpsheer.o firelike.o function.o fract.o slingshot.o gravity.o loader.o viewport.o viewer.o

#OBJ =  $(POGEL) $(OTHER) lightning.o
OBJ =  $(OTHER) simulation.o

%.o : %.$(EXTENTION)
	@echo "\033[32mCompiling File: \033[34m\"$@\"\033[31m"
	@$(CC) $(CFLAGS) -c $? -o $@
	@echo -n "\033[0m"


all: pogel lib bins test

clean: clean_pogel clean_lib clean_test clean_bins

POGELSRC := $(patsubst %.o,%.$(EXTENTION),$(POGEL))



# to compile the main sections of code without too much hassle
pogel: $(POGEL)
	@make -s $(POGEL)
	@echo -n "\033[0m"

clean_pogel:
	@echo "Removing the Compiled Code Files ..."
	-@rm $(wildcard $(POGEL) )


#for compiling a seperate lib file

#LIBPOGELOBJ := $(POGELSRC:%.cpp=%.o)

$(LIBOUT) : clean_lib pogel
	@echo "\n\033[32mBuilding Library \033[34m\"$@\"\033[31m"
	@ar cq $(LIBOUT) $(POGEL)
	@echo "\033[0m"
#	ld -G $(LIBPOGELOBJ) -o $@.so

#	@-rm $(wildcard $(LIBOUT))

lib: pogel $(LIBOUT)
	@echo -n "\033[0m"

clean_lib: 
	@echo "Removing the Library File ..."
	-@rm $(wildcard $(LIBOUT))



# for the stuff Im working on

SRC := $(patsubst %.o,%.$(EXTENTION),$(OBJ))

$(OUTPUT) : lib $(OBJ)
	@echo "\033[32mBuilding Binary: \033[34m\"$@\"\033[31m"
	@$(CC) $(LIBDIR) -L./bin/ $(OBJ) $(LIBRARIES) -lpogel -o $@
	@echo -n "\033[0m"
#	$(CC) $(LIBDIR) $(OBJ) $(LIBRARIES) -o $@

test: lib $(OUTPUT)
	@echo -n "\033[0m"

clean_test:
	@echo "Removing Main Test Binary ..."
	-@rm $(wildcard $(OTHER) $(OUTPUT))



# make all the test files into seperate rograms

TESTBINARYS := $(patsubst %.o,%,$(TESTOBJS))

$(TESTBINARYS) : lib $(OTHER) $(TESTOBJS)
	@echo "\033[32mBuilding Binary: \033[34m\"$@\"\033[31m"
	@$(CC) $(LIBDIR) -L./bin/ $(OTHER) $(patsubst %,%.o,$@) $(LIBRARIES) -lpogel -o bin/$@
	@echo -n "\033[0m"

bins: lib $(TESTOBJS) $(TESTBINARYS) $(OTHER)
	@echo -n "\033[0m"

clean_bins:
	@echo "Removing Test Binary Files ..."
	-@rm $(wildcard $(TESTOBJS) $(patsubst %,bin/%,$(TESTBINARYS)))

# in pogel_internals.h NO_OPENGL must be defined
#  to build do make clean pogel lib spec_bin
spec_bin:
	@echo "Building special binary ..."
	@$(CC) $(LIBDIR) -L./bin/ quietsim.cpp -lpogel -lpthread -o bin/sb


# for running the test code
run: test
	@echo "\n\033[35mExecuting the test binary ... \033[0m\n"
	@./run.sh

edit:$(SRC)
	gedit $(SRC)


# incase the prety colors spill out of the paint bucket
resetcolor:
	@echo "\033[0m"

# just cause I like things clean
clear:
	clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear && clear

todo:
	-@for file in $(POGELSRC); do fgrep -H -e TODO -e FIXME $$file; done; true


