#-----------------------------------------------------------------------------
# z80asm restart
# Copyright (C) Paulo Custodio, 2011-2018
# License: http://www.perlfoundation.org/artistic_license_2_0
#-----------------------------------------------------------------------------
PROJ		:= z80asm

ifeq ($(OS),Windows_NT)
  EXE		:= .exe
else
  EXE 		:= $(EXESUFFIX)
endif

# UNIXem is needed in both Windows_NT and in a MinGW build in Unix
ifeq ($(EXE),.exe)
  UNIXem_CFLAGS	:= -I../../ext/UNIXem/include
  UNIXem_SRCS	:= ../../ext/UNIXem/src/glob.c \
				   ../../ext/UNIXem/src/dirent.c
  UNIXem_OBJS	:= $(UNIXem_SRCS:.c=.o)
  REGEX_SRCS	:= $(wildcard ../../ext/regex/reg*.c)
  REGEX_OBJS	:= $(REGEX_SRCS:.c=.o)
endif

#-----------------------------------------------------------------------------
# tools
#-----------------------------------------------------------------------------
CC 			?= gcc
CXX			?= g++
INSTALL 	?= install
CROSS		?= 0

VALGRIND	:= valgrind -q --tool=memcheck --leak-check=full \
			   --track-fds=yes --error-exitcode=1

ASTYLE		:= astyle --style=linux --break-blocks --pad-oper \
		       --align-pointer=type --break-closing-braces \
		       --break-one-line-headers --remove-braces \
		       --attach-return-type \
		       --convert-tabs --max-code-length=80 \
		       --formatted --lineend=linux \
		       --suffix=none

OPT 		= -O3
LOCAL_LIB 	:= lib
LOCAL_FLAGS += -g -Wall -Wextra \
			   -MMD $(OPT) \
			   -I. -I$(LOCAL_LIB) -It \
			   -I../common \
			   -I../../ext/optparse \
			   -I../../ext/regex \
			   -I../../ext/uthash/src \
			   $(UNIXem_CFLAGS)
LOCAL_CFLAGS	= -std=gnu11   $(LOCAL_FLAGS)
LOCAL_CXXFLAGS	= -std=gnu++11 $(LOCAL_FLAGS)

.c.o:
	$(CC)  -c $(CFLAGS)   $(LOCAL_CFLAGS)   -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(LOCAL_CXXFLAGS) -o $@ $<

