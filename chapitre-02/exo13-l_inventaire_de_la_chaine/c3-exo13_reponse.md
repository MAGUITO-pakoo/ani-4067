# Exercice 13 : l'inventaire de la chaîne

## Sortie de `jenga info -v` (section `Available Toolchains`)

Available Toolchains
------------------------------------------------------------
Name                Family   Target OS   Arch     Env  
=======================================================
host-clang          clang    Windows     x86_64   mingw
host-gcc            gcc      Windows     x86_64   mingw
msvc                msvc     Windows     x86_64   msvc
clang-mingw         clang    Windows     x86_64   mingw
mingw               gcc      Windows     x86_64   mingw
clang-cross-linux   clang    Linux       x86_64   gnu


## Ce qui est présent sur la machine

Six toolchains sont presentes, réparties en deux familles :

- **Windows x86_64**, cinq entrées : `msvc` (le compilateur Microsoft natif, `cl.exe`/`lib.exe`), et quatre variantes liées à mon installation MSYS2/MinGW-w64 : `clang-mingw`, `mingw` (GCC), `host-clang` et `host-gcc`. Ces cinq-là ciblent Windows, en environnement `msvc` ou `mingw`.
- **Linux x86_64**, une seule entrée : `clang-cross-linux`, une chaîne de compilation croisée qui permettrait, en théorie, de construire pour Linux depuis Windows.


celle choisie par défaut par Jenga est `clang-mingw`.

## Ce qu'on ne voit pas'

Aucune toolchain Android n'est presente, alors que mes `targetoses` peuvent viser Android. Il n'y a pas non plus de toolchain pour iOS, macOS/Xcode.

