CC=gcc
CXX=g++

RM=rm -f

CFLAGS=-g $(shell root-config --cflags)
CXXFLAGS=-g
CPPFLAGS=-Wall
LDFLAGS=
LDLIBS=

SRCS=src/main.cpp 
OBJS=$(subst .ccp,.o,$(SRCS))
TARGETS=src/main

all: $(TARGETS)

src/main: $(OBJS)
	$(CXX) $(LDFLAGS) -o build/main $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM &^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend