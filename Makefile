CXX=g++
SRC=$(shell ls *.cc)
OBJ=$(SRC:%.cc=%.o)
CORE=truepdf
INC=-I/usr/include/gtk-2.0 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/lib/i386-linux-gnu/gtk-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0 -I/usr/include/poppler/glib
SDLCONFIG_LIBS=$(shell sdl-config --libs)
SDLCONFIG_CFLAGS=$(shell sdl-config --cflags)
LIB=-lgtk-x11-2.0 -lpoppler-glib $(SDLCONFIG_LIBS)
FLAGS=$(SDLCONFIG_CFLAGS)
CFLAGS=$(FLAGS) -Wall -g

$(CORE): $(OBJ)
	$(CXX) $(OBJ) $(LIB) -o $(CORE)

$(OBJ): %.o: %.cc
	$(CXX) $(INC) $(CFLAGS) -c $<

clean:
	rm -rf $(CORE) $(OBJ)

new: clean $(CORE)
