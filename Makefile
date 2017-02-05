LOCAL_DIR=${PWD}/.local

CXX:=c++
CFLAG:=-g -Wall -O3 -std=c++11

EXEC:=PhotoBackup

# Using boost by homebrew
BOOST_VERSION := 1.63.0
BOOST_LIB := -L/usr/local/Cellar/boost/${BOOST_VERSION}/lib
BOOST_LK := -I/usr/local/Cellar/boost/${BOOST_VERSION}/include
BOOST := -lboost_system -lboost_filesystem -lboost_program_options

all: PhotoBackup.o CopyRecentDetails.o CommandLineEssential.o
	$(CXX) $(CFLAG) $(BOOST_LIB) $(BOOST_LK) $(BOOST) PhotoBackup.o CopyRecentDetails.o CommandLineEssential.o -o $(EXEC)

CopyRecentDetails.o: CopyRecentDetails.cpp CopyRecentDetails.h
	$(CXX) $(CFLAG) -c CopyRecentDetails.cpp

CommandLineEssential.o: CommandLineEssential.cpp CommandLineEssential.h
	$(CXX) $(CFLAG) -c CommandLineEssential.cpp

PhotoBackup.o: PhotoBackup.cpp CopyRecentDetails.o CommandLineEssential.o
	$(CXX) $(CFLAG) -c PhotoBackup.cpp 


clean:
	rm $(EXEC) *.o
