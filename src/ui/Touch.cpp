#include "ui/Touch.hpp"


string enumToString(TouchEvent e) {
    static vector<string> ret{"BEGAN", "MOVED", "END", "CANCEL"};
    return ret[static_cast<int>(e)];
};
