param(
    [string]$Version = "dev"
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$mingwBin = "C:\msys64\mingw64\bin"
$distRoot = Join-Path $projectRoot "dist"
$bundleRoot = Join-Path $distRoot ("TicTacToe-Windows-" + $Version)
$zipPath = Join-Path $distRoot ("TicTacToe-Windows-" + $Version + ".zip")

$env:PATH = $mingwBin + ";" + $env:PATH

if (Test-Path $bundleRoot) {
    Remove-Item $bundleRoot -Recurse -Force
}

if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}

New-Item -ItemType Directory -Force $bundleRoot | Out-Null

gcc -Wall -Wextra -std=c11 -O2 `
    "$projectRoot\Main.c" `
    "$projectRoot\Game.c" `
    "$projectRoot\Render.c" `
    "$projectRoot\Ai.c" `
    -o "$bundleRoot\tictactoe.exe" `
    -I"C:\msys64\mingw64\include" `
    -L"C:\msys64\mingw64\lib" `
    -lraylib -lopengl32 -lgdi32 -lwinmm

Copy-Item "$mingwBin\libraylib.dll" $bundleRoot
Copy-Item "$projectRoot\README.md" $bundleRoot

Compress-Archive -Path "$bundleRoot\*" -DestinationPath $zipPath -Force

Write-Output "Release package created: $zipPath"