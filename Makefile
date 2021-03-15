MAIN_NAME=main
ProjectName=SeaBattle
CC_FLAGS=-Wall -lsfml-window -lsfml-system -lsfml-graphics -lsfml-network -std=c++17 -pthread # -O2
# ROOT_FOLDER=
BUILD_FOLDER=build

all: build_proj run

build_proj:
	g++ -g3 $(MAIN_NAME).cpp -o $(BUILD_FOLDER)/$(ProjectName) $(CC_FLAGS)

run:
	./$(BUILD_FOLDER)/$(ProjectName)
