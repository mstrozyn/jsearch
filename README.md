## modules versions search
## Build the "jsearch" command line tool:
Make build directory:
```
mkdir out
```
Generate the makefile:
```
cd out
cmake ../src
```
Compile:
```
make
```
The "jsearch" command searches for the ".json" and ".json-bak" files on the specified path and then searches them for the modules version and reports to the specified server. json files processing is done using https://github.com/nlohmann/json/ header only library for json parsing.
## Run the program:
```
cd out
./jsearch ../test/testcase/1/ 192.168.1.100:61543
```
## Test the program:
```
cd test
./test.sh
TestCase 1 OK
TestCase 2 OK
TestCase 3 OK
```
## Development and test environment:
Developed and tested on ubuntu 22.04, 64 bit, Intel Core i3.
