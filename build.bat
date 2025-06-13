mkdir build
cd build
cmake -G Ninja .. -DCMPSDR=ON -DCPTX=ON
cmake --build .