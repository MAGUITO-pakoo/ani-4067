// chapitre 7, exercice 6 : ce que la prediction vous apporte
#include "NKMath/NkQuat.h"
#include "NKMath/NkMat.h"
#include "NKMath/NkVec.h"
#include "NKMath/NkAngle.h"
#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::math;

namespace {
    const double kPi = 3.14159265358979323846;
    const double kHz = 72.0;   // cadence d'un casque : une pose par periode

    // Un geste : amplitude (deg) en duree (s), depart et arrivee doux.
    double Lacet(double t, double amplitude, double duree) {   // t en secondes, lacet en degres
        if (t <= 0.0) return 0.0;
        if (t >= duree) return amplitude;
        return amplitude * 0.5 * (1.0 - std::cos(kPi * t / duree));
    }

    // La vraie pose de l'instant t, par les quaternions
    NkQuatf Pose(double t, double amplitude, double duree) {
        return NkQuatf::RotateY(NkAngle(static_cast<float32>(Lacet(t, amplitude, duree))));
    }

    // angle entre deux orientations, en degres : (b * a.Conjugate()).Normalized().Angle().Deg()
    double AngleDeg(const NkQuatf &a, const NkQuatf &b) {
        NkQuatf d = (b * a.Conjugate()).Normalized();
        if (d.w < 0.f) {
            d = NkQuatf(-d.x, -d.y, -d.z, -d.w);
        }
        return static_cast<double>(d.Angle().Deg());
    }

    // Vitesse angulaire entre deux poses horodatees (calcul donne par le sujet)
    NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to, float32 dtSeconds) {
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
    NkQuatf Extrapoler(const NkQuatf &orientation, const NkVec3f &vitesse, float32 dtSeconds) {
        const float32 speedRad = vitesse.Len();
        if (speedRad > 1e-6f) {
            const NkVec3f axis = vitesse * (1.f / speedRad);
            // Delta en espace de BASE : il se compose a GAUCHE de l'orientation courante
            const NkQuatf delta(NkAngle::FromRad(speedRad * dtSeconds), axis);
            return (delta * orientation).Normalized();
        }
        return orientation;
    }

    // Une pose par periode, pendant la duree du geste seulement
    void Mesurer(const char *nom, double amplitude, double duree) {
        const double T = 1.0 / kHz;
        const int n = static_cast<int>(duree / T);

        double sommeAvec = 0.0, sommeSans = 0.0;
        int compte = 0;
        NkQuatf precedente = Pose(-T, amplitude, duree);

        for (int k = 0; k <= n; ++k) {
            const double t = k * T;
            const NkQuatf pose = Pose(t, amplitude, duree);

            // la vitesse vient de la pose precedente
            const NkVec3f v = VitesseAngulaire(precedente, pose, static_cast<float32>(T));

            // chaque pose sera vue une periode plus tard
            const NkQuatf avec = Extrapoler(pose, v, static_cast<float32>(T));   // avec prediction
            const NkQuatf vraie = Pose(t + T, amplitude, duree);                 // vraie pose de l'affichage

            sommeAvec += AngleDeg(vraie, avec);
            sommeSans += AngleDeg(vraie, pose);   // sans prediction : la pose telle quelle
            ++compte;
            precedente = pose;
        }

        const double moyAvec = sommeAvec / compte;
        const double moySans = sommeSans / compte;
        const double rapport = (moyAvec > 0.0) ? (moySans / moyAvec) : 0.0;
        std::printf("%s (%.0f deg en %.0f ms) : avec %.4f deg, sans %.4f deg, rapport %.2f\n",
                    nom, amplitude, duree * 1000.0, moyAvec, moySans, rapport);
    }
}

// a appeler au demarrage, avant d'ouvrir la fenetre
void GestesImprimer() {
    Mesurer("rapide", 90.0, 0.400);
    Mesurer("lent", 45.0, 2.000);
    Mesurer("demi-tour", 180.0, 0.200);
    std::fflush(stdout);
}