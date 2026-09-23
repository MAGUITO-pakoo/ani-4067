#include "Widget.hpp"

#ifdef WIDGET_FULL
Widget::Widget() : m_value(21) {}
int Widget::GetValue() const { return m_value; }
#endif