CC = gcc

CFLAGS = -Wall -fsanitize=address,undefined -O0 -lm

SRC_DIR_CPU = cpu/src
SRC_CPU = $(wildcard $(SRC_DIR_CPU)/*c ) 
OBJ_CPU = $(SRC_CPU:.c=.o)

SRC_DIR_UTILITIES = utilities
SRC_UTILITIES = $(wildcard $(SRC_DIR_UTILITIES)/*c) 
OBJ_UTILITIES = $(SRC_UTILITIES:.c=.o)

SRC = $(SRC_CPU) $(SRC_UTILITIES)
OBJ = $(OBJ_CPU) $(OBJ_UTILITIES)

EXECUTABLE = CPU

all: $(EXECUTABLE) 

$(EXECUTABLE): $(OBJ) 
	$(CC) $(OBJ) $(CFLAGS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -Icpu/include -Iutilities $< -o $@ 

clean: 
	@rm -f $(EXECUTABLE) 
	@rm -f cpu/src/main.o cpu/src/instructions.o
	@rm -f utilities/file_work.o
	