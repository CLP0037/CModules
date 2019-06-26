#MODIFIED    =$(shell git status ./ --short -uno)

#ifeq ($(MODIFIED),)
#VER         =$(shell git log --pretty=format:%h -n 1;git tag | tail -n 1)
#else
#VER         =$(addsuffix -POLLUTED,$(shell git log --pretty=format:%h -n 1;git tag | tail -n 1))
#endif
#DEFINE      =-DVER=\"$(VER)\"

CC = gcc #g++ # C compiler

#CFLAGS = -fPIC -Wall -Wextra -O2 -g -c -std=gnu99 # C flags
#LDFLAGS =  -lrt -luv -lzktybase
INCPATH = -I./
#LDPATH = -L/usr/local/lib/

RM = rm -f  # rm command

SRCS = test.c configfile.c

OBJS = $(SRCS:.c=.o) 

TARGET = testdemo

#.PHONY: all
all: $(OBJS)
	$(CC) $(OBJS) -g -o $(TARGET) 

$(SRCS:.c=.o):%.o:%.c
	$(CC) $(INCPATH) -c $< -o $@ -g

#.PHONY: clean
clean:
	-${RM} ${TARGET} ${OBJS} $(SRCS:.c=.d)

#.PHONY: distclean
distclean:
	-${RM} ${TARGET} ${OBJS} $(SRCS:.c=.d)

