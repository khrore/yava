mkdir ../build
cd ../build
cmake -G Ninja .. 
cmake --build .

cd ../scripts
compile_shaders.bat