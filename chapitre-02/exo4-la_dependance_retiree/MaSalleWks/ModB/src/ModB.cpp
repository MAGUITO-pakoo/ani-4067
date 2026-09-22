#include "ModB.hpp"
#include "ModA.hpp"
namespace ModB {
    int GetDoubledValue() { return ModA::GetValue() * 2; }
}