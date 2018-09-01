#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "util.h"
#include "TypeMap.h"
#include "Object.h"
#include "Logger.h"

namespace Kiwi {

class GameObject;
class Component;
using ComponentPrototypes = std::unordered_map<std::type_index, Component *>;

/**
 * Base class for everything attached to @link GameObject GameObjects@endlink.
 * All subclasses must include the macro <code>K_COMPONENT_H</code> as following.
 *
 * In <code>DerivedComponent.h</code>
 * <pre>
 * class DerivedComponent : public Component {
 *     K_COMPONENT_H(DerivedComponent)
 *     ...
 * }
 * </pre>
 * 
 * In addition, in the source (*.cpp) file for the Component subclass,
 * the macro <code>K_COMPONENT_S</code> must be included as following.
 *
 * In <code>DerivedComponent.cpp</code>
 * <pre>
 * %K_COMPONENT_S(DerivedComponent, "DerivedComponent")
 * </pre>
 * A default constructor cannot be provided for any subclasses. Provide a
 * constructor taking in a single String argument for the name of the Object.
 */
class Component : public Object {
public:
	Component() : gameObject(nullptr) {}
	virtual ~Component() = 0;

	/**
	 * Returns a clone of this component. All subclasses
	 * of Component must implement this. This is used
	 * in conjunction with the prototype map and registry
	 * to dynamically create instances of Component subclasses.
	 * @return
	 */
	virtual Component *clone() = 0;

	/**
	 * Returns the type_index of this component. There is no
	 * need to explicitly implement this in subclasses as it
	 * is already implemented by using the K_COMPONENT_H macro.
	 * @return
	 */
	virtual std::type_index getType() = 0;
	static TypeMap &getRegistry() {
		static TypeMap registry;
		return registry;
	}
	static ComponentPrototypes &getPrototypes() { static ComponentPrototypes prototypes; return prototypes; }

	GameObject *gameObject;
};

template <typename T>
struct ComponentRegistrar {
	static std::type_index registerComponent(const String &name) {
		Component::getRegistry().put(typeid(T), name);
		Component::getRegistry().put(name, typeid(T));
		Component::getPrototypes().emplace(typeid(T), new T());
		return typeid(T);
	}
};

}

#define K_COMPONENT_H(Type) \
private: \
	static std::type_index type; \
public: \
	std::type_index getType() override { return type; }

#define K_COMPONENT_S(Type, String) \
std::type_index Type::type = ComponentRegistrar<Type>::registerComponent(String);

#endif // __COMPONENT_H__