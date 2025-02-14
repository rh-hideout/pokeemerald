CXX ?= g++

CXXFLAGS := -Werror -std=c++17 -pthread -O2 -Wunused

INCLUDES := -I .

SRCS := compresSmol.cpp compressAlgo.cpp tANS.cpp fileDispatcher.cpp

HEADERS := compressAlgo.h tANS.h fileDispatcher.h

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

.PHONY: all clean

all: compresSmol$(EXE)
	@:

compresSmol$(EXE): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) compresSmol compresSmol.exe
