// chapitre 7, exercice 5 : predire et comparer
#include "NKTime/NkChrono.h"
#include "NKMath/NkQuat.h"
#include "NKMath/NkMat.h"
#include "NKMath/NkVec.h"
#include "NKMath/NkAngle.h"
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::math;

namespace {
    const float32 kPi = 3.14159265f;
    const float32 kSensibilite = 0.0022f;   // radian par unite de souris
    const int kEcart = 30;                  // les 30 premieres images paient le demarrage
    const int kMesures = 1000;              // nombre d'images comparees

    float32 gAccX = 0.f, gAccY = 0.f;       // mouvements bruts accumules
    float32 gLacet = 0.f, gTangage = 0.f;   // en radians
    double gHz = 72.0;                      // MASALLE_HZ
    double gPeriodeS = 1.0 / 72.0;          // une periode de l'ecran, en secondes
    double gT0 = 0.0;                       // instant du lancement, en ns

    bool gAPrecedente = false;
    NkQuatf gMesureePrec = NkQuatf(0.f, 0.f, 0.f, 1.f);
    double gTPrec = 0.0;                    // en secondes
    bool gAPrediction = false;
    NkQuatf gPredPrec = NkQuatf(0.f, 0.f, 0.f, 1.f);   // prediction gardee une image

    int gImage = 0;
    int gMesurees = 0;
    int gImmobiles = 0;
    double gSommeAvec = 0.0, gMaxAvec = 0.0;
    double gSommeSans = 0.0, gMaxSans = 0.0;
    bool gImprime = false;

    double MaintenantNs() {
        return static_cast<double>(NkChrono::Now().nanoseconds);
    }

    // angle entre deux orientations, en degres : (b * a.Conjugate()).Normalized().Angle().Deg()
    // (avec le test du chemin court, vu a l'exercice 4)
    double AngleDeg(const NkQuatf &a, const NkQuatf &b) {
        NkQuatf d = (b * a.Conjugate()).Normalized();
        if (d.w < 0.f) {
            d = NkQuatf(-d.x, -d.y, -d.z, -d.w);
        }
        return static_cast<double>(d.Angle().Deg());
    }
}

// Vitesse angulaire entre deux poses horodatees (calcul donne par le sujet)
static NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to, float32 dtSeconds) {
    if (dtSeconds <= 0.f) {
        return NkVec3f(0.f, 0.f, 0.f);
    }
    NkQuatf delta = (to * from.Conjugate()).Normalized();
    // Les deux hemispheres du quaternion codent la meme rotation : on force le chemin court
    if (delta.w < 0.f) {
        delta = NkQuatf(-delta.x, -delta.y, -delta.z, -delta.w);
    }
    const float32 angleRad = delta.Angle().Rad();
    if (angleRad <= 1e-6f) {
        return NkVec3f(0.f, 0.f, 0.f);
    }
    return delta.Axis() * (angleRad / dtSeconds);
}

// Extrapole l'orientation a vitesse constante pendant dtSeconds
static NkQuatf Extrapoler(const NkQuatf &orientation, const NkVec3f &vitesse, float32 dtSeconds) {
    const float32 speedRad = vitesse.Len();
    if (speedRad > 1e-6f) {
        const NkVec3f axis = vitesse * (1.f / speedRad);
        // Delta en espace de BASE : il se compose a GAUCHE de l'orientation courante
        const NkQuatf delta(NkAngle::FromRad(speedRad * dtSeconds), axis);
        return (delta * orientation).Normalized();
    }
    return orientation;
}

// a appeler une fois, avant la boucle
void PredictionInit() {
    const char *v = std::getenv("MASALLE_HZ");
    const double hz = (v != nullptr) ? std::atof(v) : 72.0;
    gHz = (hz > 1.0) ? hz : 72.0;
    gPeriodeS = 1.0 / gHz;                // la periode de l'ecran
    gT0 = MaintenantNs();
}

// a appeler pour chaque mouvement brut de souris
void PredictionSouris(float32 dx, float32 dy) {
    gAccX += dx;
    gAccY += dy;
}

// a appeler une fois par image : rend l'orientation mesuree de la tete
NkQuatf PredictionOrientationImage() {
    const double t = (MaintenantNs() - gT0) * 1e-9;   // secondes

    // la souris est lue une fois par image, l'accumulateur est vide
    const bool immobile = (gAccX == 0.f && gAccY == 0.f);
    gLacet -= gAccX * kSensibilite;
    gTangage -= gAccY * kSensibilite;
    gAccX = 0.f;
    gAccY = 0.f;

    const NkMat4f rotation = NkMat4f::RotationY(NkAngle::FromRad(gLacet)) *
                             NkMat4f::RotationX(NkAngle::FromRad(gTangage));
    const NkQuatf mesuree = NkQuatf(rotation).Normalized();

    if (gAPrecedente) {
        // 1) on compare la prediction de l'image d'avant a l'orientation mesuree maintenant
        if (gAPrediction && gImage >= kEcart && gMesurees < kMesures) {
            const double avec = AngleDeg(gPredPrec, mesuree);       // avec prediction
            const double sans = AngleDeg(gMesureePrec, mesuree);    // sans prediction
            gSommeAvec += avec;
            gSommeSans += sans;
            if (avec > gMaxAvec) gMaxAvec = avec;
            if (sans > gMaxSans) gMaxSans = sans;
            if (immobile) gImmobiles++;
            gMesurees++;

            if (gMesurees == kMesures && !gImprime) {
                std::printf("avec prediction : moyenne %.4f deg, max %.4f deg, sur %d images\n",
                            gSommeAvec / gMesurees, gMaxAvec, gMesurees);
                std::printf("sans prediction : moyenne %.4f deg, max %.4f deg, sur %d images\n",
                            gSommeSans / gMesurees, gMaxSans, gMesurees);
                std::printf("[info] %d images sur %d sans mouvement de souris (periode %.3f ms)\n",
                            gImmobiles, gMesurees, gPeriodeS * 1000.0);
                std::fflush(stdout);
                gImprime = true;
            }
        }

        // 2) on predit pour l'instant ou cette image sera vue : une periode plus tard
        const double dt = t - gTPrec;
        const NkVec3f v = VitesseAngulaire(gMesureePrec, mesuree, static_cast<float32>(dt));
        gPredPrec = Extrapoler(mesuree, v, static_cast<float32>(gPeriodeS));
        gAPrediction = true;
    }

    gMesureePrec = mesuree;
    gTPrec = t;
    gAPrecedente = true;
    gImage++;
    return mesuree;
}