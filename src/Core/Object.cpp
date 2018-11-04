#include "Core/Object.h"

namespace Kiwi {

std::ostream &operator<<(std::ostream &os, const Object &obj) {
	return os << obj.name;
}

}