### Physics simulation Cirlce2Circle

15000 objects at 60FPS
M1 PRO

More optimizations coming soon

Collision handler algorithms

- Naive O(n^2)
- Sorting O(n * log(n))
- Grid O(n * m)

New Feature:
- Collision resolving using Machine Learning

Screenshot
Circle-Circle collision without multithreading
![alt text](images/ss.png)
Chain demonstration
![alt text](images/chain.png)

HOW TO RUN:

MacOS:
```
brew install sfml g++
```
```
mkdir build && cd build
```
```
cmake ../ && make
```
```
cp -R ../resource/ ./resource
```
```
./main
```
Linux:
```
sudo apt install g++ libsfml-dev
```
```
mkdir build && cd build
```
```
cmake ../ && make
```
```
cp -R ../resource/ ./resource       
```
```
./main
```
Windows:

Haven't tried but it should work with cmake if u configure sfml correctly