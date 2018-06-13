TARGET = app.o
CC = gcc
RM = rm -rf
SRCDIR = ./src
INCDIR = ./inc
SRC = $(wildcard ./$(SRCDIR)/*.c)
CFLAGS 	= `pkg-config --cflags --libs gtk+-3.0`
CFLAGS += -I$(SRCDIR)
CFLAGS += -I$(INCDIR)
CFLAGS += -g
#RESOURCE = task2_pwm_res

OBJS = $(SRC:.c=.o)

all: $(OBJS) $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

#$(RESOURCE).c: $(RESOURCE).gresource.xml
#	glib-compile-resources --sourcedir=. $(RESOURCE).gresource.xml --generate-source

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)