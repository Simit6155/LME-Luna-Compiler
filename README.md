# Luna Compiler



Small compiler written in C++



## Features

* Custom language (Turkish)
* Tokenizer, basic code generation
* Outputs x86\_64 Linux ELF using NASM



## Available Stuff

* getir (number =< 255);
* Language is made with semicolons not whitespaces

 

## Build

Requires

* g++
* nasm
* ld



## Running the Programm

* cd cmake-build-debug-wsl # go to the directory
* ./Luna\_Language\_Machine\_Executable ../test.lme # compiles the test.lme and makes an asm executable
* ./out # runs the executable
* echo $? # see output


## Status
Shortly after coding this I somehow forgot everything and cant do this anymore, this wont be continuing :/
I also realized coding a parse tree is too overcomplicated for me, maybe in the future.



## License

MIT



