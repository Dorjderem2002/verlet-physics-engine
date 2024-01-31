Physics simulation Cirlce2Circle

10000 objects at 60FPS

More optimizations coming soon

Collision handler algorithms

- Naive O(n^2)
- Sorting O(n * log(n))
- Grid O(n * m)

Screenshot
![alt text](images/ss.png)

HOW TO RUN:

MacOS:

brew install sfml g++

mkdir build && cd build

cmake ../

make

Linux:

sudo apt install g++ libsfml-dev

mkdir build && cd build

cmake ../

make

Windows:

Haven't tried but it should work with cmake if u configure sfml correctly