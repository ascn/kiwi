#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <iostream>
#include "util.h"

namespace Kiwi {

class Object {
public:
	Object() : name("") {}
	Object(const String &name) : name(name) {}
	~Object() = default;

	/** The name of the object */
	String name;

	friend std::ostream &operator<<(std::ostream &os, const Object &obj);
};

/**
 * Implementation of operator<< for an Object. Used for outputting
 * the Object to a stream.
 */
std::ostream &operator<<(std::ostream &os, const Object &obj);

}

#endif // __OBJECT_H__