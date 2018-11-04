#include "Core/TypeMap.h"

namespace Kiwi {

TypeMap::TypeMap() : typeToString(), stringToType() {}

const String &TypeMap::get(const std::type_index &key) {
	return typeToString.at(key);
}

void TypeMap::put(const std::type_index &key, const String &value) {
	typeToString.emplace(key, value);
}

const std::type_index &TypeMap::get(const String &key) {
	return stringToType.at(key);
}

void TypeMap::put(const String &key, const std::type_index &value) {
	stringToType.emplace(key, value);
}

}