mkdir build
cd build || (echo "failed to create build dir" && exit)
cmake -G Ninja .. -DCMPSDR=ON -DCPTX=ON
cmake --build .

