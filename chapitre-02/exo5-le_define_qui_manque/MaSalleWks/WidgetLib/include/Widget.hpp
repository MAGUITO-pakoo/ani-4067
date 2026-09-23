#pragma once

// Widget n'expose une vraie API que si WIDGET_FULL est defini
// au moment ou ce fichier est inclus. Sans ce define, la classe
// est une coquille vide : aucun membre, aucune methode.
class Widget {
public:
#ifdef WIDGET_FULL
    Widget();
    int GetValue() const;
private:
    int m_value = 0;
#endif
};