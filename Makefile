MAIN_NAME=main
CC_FLAGS=-Wall -lsfml-window -lsfml-system -lsfml-graphics -std=c++17
# ROOT_FOLDER=
BUILD_FOLDER=build

all: build_proj run

build_proj:
	g++ -g3 $(MAIN_NAME).cpp -o $(BUILD_FOLDER)/$(MAIN_NAME) $(CC_FLAGS)

run:
	./$(BUILD_FOLDER)/$(MAIN_NAME)
