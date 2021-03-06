TARGET   = main
CC        = gcc
CFLAGS    = -Wall
OBJFILES = main.o callbacks.o functions.o image.o
LDFLAGS   = -lGL -lGLU -lglut -lm

%.o: %c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

.PHONY: clean dist

clean:
	rm -f $(OBJFILES) $(TARGET) *core
	rm -f highscores.txt

dist: clean
	-tar -chvj -C .. -f ../$(TARGET).tar.bz2 $(TARGET)