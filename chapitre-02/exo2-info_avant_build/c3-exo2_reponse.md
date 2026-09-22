# Exercice 2 : commande `jenga info` 

## Sortie de `jenga info`

```
PS C:\Users\pakoo\Desktop\AN-ING4\Coder_pour_la_VR_l'XR_et_l'AR_I\ani-4067\chapitre-02\exo1-le_projet_minimal\MaSalleWks\MaSalle> jenga info 

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

========================= Jenga Workspace: MaSalleWks ==========================

Location: C:\Users\pakoo\Desktop\AN-ING4\Coder_pour_la_VR_l'XR_et_l'AR_I\ani-4067\chapitre-02\exo1-le_projet_minimal\MaSalleWks
Entry file: C:\Users\pakoo\Desktop\AN-ING4\Coder_pour_la_VR_l'XR_et_l'AR_I\ani-4067\chapitre-02\exo1-le_projet_minimal\MaSalleWks\MaSalleWks.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows, Linux, macOS
Target Architectures: x86_64


Projects
------------------------------------------------------------
Name      Kind          Language   Test   External
==================================================
MaSalle   WindowedApp   C++        No     No


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


Daemon
------------------------------------------------------------
Status: Not running

PS C:\Users\pakoo\Desktop\AN-ING4\Coder_pour_la_VR_l'XR_et_l'AR_I\ani-4067\chapitre-02\exo1-le_projet_minimal\MaSalleWks\MaSalle> 
```

### Ce que cette sortie apprend
Le projet est configuerer en mode debug et release. la plate forme actuellement utiliser est Windows. les trois systemes cible sur lesquelles le programmes peut tourner sont : windows, Linux et macOS. l'architecture est celui de x86_64. notre projet se nomme MaSalle, il est de type WindowsApp, le langage est le C++, le projet n'est pas un test ou un ensemble de tests et il n'est pas non plus un projet exterieur(ie qu'l a bien ete concue ds ce workspace). 
Par la suite, nous pouvons voir l'ensemble des Toolchains disponible avec une specification sur la famille de compilateur, le systeme pour le quel cette toolchain fonctionne, l'architecture visee, ainsi que l'environnement d'execution du compilateur.  
pour finir, nous avons "Daemon
------------------------------------------------------------
Status: Not running" qui amene a comprendre qu'il n y a pas de processeur ou encore qu'aucun processeur en arriere plan permettant de garder le workspace charger n'est actif. 