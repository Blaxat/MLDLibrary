# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRC = app.c mld.c

# Header files
HEADERS = mld.h css.h

# Output executable
TARGET = mld_app

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up the build files
clean:
	rm -f $(TARGET)
	rm -f *.o

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
