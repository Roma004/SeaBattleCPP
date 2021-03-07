@echo off

if "%1" == "/help" (
	echo "usage:\n"
	echo ".\make.bat /build [project's build path] [SFML files path]\n  -- to build the project"
	echo ".\make.bat /help  --  to show that menue"
) else (
	if "%1" == "/build" (
		mkdir %2
		mkdir %2\src
		mkdir %2\configs
		g++ -g3 main.cpp -o %2/SeaBattle.exe -Wall -lsfml-window -lsfml-system -lsfml-graphics -I%3/include -L%3/lib	
		copy %3\lib\sfml-graphics-2.dll %2
		copy %3\lib\sfml-window-2.dll %2
		copy %3\lib\sfml-system-2.dll %2
		copy src %2\src
		copy configs %2\configs
		echo "Done!"
	)
)
