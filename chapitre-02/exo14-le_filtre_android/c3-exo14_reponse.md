# Exercice 14 : le filtre Android

## Fichier de projet

from Jenga import *

with workspace("MaSalleWks"):
    configurations(['Debug', 'Release'])
    targetoses([TargetOS.WINDOWS, TargetOS.LINUX, TargetOS.MACOS, TargetOS.ANDROID])
    targetarchs([TargetArch.X86_64, TargetArch.ARM64])

    with project("MaSalle"):
        consoleapp()
        language("C++")
        location("MaSalle")
        files(["src/**.cpp", "include/**.hpp"])

        with filter("system:Android"):
            defines([
                "NKENTSEU_PLATFORM_ANDROID=1",
            ])
            links([
                "log",
                "android",
                "EGL",
                "GLESv3",
            ])


## Premier constat : `jenga info` ne montre rien sur le filtre

Sortie de `jenga info -v`, avec le filtre android dans le fichier :

Projects
------------------------------------------------------------
Name      Kind         Language   Test   External
=================================================
MaSalle   ConsoleApp   C++        No     No


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


les deux tableaux sont  identiques, que le bloc `with filter("system:Android"):` soit présent dans le fichier ou totalement absent. `jenga info` liste les projets et les toolchains détectées sur la machine. 

## comment vérifier que le filtre s'active pour Android

la commande `jenga build --platform Android-arm64 --verbose`, une fois un NDK installé et détecté par `jenga info -v`, et l;on cherchera dans la ligne de commande affichée pour compiler `main.cpp` la présence de `-DNKENTSEU_PLATFORM_ANDROID=1`, ainsi que `-llog -landroid -lEGL -lGLESv3` à l'édition de liens — leur présence prouverait que le filtre `system:Android` s'est bien évalué comme vrai pour cette plateforme.

