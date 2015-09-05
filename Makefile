default: all
.PHONY: all dirs icon deploy clean

# Build target
TARGET = pluto

# Directories
SRC_DIR = src
ASSETS_DIR = assets
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
CFLAGS_DEPLOY = -O3 -std=c++11 -I. -c
LFLAGS_DEPLOY = -static-libgcc -static-libstdc++

# No config beyond this line

# Define sources and objects
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(subst $(SRC_DIR)/,,$(SOURCES:.cpp=.o)))

ifeq ($(MAKECMDGOALS),deploy)
COMPILE_FLAGS = $(CFLAGS_DEPLOY)
else
COMPILE_FLAGS = $(CFLAGS)
endif

# Sources location
vpath %.cpp $(SRC_DIR)

# Targets beyond this line

# Target - dirs
dirs:
	@mkdir -p $(BUILD_DIR) $(DIST_DIR)

# Target - icon
icon: $(BUILD_DIR)/$(TARGET).res

$(BUILD_DIR)/$(TARGET).res: $(ASSETS_DIR)/$(TARGET).rc
	windres $(ASSETS_DIR)/$(TARGET).rc -O coff -o $(BUILD_DIR)/$(TARGET).res
	
# Target - deploy
deploy: dirs icon $(OBJECTS)
	$(CC) $(LFLAGS_DEPLOY) -o $(DIST_DIR)/$(TARGET)-dist $(OBJECTS) $(BUILD_DIR)/$(TARGET).res

# Target - all
all: dirs $(OBJECTS)
	$(CC) $(LFLAGS) -o $(DIST_DIR)/$(TARGET) $(OBJECTS)

# Target - .cpp file
$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(COMPILE_FLAGS) $< -o $@

# Target - clean
clean:
	$(RM) -R $(BUILD_DIR)
