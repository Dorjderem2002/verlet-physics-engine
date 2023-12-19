echo 'Compiling...'
g++ *.cpp physics/*.cpp -o main -Ofast -std=c++14 -lsfml-graphics -lsfml-system -lsfml-window -I/opt/homebrew/Cellar/sfml/2.6.1/include -L/opt/homebrew/Cellar/sfml/2.6.1/lib
echo 'Done.'
./main
