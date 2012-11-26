CXX=g++
SRC=$(shell ls *.cc)
OBJ=$(SRC:%.cc=%.o)
CORE=truepdf
INC=
SDLCONFIG_LIBS=$(shell sdl-config --libs)
SDLCONFIG_CFLAGS=$(shell sdl-config --cflags)
LIB=$(SDLCONFIG_LIBS) $(shell pkg-config --libs poppler-glib gtk+-2.0)
FLAGS=$(SDLCONFIG_CFLAGS) $(shell pkg-config --cflags poppler-glib gtk+-2.0)
CFLAGS=$(FLAGS) -Wall -g -O3

$(CORE): $(OBJ)
	$(CXX) $(OBJ) $(LIB) -o $(CORE)

$(OBJ): %.o: %.cc
	$(CXX) $(INC) $(CFLAGS) -c $<

clean:
	rm -rf $(CORE) $(OBJ)

new: clean $(CORE)
