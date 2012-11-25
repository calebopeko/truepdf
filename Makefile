CXX=g++
SRC=$(shell ls *.cc)
OBJ=$(SRC:%.cc=%.o)
CORE=truepdf
INC=
LIB=
FLAGS=
CFLAGS=$(FLAGS) -Wall -O3

$(CORE): $(OBJ)
	$(CXX) $(OBJ) $(LIB) -o $(CORE)

$(OBJ): %.o: %.cc
	$(CXX) $(INC) $(CFLAGS) -c $<

clean:
	rm -rf $(CORE) $(OBJ)

new: clean $(CORE)
