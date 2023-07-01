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
HEADER_FILES = Game.h Dice.h Attributes.h Weapon.h MemeFighter.h
SOURCE_FILES = Main.cpp Game.cpp MemeFighter.cpp
OBJS = $(patsubst %.cpp,$(BUILDDIR)%.o,$(SOURCE_FILES))

debug:  CXXFLAGS = $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG)
debug: $(PROJECT)
	mkdir -p $(BUILDDIR)

$(PROJECT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(BUILDDIR)$(PROJECT) $(LIBS)

$(BUILDDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test:
	$(BUILDDIR)$(PROJECT)

run: debug test	

clean:
	rm -rf $(BUILDDIR)*

