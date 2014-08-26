LIBTCOD = src/libtcod-1.5.1
SDL = src/SDL-1.2.14

BUILD = build
BIN = bin
TARGET = $(BIN)/rogue
CC = g++
INCLUDES = -I$(LIBTCOD)/include -I$(SDL)/include
CFLAGS = $(INCLUDES) -g -Wall
LFLAGS = $(INCLUDES) -g -Wall
OBJS = \
	$(BUILD)/main.o

$(TARGET) : $(OBJS)
	@mkdir -p $(BIN)
	@echo " Linking..."
	$(CC) $(LFLAGS) $^ -o $(TARGET)

$(BUILD)/%.o : src/%.cpp
	@mkdir -p $(BUILD)
	@echo " Compiling..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo " Cleaning..."
	$(RM) -r $(BUILD) $(BIN)
