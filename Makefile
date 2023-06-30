PROJECT = Inheritance

# CXX = g++
CC = zig cc
CXX = zig c++
CXXFLAGS_COMMON = -std=gnu++2b -Wall -Wextra
CXXFLAGS_RELEASE = -O2
CXXFLAGS_DEBUG = -g -O0
LDFLAGS = -Bstatic
LIBS = 

BUILDDIR = build/
HEADER_FILES = 
SOURCE_FILES = Main.cpp

debug:  CXXFLAGS = $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG)

debug: $(SOURCE_FILES)
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(BUILDDIR)$(PROJECT) $(LIBS)

test:
	$(BUILDDIR)$(PROJECT)

run: debug test	

clean:
	rm -rf $(BUILDDIR)*

