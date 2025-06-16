#include "util.h"
#include <sstream>
#include <iomanip>

std::string formatarNomeArmazem(int id) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << id;
    return oss.str();
}
