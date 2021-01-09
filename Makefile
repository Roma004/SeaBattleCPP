MAIN_NAME=main
CC_FLAGS=-Wall -lsfml-window -lsfml-system -lsfml-graphics
# ROOT_FOLDER=
BUILD_FOLDER=build

all: build run

build:
	g++ -g3 $(MAIN_NAME).cpp -o $(BUILD_FOLDER)/$(MAIN_NAME) $(CC_FLAGS)

run:
	./$(BUILD_FOLDER)/$(MAIN_NAME)
