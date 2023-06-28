PROJECT = Inheritance

# CXX = g++
CC = zig cc
CXX = zig c++
CXXFLAGS_COMMON = -std=gnu++2b -Werror -Wall -Wextra -fsanitize=undefined,address
CXXFLAGS_RELEASE = -O2
CXXFLAGS_DEBUG = -g -O0
LDFLAGS = -Bstatic
LIBS = 

BUILDDIR = build/
HEADER_FILES = 
SOURCE_FILES = Main

$(SOURCE_FILES): CXXFLAGS = $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG)
$(BUILDDIR)Main: Main.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(BUILDDIR)$(PROJECT) $(LIBS)

test:
	$(BUILDDIR)$(PROJECT)

clean:
	rm -rf $(BUILDDIR)*

