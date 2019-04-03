#include "Case.h"

Case::Case(bool used, int value) {
    this->usedValue = used;
    this->value = value;
}

Case::Case() {
    this->usedValue = false;
    this->value = 0;
}