#------------------------------------------------------------------------------
# Object files
#------------------------------------------------------------------------------
C_SRCS1		:= $(wildcard *.c lib/*.c ../common/*.c)
C_SRCS		:= $(filter-out test*.c,$(C_SRCS1)) \
			   $(UNIXem_SRCS) \
			   $(REGEX_SRCS)
OBJS		+= $(C_SRCS:.c=.o)
DEPENDS		+= $(C_SRCS:.c=.d)

CXX_SRCS	:= $(wildcard *.cpp)
OBJS 		+= $(CXX_SRCS:.cpp=.o)
DEPENDS		+= $(CXX_SRCS:.cpp=.d)

T_SRCS 		:= $(wildcard t/*.cpp)
T_OBJS		:= $(T_SRCS:.cpp=.o) \
			   $(OBJS:main.o=)
DEPENDS		+= $(T_SRCS:.cpp=.d)

TEST_SRC_MAIN	:= $(wildcard t/test_*.c)
TEST_SRC_LIB	:= t/testlib.c

#------------------------------------------------------------------------------
# how to build a C++ executable
#------------------------------------------------------------------------------
.PHONY: all clean test valgrind

define MAKE_EXE
all: $(1)$(EXE)

$(1)$(EXE): $(2)
	$(CXX) $(CXXFLAGS) $(LOCAL_CXXFLAGS) -o $(1)$(EXE) $(2) $(LDFLAGS)

clean::
	$(RM) $(1)$(EXE) $(2)

test:: $(1)$(EXE)
ifeq ($(3),1)
	./$(1)$(EXE)
endif

valgrind:: $(1)$(EXE)
	$(VALGRIND) $(1)$(EXE)
endef

#------------------------------------------------------------------------------
# main
#------------------------------------------------------------------------------
$(eval $(call MAKE_EXE,$(PROJ),$(OBJS),0))

$(PROJ)$(EXE): ../config.h

../config.h:
	@echo \#define PREFIX \"/usr/local/share/z88dk\" 				 > $@
	@echo \#define UNIX 1											>> $@
	@echo \#define EXEC_PREFIX \"\"									>> $@
	@echo \#define Z88DK_VERSION \"build-$(shell date +%Y%m%d)\"	>> $@

# parser is too big to optimize with -O3
parse.o: parse.c
	$(CC)  -c $(CFLAGS)   $(LOCAL_CFLAGS) -O0 -o $@ $<

#------------------------------------------------------------------------------
# test
#------------------------------------------------------------------------------
$(eval $(call MAKE_EXE,t/test,$(T_OBJS),1))

t/test.o: t/test.def

t/test.def: t/*.cpp Makefile t/make_tests.pl
	perl t/make_tests.pl

test:: $(PROJ)$(EXE)
	perl -S prove t/*.t

#------------------------------------------------------------------------------
# libraries
#------------------------------------------------------------------------------
.PHONY: z80asm_lib
all: z80asm_lib

z80asm_lib:
ifeq ($(CROSS),0)
	$(MAKE) -C dev/z80asm_lib
	cp dev/z80asm_lib/$(PROJ)-*.lib .
endif

clean::
	$(MAKE) -C dev/z80asm_lib clean
	$(RM) $(PROJ)-*.lib

#------------------------------------------------------------------------------
# developer generated source files, and test libs
#------------------------------------------------------------------------------
.PHONY: dev
dev:
	$(MAKE) -C tt
	$(MAKE) -C lib
	$(MAKE) -f dev/Makefile
	$(MAKE) all

clean::
	$(MAKE) -C lib clean

#------------------------------------------------------------------------------
# intall
#------------------------------------------------------------------------------
install:
	$(INSTALL) $(PROJ)$(EXE) $(PREFIX)/bin/$(EXEC_PREFIX)$(PROJ)$(EXE)
	$(INSTALL) asmpp.pl      $(PREFIX)/bin/asmpp.pl
	$(MAKE) -C dev/z80asm_lib install PREFIX=$(PREFIX_SHARE)

#------------------------------------------------------------------------------
# clean
#------------------------------------------------------------------------------
clean::
	$(RM) *.core $(LOCAL_LIB)/*.core $(LOCAL_LIB)/t/*.core $(LOCAL_LIB)/t/uthash/*.core
	$(RM) *.o    $(LOCAL_LIB)/*.o    $(LOCAL_LIB)/t/*.o    $(LOCAL_LIB)/t/uthash/*.o
	$(RM) *.out  $(LOCAL_LIB)/*.out  $(LOCAL_LIB)/t/*.out  $(LOCAL_LIB)/t/uthash/*.out
	$(RM) *.orig $(LOCAL_LIB)/*.orig $(LOCAL_LIB)/t/*.orig $(LOCAL_LIB)/t/uthash/*.orig
	$(RM) *.exe  $(LOCAL_LIB)/*.exe  $(LOCAL_LIB)/t/*.exe  $(LOCAL_LIB)/t/uthash/*.exe
	$(RM) *.tmp  $(LOCAL_LIB)/*.tmp  $(LOCAL_LIB)/t/*.tmp  $(LOCAL_LIB)/t/uthash/*.tmp
	$(RM) *.bak  $(LOCAL_LIB)/*.bak  $(LOCAL_LIB)/t/*.bak  $(LOCAL_LIB)/t/uthash/*.bak
	$(RM) *.log  $(LOCAL_LIB)/*.log  $(LOCAL_LIB)/t/*.log  $(LOCAL_LIB)/t/uthash/*.log
	$(RM) *~     $(LOCAL_LIB)/*~     $(LOCAL_LIB)/t/*~     $(LOCAL_LIB)/t/uthash/*~
	$(RM) -rf Debug Release x1 x2 x3

#------------------------------------------------------------------------------
# test
#------------------------------------------------------------------------------
all: t/libtestlib.a

t/libtestlib.a : $(OBJS) $(TEST_SRC_LIB:.c=.o)
	ar rcs t/libtestlib.a $(OBJS:$(PROJ).o=) $(TEST_SRC_LIB:.c=.o)

$(TEST_SRC_MAIN:.c=.o) : t/libtestlib.a

.o.out :
	$(CC) $(CFLAGS) $< -o $(<:.o=$(EXE)) $(LDFLAGS) -Lt -ltestlib
	$(<:.o=$(EXE)) 2> $@~
	diff -w $(<:.o=.bmk) $@~
	mv -f $@~ $@

clean::
	$(RM) t/libtestlib.a t/*.o t/*.out lib/t/*.o lib/t/*.out $(TEST_SRC_MAIN:.c=$(EXE))

test:: $(PROJ)$(EXE) z80asm_lib
	$(MAKE) dev
	$(MAKE) z80asm_lib
	#$(MAKE) -C dev\z80asm_lib test
	$(MAKE) prove_c

testlib:
	$(MAKE) -C lib test

prove_c: t/libtestlib.a $(TEST_SRC_MAIN:.c=.o) $(TEST_SRC_MAIN:.c=.out)

clean::
	$(RM) zcc_opt.def zcc_proj.lst

testzcc : $(PROJ)$(EXE)
	zcc +zx -lndos       -create-app -omicroman.bin ../../examples/microman.c
	zcc +zx -lndos -lmzx -create-app -omandel.bin   ../../examples/graphics/mandel.c
	eightyone microman.tap
	eightyone mandel.tap

clean::
	$(RM) zcc_opt.def microman.bin microman.tap
	$(RM) zcc_opt.def mandel.bin   mandel.tap

clean::
	$(RM) $(PROJ)$(EXE) $(LOCAL_LIB)/$(PROJ)$(EXE) test* $(LOCAL_LIB)/test*
	$(RM) t/*.tmp $(LOCAL_LIB)/t/*.tmp t/dev/*.tmp

#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------
clean::
	rm -f $(DEPENDS)

-include $(DEPENDS)
