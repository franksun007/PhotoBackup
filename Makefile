LOCAL_DIR=${PWD}/.local

CXX := c++
CFLAGS := -g -Wall -O3 -std=c++11

SRCDIR := src
BUILDDIR := build

TARGET:=bin/PhotoBackup

# Using boost by homebrew
BOOST_VERSION := 1.63.0
BOOST_LIB := -L/usr/local/Cellar/boost/${BOOST_VERSION}/lib
BOOST_LK := -I/usr/local/Cellar/boost/${BOOST_VERSION}/include
BOOST := -lboost_system -lboost_filesystem -lboost_program_options

LIB := $(BOOST_LIB) $(BOOST) 
INC := $(BOOST_LK) -I include

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))


$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@echo "$(CXX) $^ -o $(TARGET) $(BOOST_LIB)"; $(CXX) $^ -o $(TARGET) $(LIB) $(INC)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

