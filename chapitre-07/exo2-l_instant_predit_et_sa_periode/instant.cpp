// chapitre 7, exercice 2 : l'instant predit et sa periode
#include "NKTime/NkChrono.h"
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;

namespace {
    double gHz = 72.0;               // cadence lue dans MASALLE_HZ
    double gPeriodeNs = 0.0;         // 1e9 / hz, en nanosecondes
    double gT0 = 0.0;                // instant du lancement
    double gPreditPrecedent = 0.0;   // instant predit a l'image d'avant
    int gImage = 0;                  // numero de l'image courante
    double gSomme = 0.0;             // somme des (reel - predit)
    int gMesures = 0;                // nombre de mesures gardees
    bool gImprime = false;

    // float64, en nanosecondes : un float32 ne garde que sept chiffres
    double MaintenantNs() {
        return static_cast<double>(NkChrono::Now().nanoseconds);
    }
}

// a appeler une fois, avant la boucle
void InstantInit() {
    const char *v = std::getenv("MASALLE_HZ");
    const double hz = (v != nullptr) ? std::atof(v) : 72.0;
    gHz = (hz > 1.0) ? hz : 72.0;     // meme garde-fou que le simulateur
    gPeriodeNs = 1e9 / gHz;
    gT0 = MaintenantNs();
}

// a appeler au tout debut de chaque image
void InstantDebutImage() {
    const double maintenant = MaintenantNs();

    // reel - predit : debut reel de cette image moins l'instant predit a l'image d'avant
    // (on ecarte les 30 premieres images, on garde 600 mesures)
    if (gImage >= 30 && gMesures < 600) {
        gSomme += maintenant - gPreditPrecedent;
        gMesures++;
        if (gMesures == 600 && !gImprime) {
            std::printf("hz = %g, periode = %.3f ms, reel - predit = %.3f ms\n",
                        gHz, gPeriodeNs / 1e6, (gSomme / gMesures) / 1e6);
            std::fflush(stdout);
            gImprime = true;
        }
    }

    // instant predit = maintenant + une periode
    const double predit = maintenant + gPeriodeNs;

    // vingt images de suite (images 30 a 49, une fois le demarrage passe)
    if (gImage >= 30 && gImage < 50) {
        std::printf("maintenant = %.3f ms, predit = %.3f ms, ecart = %.3f ms\n",
                    (maintenant - gT0) / 1e6, (predit - gT0) / 1e6,
                    (predit - maintenant) / 1e6);
    }

    gPreditPrecedent = predit;
    gImage++;
}