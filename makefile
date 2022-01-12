CXXFLAGS := -std=c++20 -Wall -Wfatal-errors
DB =
LIBS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
INCLUDEDIR := -I includes

EXE := $(patsubst %.cpp,%,$(wildcard *.cpp))

all: $(EXE)


%: %.cpp includes/glad.c includes/stb_image.cpp
	g++ $(INCLUDEDIR) $(CXXFLAGS) $(DB) $^ $(LIBS) -o $@

.PHONY : clean

clean :
	rm -f $(EXE)
