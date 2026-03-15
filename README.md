# Tic Tac Toe

Jeu de morpion en C avec interface graphique Raylib et une IA Minimax.

Ce depot contient maintenant deux versions :

- Desktop (C + Raylib) pour generer `tictactoe.exe`
- Web (HTML/CSS/JS) deployable sur Vercel

Le depot GitHub est public et le contenu principal est bien publie.

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

## Version Web (Vercel)

La version web est a la racine du projet :

- `index.html`
- `web.css`
- `web.js`
- `vercel.json`

### Lancer localement la version web

Tu peux simplement ouvrir `index.html` dans ton navigateur.

### Deployer sur Vercel

1. Importer le repo GitHub dans Vercel
2. Garder les options par defaut (pas de build special requis)
3. Deploy

La configuration `vercel.json` sert automatiquement `index.html`.

La version desktop `.exe` reste intacte et continue de se compiler comme avant.

## GitHub Actions

Le depot contient un workflow GitHub Actions qui :

- compile le projet automatiquement sur Windows a chaque push sur `main`
- compile aussi sur chaque pull request
- genere une archive de release Windows sur chaque tag de type `v*`

## Release Windows

### Creer une release localement

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\package-release.ps1 -Version v1.0.0
```

Cela produit :

- `dist\TicTacToe-Windows-v1.0.0\`
- `dist\TicTacToe-Windows-v1.0.0.zip`

### Publier une release via GitHub

Apres commit des changements, cree puis pousse un tag :

```powershell
git tag v1.0.0
git push origin main
git push origin v1.0.0
```

Le workflow GitHub va compiler le jeu, creer une archive ZIP Windows, puis publier une release GitHub automatiquement.

### Installer la version release

- telecharger le ZIP depuis la page Releases GitHub
- extraire le contenu dans un dossier
- conserver tictactoe.exe, libraylib.dll et glfw3.dll dans le meme dossier
- lancer `tictactoe.exe`

## Fichiers sources

- Main.c
- Game.c / Game.h
- Render.c / Render.h
- Ai.c / Ai.h

## Distribution

Le package Windows distribue contient :

- `tictactoe.exe`
- `libraylib.dll`
- `glfw3.dll`
- `README.md`
