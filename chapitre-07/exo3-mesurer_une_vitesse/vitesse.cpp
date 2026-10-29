// chapitre 7, exercice 3 : mesurer une vitesse
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

    float32 gAccX = 0.f, gAccY = 0.f;       // mouvements bruts accumules
    float32 gLacet = 0.f, gTangage = 0.f;   // en radians
    double gTourDegS = 0.0;                 // MASALLE_TOUR, en degres par seconde
    double gT0 = 0.0;                       // instant du lancement, en ns
    bool gAPrecedente = false;
    NkQuatf gPrecedente = NkQuatf(0.f, 0.f, 0.f, 1.f);
    double gTPrecedent = 0.0;               // instant de la pose precedente, en secondes
    int gImage = 0;
    int gImprimees = 0;

    double MaintenantNs() {
        return static_cast<double>(NkChrono::Now().nanoseconds);
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

// a appeler une fois, avant la boucle
void VitesseInit() {
    const char *v = std::getenv("MASALLE_TOUR");
    gTourDegS = (v != nullptr) ? std::atof(v) : 0.0;
    gT0 = MaintenantNs();
}

// a appeler pour chaque mouvement brut de souris
void VitesseSouris(float32 dx, float32 dy) {
    gAccX += dx;
    gAccY += dy;
}

// a appeler une fois par image : rend l'orientation de la tete
NkQuatf VitesseOrientationImage() {
    const double t = (MaintenantNs() - gT0) * 1e-9;   // secondes

    if (gTourDegS > 0.0) {
        // tour programme : le lacet suit l'horloge
        gLacet = static_cast<float32>(gTourDegS * (kPi / 180.0) * t);
    } else {
        // souris : on retire le total accumule, puis on le vide
        gLacet -= gAccX * kSensibilite;
        gTangage -= gAccY * kSensibilite;
    }
    gAccX = 0.f;
    gAccY = 0.f;

    const NkMat4f rotation = NkMat4f::RotationY(NkAngle::FromRad(gLacet)) *
                             NkMat4f::RotationX(NkAngle::FromRad(gTangage));
    const NkQuatf orientation = NkQuatf(rotation).Normalized();

    if (gAPrecedente) {
        const double dt = t - gTPrecedent;
        const NkVec3f v = VitesseAngulaire(gPrecedente, orientation, static_cast<float32>(dt));
        const float32 radVersDeg = 180.f / kPi;

        // programme : vingt lignes a partir de l'image 30 (demarrage ecarte)
        // souris : vingt lignes a partir du premier mouvement apres l'image 30
        bool enCours = (gImprimees < 20) && (gImage >= 30);
        if (gTourDegS <= 0.0 && gImprimees == 0 && v.Len() * radVersDeg < 0.001f) {
            enCours = false;
        }
        if (enCours) {
            if (gImprimees == 0) {
                std::printf("serie %s\n", (gTourDegS > 0.0) ? "programme" : "souris");
            }
            std::printf("image %d : dt = %.3f ms, vitesse = (%.3f, %.3f, %.3f) deg/s\n",
                        gImage, dt * 1000.0,
                        v.x * radVersDeg, v.y * radVersDeg, v.z * radVersDeg);
            if (gImprimees == 19) {
                std::fflush(stdout);
            }
            gImprimees++;
        }
    }

    gPrecedente = orientation;
    gTPrecedent = t;
    gAPrecedente = true;
    gImage++;
    return orientation;
}