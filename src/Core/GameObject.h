#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <stack>
#include "util.h"
#include "Core/Object.h"
#include "Core/Component.h"
#include "Core/ServiceLocator.h"
#include "Logger.h"

namespace Kiwi {

/**
 * Base class for all entities in the scene.
 */
class GameObject : public Object {
public:
	GameObject(String name);
	GameObject(const GameObject &other);
	GameObject(GameObject &&other);
	virtual ~GameObject();
	GameObject &operator=(GameObject other);

	// Vector of components
	Vector<std::unique_ptr<Component>> components;

	GameObject *parent;

	// Vector of children game objects (this is essentially a scene graph)
	Vector<std::unique_ptr<GameObject>> children;

	/**
	 * Add a new component of the specified type T to this game object and
	 * return a reference to the new component. This function constructs
	 * a new component of type T using the forward arguments args.
	 *
	 * The arguments args will be forwarded to the constructor as
	 * <code>std::forward<Args>(args)...</code>.
	 * 
	 * If the type T is not a component, or is an abstract class,
	 * then the code will fail to compile.
	 *
	 * @tparam T the type of Component to add
	 * @tparam args arguments to a constructor of T
	 * 
	 */
	template <class T, typename... Args>
	T &AddComponent(Args&&... args);

	/**
	 * Returns the component of the specified type T if the game object
	 * has one attached, nullptr if it doesn't.
	 * 
	 * @tparam T the type of Component to retrieve
	 * @return   pointer to the Component if one exists, nullptr otherwise
	 */
	template <class T>
	T *GetComponent() const;

	/**
	 * Returns the component of the specified type T in this game object or
	 * any of its descendants in a depth first order if one is attached,
	 * nullptr otherwise.
	 * 
	 * @tparam T the type of Component to retrieve
	 * @return   pointer to the Component if one exists, nullptr otherwise
	 */
	template <class T>
	T *GetComponentInChildren() const;

	/**
	 * Returns the component of the specified type T in this game object or
	 * any of its ancestors if one is attached, nullptr otherwise.
	 * 
	 * @tparam T the type of Component to retrieve
	 * @return   pointer to the Component if one exists, nullptr otherwise
	 */
	template <class T>
	T *GetComponentInParent() const;

	/**
	 * Finds all components of the specified type T in this game object.
	 * These components are added to the vector components.
	 *
	 * @tparam T the type of Component to retrieve
	 * @param components the vector to fill
	 */
	template <class T>
	void GetComponents(Vector<T *> &components) const;

	/**
	 * Finds all components of the specified type T in this game object and
	 * all of its descendants. These components are added to the vector
	 * components.
	 *
	 * @tparam T the type of Component to retrieve
	 * @param components the vector to fill
	 */
	template <class T>
	void GetComponentsInChildren(Vector<T *> &components) const;

	/**
	 * Finds all components of the specified type T in this game object and
	 * all of its ancestors. These components are added to the vector
	 * components.
	 *
	 * @tparam T the type of Component to retrieve
	 * @param components the vector to fill
	 */
	template <class T>
	void GetComponentsInParent(Vector<T *> &components) const;

	/**
	 * Set the parent of this game object.
	 * @param parent pointer to the new parent GameObject
	 */
	void SetParent(GameObject *parent);

	virtual GameObject *clone() const;

	friend void swap(GameObject &first, GameObject &second) {
		using std::swap;

		swap(first.name, second.name);
		swap(first.children, second.children);
		swap(first.components, second.components);
		swap(first.parent, second.parent);
	}
};

template <class T, typename... Args>
T &GameObject::AddComponent(Args&&... args) {
	static_assert(std::is_base_of_v<Component, T>, "can't add a non component type!");
	auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
	ptr->gameObject = this;
	auto ret = ptr.get();
	GameObject::components.push_back(std::move(ptr));
	return *ret;
}

template <class T>
T *GameObject::GetComponent() const {
	static_assert(std::is_base_of_v<Component, T>, "can't get a non component type!");
	T *ret = nullptr;
	try {
		std::type_index idx = typeid(T);
		for (auto &comp : components) {
			if (comp->getType() == idx) {
				ret = static_cast<T *>(comp.get());
				break;
			}
		}
	} catch (std::exception &e) {
		ServiceLocator::getLogger().Log(e.what());
	}
	return ret;
}

template <class T>
T *GameObject::GetComponentInChildren() const {
	static_assert(std::is_base_of_v<Component, T>, "can't get a non component type!");
	std::stack<GameObject *> stack;
	stack.push(this);
	Component *ret = nullptr;
	while (!stack.empty()) {
		GameObject *currObj = stack.top();
		stack.pop();
		if ((ret = currObj->GetComponent<T>()) != nullptr) {
			break;
		}
		for (auto &child : currObj->children) {
			stack.push(child.get());
		}
	}
	return static_cast<T *>(ret);
}

template <class T>
T *GameObject::GetComponentInParent() const {
	static_assert(std::is_base_of_v<Component, T>, "can't get a non component type!");
	T *ret = nullptr;
	GameObject *currObj = this;
	while (currObj != nullptr && ret == nullptr) {
		ret = currObj->GetComponent<T>();
		currObj = currObj->parent;
	}
	return ret;
}

template <class T>
void GameObject::GetComponents(Vector<T *> &components) const {
	static_assert(std::is_base_of_v<Component, T>, "can't get a non component type!");
	std::type_index idx = typeid(T);
	for (auto &comp : components) {
		if (comp->getType() == idx) {
			components.push_back(comp.get());
		}
	}
}

template <class T>
void GameObject::GetComponentsInChildren(Vector<T *> &components) const {
	static_assert(std::is_base_of_v<Component, T>, "can't get a non component type!");
	std::stack<GameObject *> stack;
	stack.push(this);
	while (!stack.empty()) {
		GameObject *currObj = stack.top();
		stack.pop();
		currObj->GetComponents<T>(components);
		for (auto &child : currObj->children) {
			stack.push(child.get());
		}
	}
}

template <class T>
void GameObject::GetComponentsInParent(Vector<T *> &components) const {
	static_assert(std::is_base_of_v<Component, T>, "can't get a non component type!");
	Gameobject *currObj = this;
	while (currObj != nullptr) {
		currObj->GetComponents<T>(components);
		currObj = currObj->parent;
	}
}

}

#endif // __GAMEOBJECT_H__