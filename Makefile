CC = g++

CFLAGS = -Wall -fsanitize=address,undefined -O0 -lm

SRC_DIR_CPU = cpu/src
SRC_CPU = $(wildcard $(SRC_DIR_CPU)/*cpp) 
# OBJ_CPU = $(SRC_CPU:.cpp=.o)

SRC_DIR_UTILITIES = utilities
SRC_UTILITIES = $(wildcard $(SRC_DIR_UTILITIES)/*cpp) 
# OBJ_UTILITIES = $(SRC_UTILITIES:.cpp=.o)

SRC = $(SRC_CPU) $(SRC_UTILITIES)
# OBJ = $(OBJ_CPU) $(OBJ_UTILITIES)
OBJ = $(SRC:.cpp=.o)

EXECUTABLE = CPU

all: $(EXECUTABLE) delete 

$(EXECUTABLE): $(OBJ) 
	$(CC) $(OBJ) $(CFLAGS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -Icpu/include -Iutilities $< $@ 
	
delete:
	@rm -f cpu/src/main.o cpu/src/instructions.o cpu/src/stages.o cpu/src/arr_work.o
	@rm -f utilities/file_work.o

clean: 
	@rm -f $(EXECUTABLE) 

	