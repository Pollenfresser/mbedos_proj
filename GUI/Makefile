TARGET   = application.out
CC       = gcc
RM       = rm -rf
SRCDIR   = ./
SRC      = $(wildcard $(SRCDIR)/*.c)
SRC			+= gui_res.c
INCDIR   = ./header
CFLAGS   = `pkg-config --cflags --libs gtk+-3.0`
CFLAGS  += -I$(SRCDIR)
CFLAGS  += -I$(INCDIR)
CFLAGS 	+= -std=c99
CFLAGS  += -Wall -g
OBJS     = $(SRC:.c=.o)
RESOURCE = gui_res

all: $(OBJS) $(TARGET)

%.o: %.c $(RESOURCE).c
	$(CC) -c $(CFLAGS) $< -o $@
	@echo ""

$(RESOURCE).c: $(RESOURCE).gresource.xml
	glib-compile-resources --sourcedir=. $(RESOURCE).gresource.xml --generate-source

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET) $(RESOURCE).c core *~
