# Regex Engine

This project is a simple implementation of a regular expression engine, inspired by [实现一个简单的正则表达式引擎](https://writings.sh/post/regexp).

## Features

- Basic operators, including concatenation, alternation (`|`), and the Kleene star (`*`), etc.
- Dijkstra's two-stack algorithm for parsing regular expressions.
- Thompson's construction for building NFAs.
- Subset construction for generating DFAs.
- Hopcroft's algorithm for minimizing DFAs.

## Requirements

- C++17 or later
- CMake 3.10 or later
- A C++ compiler (e.g., GCC, Clang)

## How to Run

- Compile the project with CMake:

```bash
cd build
cmake ..
make
```

- Run the application:

```bash
./regex_engine
```
