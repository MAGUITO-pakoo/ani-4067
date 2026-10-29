#include "NKTime/NkChrono.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace nkentseu;

namespace {
    bool gAUnPrecedent = false;
    int64 gPrecedent = 0;                 //début de l'image précédente
    std::vector<double> gIntervalles;     //intervalles entre deux débuts d'image
}

void BudgetImprimer() {
    const int cadences[3] = {72, 90, 120};
    for (int hz : cadences)
        std::printf("budget %d Hz : %.3f ms\n", hz, 1000.0 / hz);
}

void BudgetDebutImage() {
    const int64 maintenant = NkChrono::Now().nanoseconds;
    if (gAUnPrecedent)
        gIntervalles.push_back(static_cast<double>(maintenant - gPrecedent) * 1e-6);
    gPrecedent = maintenant;
    gAUnPrecedent = true;
}

void BudgetImprimerMesure() {
    const char *mailbox = std::getenv("NK_VK_MAILBOX");
    const char *vsync = std::getenv("MASALLE_VSYNC");
    const bool sansAttente = (mailbox != nullptr && mailbox[0] == '1') ||
                             (vsync != nullptr && vsync[0] == '0');
    const char *etiquette = sansAttente ? "sans attente" : "ecran";
    const size_t ecart = 30;                                    // les 30 premières images paient le démarrage
    if (gIntervalles.size() <= ecart) {
        std::printf("[mesure] trop peu d'images pour %s\n", etiquette);
        return;
    }
    const size_t fin = std::min(gIntervalles.size(), ecart + 1000);
    std::vector<double> v(gIntervalles.begin() + ecart, gIntervalles.begin() + fin);

    double somme = 0.0, maxi = v[0];
    for (double x : v) { somme += x; if (x > maxi) maxi = x; }
    std::sort(v.begin(), v.end());
    const size_t n = v.size();
    const double mediane = (n % 2 == 1) ? v[n / 2] : 0.5 * (v[n / 2 - 1] + v[n / 2]);

    std::printf("%s : moyenne %.3f ms, mediane %.3f ms, max %.3f ms\n",
                etiquette, somme / n, mediane, maxi);
    std::printf("[mesure] %zu intervalles retenus (ne recopiez pas cette ligne)\n", n);
}