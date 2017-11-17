
CC=g++
CFLAGS=-W -Wall -Werror -Wextra -std=c++1z -g

# define any directories containing header files other than /usr/include
#
INCLUDES = #-I/home/newhall/include  -I../include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = #-L/home/newhall/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = #-lmylib -lm

# define the C source files
SRCS = main.cpp parser.cpp lexer.cpp runner.cpp

OBJS = $(SRCS:.cpp=.o)

# define the executable file
MAIN = lispc


all:    $(MAIN)
	@echo  Simple compiler named $(MAIN) has been compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
