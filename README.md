# Tic Tac Toe

Jeu de morpion en C avec interface graphique Raylib et une IA Minimax.

## Prerequis

- GCC
- Raylib
- Windows avec MinGW/MSYS2 configure

## Compilation

Exemple de compilation sous Windows avec MSYS2 MinGW64 :

```powershell
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
gcc -Wall -Wextra -std=c11 -O2 .\Main.c .\Game.c .\Render.c .\Ai.c -o .\tictactoe.exe -IC:\msys64\mingw64\include -LC:\msys64\mingw64\lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Execution

```powershell
.\tictactoe.exe
```

## Fichiers sources

- Main.c
- Game.c / Game.h
- Render.c / Render.h
- Ai.c / Ai.h