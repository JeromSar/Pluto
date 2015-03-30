default: all
.PHONY: all dirs deploy clean

# Build target
TARGET = pluto

# Directories
SRC_DIR = src
#GEN_DIR = gen
BUILD_DIR = build
DIST_DIR = dist

# Define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -c 	   compiles to an unlinked object, instead of an executable
#  -g      generates debug information
#  -Wall   turns on most, but not all, compiler warnings
#  -Wextra turns on extra compiler warnings
#  -O3     sets optimization level
#  -I.     adds the current working directory to the include path
#
CC = g++
CFLAGS = -O0 -g -Wall -Wextra -std=c++11 -I. -c
LFLAGS = -g
CFLAGS_DEPLOY = -O3
LFLAGS_DEPLOY = -static-libgcc -static-libstdc++

# No config beyond this line

# Define sources and objects
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(subst $(SRC_DIR)/,,$(SOURCES:.cpp=.o)))

# Sources location
vpath %.cpp $(SRC_DIR)

# Targets beyond this line

# Target - dirs
dirs:
	@mkdir -p $(BUILD_DIR) $(DIST_DIR)

# Target - deploy
deploy: all

# Target - all
all: dirs $(OBJECTS)
ifeq ($(MAKECMDGOALS),deploy)
	$(CC) $(LFLAGS) $(LFLAGS_DEPLOY) -o $(DIST_DIR)/$(TARGET) $(OBJECTS)
else
	$(CC) $(LFLAGS) -o $(DIST_DIR)/$(TARGET) $(OBJECTS)
endif

# Target - .cpp file
$(BUILD_DIR)/%.o: %.cpp
ifeq ($(MAKECMDGOALS),deploy)
	$(CC) $(CFLAGS) $(CFLAGS_DEPLOY) $< -o $@
else
	$(CC) $(CFLAGS) $< -o $@
endif

# Target - clean
clean:
	$(RM) -R $(BUILD_DIR)
