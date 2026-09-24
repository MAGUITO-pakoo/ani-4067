#include <iostream>
#include <cstdio>

int main() {
#ifdef FILTRE_ACTIF
    printf("FILTRE_ACTIF est defini : le filtre s'est applique.\n");
#else
    printf("FILTRE_ACTIF n'est PAS defini : le filtre ne s'est pas applique.\n");
#endif
    return 0;
}
