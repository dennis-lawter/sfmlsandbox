rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

EXECUTABLE=sfmlsandbox

CC=C:/cppdev/mingw-w64-7.3.0/mingw64/bin/g++.exe
CFLAGS=-O0 -Wall -c -fmessage-length=0 -std=c++17 -Wl,--subsystem,windows
LIBRARYDIR="-LC:/cppdev/SFML-2.5.1/lib"
LIBRARIES=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
INCLUDEDIR="-IC:/cppdev/SFML-2.5.1/include"

dir_guard=mkdir -p $(@D)

SRC_DIR=src/
DEBUG_OBJ_DIR=Obj_Debug/
DEBUG_OUT_DIR=Debug/
DLLS=\
"C:/cppdev/SFML-2.5.1/bin/sfml-audio-2.dll" \
"C:/cppdev/SFML-2.5.1/bin/sfml-graphics-2.dll" \
"C:/cppdev/SFML-2.5.1/bin/sfml-system-2.dll" \
"C:/cppdev/SFML-2.5.1/bin/sfml-window-2.dll" \
"C:/cppdev/SFML-2.5.1/bin/openal32.dll"


CPPSOURCES=$(call rwildcard,src/,*.cpp)
CCSOURCES=$(call rwildcard,src/,*.cc)
SOURCES= $(CPPSOURCES) $(CCSOURCES)

TEMP=$(SOURCES:.cpp=.o)
OBJECTS=$(TEMP:.cc=.o)
OBJECTSOUT=$(OBJECTS:$(SRC_DIR)%=$(DEBUG_OBJ_DIR)%)

clean_build_run: clean all run

build_run: all run

all: $(DEBUG_OUT_DIR)$(EXECUTABLE)

run:
	$(DEBUG_OUT_DIR)$(EXECUTABLE).exe

$(DEBUG_OUT_DIR)$(EXECUTABLE): $(OBJECTSOUT)
	$(dir_guard)
	@echo 'Building executable: $@'
	$(CC) $(LIBRARYDIR) -o $@ $(OBJECTSOUT) $(LIBRARIES)
	cp -R res/resource $(DEBUG_OUT_DIR)
	cp $(DLLS) $(DEBUG_OUT_DIR)

$(DEBUG_OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(dir_guard)
	@echo 'Building file: $@'
	$(CC) $(LIBRARIES) $(CFLAGS) $(INCLUDEDIR) -c $< -o $(@:src/%=debug/%)

$(DEBUG_OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(dir_guard)
	@echo 'Building file: $@'
	$(CC) $(LIBRARIES) $(CFLAGS) $(INCLUDEDIR) -c $< -o $(@:src/%=debug/%)

clean:
	rm -rf $(DEBUG_OUT_DIR)
	rm -rf $(DEBUG_OBJ_DIR)
