#Copyright (c) 2011-2017 Hiroshi Tsubokawa
#See LICENSE and README

CC = g++
OPT = -O3 -std=c++03
CFLAGS = $(OPT) -Wall
LDFLAGS = -lscene -lm
RM = rm -f

CFLAGS  += $(shell pkg-config --cflags OpenEXR)
LDFLAGS += $(shell pkg-config --libs OpenEXR)

topdir      := ../..
target_dir  := bin
target_name := fb2exr
files       := fb2exr

incdir  := $(topdir)/src
libdir  := $(topdir)/lib
target  := $(topdir)/$(target_dir)/$(target_name)

sources := $(addsuffix .cc, $(files))
objects := $(addsuffix .o, $(files))
depends := $(addsuffix .d, $(files))

.PHONY: all clean depend
all: $(target)

$(objects): %.o: %.cc
	@echo '  compile $<'
	@$(CC) $(CFLAGS) -I$(incdir) -c -o $@ $<

$(target): $(objects)
	@echo '  link $(target_name)'
	@$(CC) -o $@ $^ -L$(libdir) $(LDFLAGS)

$(depends): %.d: %.cc
	@echo '  dependency $<'
	@$(CC) $(CFLAGS) -I$(incdir) -c -MM $< > $@

clean:
	@echo '  clean $(target_name)'
	@$(RM) $(target) $(objects) $(depends) 

ifneq "$(MAKECMDGOALS)" "clean"
-include $(depends)
endif

