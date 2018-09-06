CC=gcc
FLAGS=-g -Wall -I./include
LIBS=-lGL -lGLEW -lSDL2 -lm
SOURCES=main.c graph.c util.c
OBJS=$(addsuffix .o, $(basename $(notdir $(SOURCES))))
NAME=bifurication.out

%.o:src/%.c
	$(CC) $(FLAGS) -c -o $@ $< $(LIBS)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS)

clean:
	rm -f $(NAME) $(OBJS)