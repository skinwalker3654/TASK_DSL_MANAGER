COMPILER = compiler/compiler.c
HEADER = headers/task.c 
TARGET = compile
EXECUTABLE = main
CC = gcc

$(TARGET): $(COMPILER)
	@echo "Compiling the compiler..."
	$(CC) $< -o $@

clean:
	@echo "Deleting executables"
	rm -rf $(TARGET)
	rm -rf $(EXECUTABLE)
