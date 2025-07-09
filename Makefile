CC= gcc
CFLAGS= -o2 -Wall

TARGET = main

# Source files
SRC = main.c

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
