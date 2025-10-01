[README.md]
# Squid Game - OOP Project (C++)

## Overview
A console-based C++ game inspired by *Squid Game*.  
Players compete in 3 games: **Red Light Green Light**, **Glass Bridge**, and **Tug of War**.  
Uses **Object-Oriented Programming** (Inheritance, Polymorphism, Encapsulation).

## Features
- Multiple players (1–10)
- Randomized outcomes
- Colored console text (Windows only)
- OOP concepts applied

## How to Compile
### Windows (MinGW / MSVC)
```bash
g++ src/main.cpp -o SquidGame.exe
./SquidGame.exe
```

### Linux/Mac
(Replace `windows.h` with cross-platform alternatives or remove color functions.)
```bash
g++ src/main.cpp -o SquidGame
./SquidGame
```

## Future Improvements
- More games
- Cross-platform support
- Player statistics
