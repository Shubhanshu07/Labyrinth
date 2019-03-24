labyrinth
=========

Compilation instructions
------------------------
```shell
mkdir build
cd build
cmake ..
make
```

Example
-------

Executing the following sequence of commands will use the sample.txt file present in the parent diretory as an input and write the output to another file in the parent directory (sample.out)
```shell
# Switch to build directory
cd build
./labyrinth -i ../sample.txt -o ../sample.out
```
