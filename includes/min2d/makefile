CXXFLAGS := -std=c++20 -Wall -Wfatal-errors
DB =
LIBS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

EXE := $(patsubst %.cpp,%,$(wildcard *.cpp))

all: $(EXE)


%: %.cpp glad.c
	g++ $(CXXFLAGS) $(DB) $^ $(LIBS) -o $@

.PHONY : clean

clean :
	rm -f $(EXE)
