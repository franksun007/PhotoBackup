LOCAL_DIR=${PWD}/.local

# define the commands we will use for compilation and library building
AR 				:= ar
ARFLAGS 		:= rcs
CXX 			:= c++
CFLAGS 			:= -g -Wall -O3 -std=c++11 -Wpedantic
GTESTFLAGS 	:= -L./gtest -lgtest

SRC_DIR := src
TEST_DIR := test
INCLUDE_DIR := include
BUILDDIR := build
BUILD_TEST_DIR := build_test

TARGET := bin/PhotoBackup
TEST := bin/Test
SRC_LIB := include/srclib.a

# Using boost by homebrew
BOOST_VERSION := 1.63.0
BOOST_LIB := -L/usr/local/Cellar/boost/${BOOST_VERSION}/lib
BOOST_LK := -I/usr/local/Cellar/boost/${BOOST_VERSION}/include
BOOST := -lboost_system -lboost_filesystem -lboost_program_options

LIB := $(BOOST_LIB) $(BOOST) -L./include
LDFLAGS := $(BOOST_LK) -I include 

CPP_EXT := cpp
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.$(CPP_EXT)")
TESTS := $(shell find $(TEST_DIR) -type f -name "*.$(CPP_EXT)")


OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(CPP_EXT)=.o))
TESTOBJS := $(patsubst $(TEST_DIR)/%,$(BUILD_TEST_DIR)/%,$(TESTS:.$(CPP_EXT)=.o))
HEADERS := $(INCLUDE_DIR/%.h)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIB)"; 
	$(CXX) $^ -o $(TARGET) $(LIB) $(LDFLAGS)


$(BUILDDIR)/%.o: $(SRC_DIR)/%.$(CPP_EXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $<"; 
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

$(SRC_LIB): $(OBJECTS) $(HEADERS)
	$(AR) $(ARFLAGS) $(SRC_LIB) $(OBJECTS)

$(TEST): $(TESTOBJS) $(SRC_LIB)
	@echo "Linking Tests..."
	@echo " $(CXX) $^ -o $(TEST) $(LIB) $(LDFLAGS) $(GTESTFLAGS)"; 
	$(CXX) $^ -o $(TEST) $(LIB) $(LDFLAGS) $(GTESTFLAGS)


$(BUILD_TEST_DIR)/%.o: $(TEST_DIR)/%.$(CPP_EXT) 
	@mkdir -p $(BUILD_TEST_DIR)
	@echo " $(CXX) $(CFLAGS) $(LDFLAGS) $(GTESTFLAGS) -c -o $@ $<"; 
	$(CXX) $(CFLAGS) $(LDFLAGS) $(GTESTFLAGS) -c -o $@ $<


test: $(TEST)


all: $(TARGET) 
	# make $(TEST)

clean:
	@echo "Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(BUILD_TEST_DIR) $(TARGET) $(TEST) $(SRC_LIB)"; 
	$(RM) -r $(BUILDDIR) $(BUILD_TEST_DIR) $(TARGET) $(TEST) $(SRC_LIB)

.PHONY: clean

