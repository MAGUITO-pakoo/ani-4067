// chapitre 7, exercice 4 : le chemin long, provoque
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

    float32 gLacet = 0.f;                   // en radians, sans reboucler
    float32 gTangage = 0.f;                 // reste a 0 : pas de souris ici
    double gTourDegS = 0.0;                 // MASALLE_TOUR, en degres par seconde
    double gT0 = 0.0;                       // instant du lancement, en ns
    bool gAPrecedente = false;
    NkQuatf gPrecedente = NkQuatf(0.f, 0.f, 0.f, 1.f);
    double gTPrecedent = 0.0;               // en secondes
    int gImage = 0;

    double MaintenantNs() {
        return static_cast<double>(NkChrono::Now().nanoseconds);
    }
}

// Vitesse angulaire entre deux poses horodatees.
// Le test qui force le chemin court (delta.w negatif) est RETIRE, expres.
// deltaW ne sert qu'a l'impression.
static NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to,
                                float32 dtSeconds, float32 &deltaW) {
    deltaW = 1.f;
    if (dtSeconds <= 0.f) {
        return NkVec3f(0.f, 0.f, 0.f);
    }
    NkQuatf delta = (to * from.Conjugate()).Normalized();
    
    deltaW = delta.w;
    const float32 angleRad = delta.Angle().Rad();
    if (angleRad <= 1e-6f) {
        return NkVec3f(0.f, 0.f, 0.f);
    }
    return delta.Axis() * (angleRad / dtSeconds);
}

// a appeler une fois, avant la boucle
void CheminInit() {
    const char *v = std::getenv("MASALLE_TOUR");
    gTourDegS = (v != nullptr) ? std::atof(v) : 0.0;
    gT0 = MaintenantNs();
}

// a appeler une fois par image : rend l'orientation de la tete
NkQuatf CheminOrientationImage() {
    const double t = (MaintenantNs() - gT0) * 1e-9;   // secondes

    // tour programme : le lacet suit l'horloge (pas de souris)
    gLacet = static_cast<float32>(gTourDegS * (kPi / 180.0) * t);

    const NkMat4f rotation = NkMat4f::RotationY(NkAngle::FromRad(gLacet)) *
                             NkMat4f::RotationX(NkAngle::FromRad(gTangage));
    const NkQuatf orientation = NkQuatf(rotation).Normalized();

    if (gAPrecedente && gTourDegS > 0.0) {
        const double dt = t - gTPrecedent;
        float32 w = 1.f;
        const NkVec3f v = VitesseAngulaire(gPrecedente, orientation,
                                           static_cast<float32>(dt), w);
        const float32 radVersDeg = 180.f / kPi;
        // chaque image est imprimee, pas une sur dix
        std::printf("image %d : lacet = %.3f deg, vitesse = (%.3f, %.3f, %.3f) deg/s, delta.w = %.6f\n",
                    gImage, gLacet * radVersDeg,
                    v.x * radVersDeg, v.y * radVersDeg, v.z * radVersDeg, w);
    }

    gPrecedente = orientation;
    gTPrecedent = t;
    gAPrecedente = true;
    gImage++;
    return orientation;
}