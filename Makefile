BUILD = build
BIN = bin
RES = resources
TARGET = $(BIN)/rogue
CC = g++
INCLUDES = -I/usr/include/libtcod-1.5.1
LIBS = -ltcod -ltcodxx -lSDL
CFLAGS = $(INCLUDES) -g -Wall
LFLAGS = $(INCLUDES) $(LIBS) -g -Wall
OBJS = \
	$(BUILD)/main.o \
    $(BUILD)/Actor.o \
    $(BUILD)/Engine.o \
    $(BUILD)/Map.o

$(TARGET) : $(OBJS)
	@mkdir -p $(BIN)
	@cp -r $(RES) $(BIN)
	@echo " Linking..."
	$(CC) $^ $(LFLAGS) -o $(TARGET)

$(BUILD)/%.o : src/%.cpp
	@mkdir -p $(BUILD)
	@echo " Compiling..."
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	@echo " Cleaning..."
	$(RM) -r $(BUILD) $(BIN)
