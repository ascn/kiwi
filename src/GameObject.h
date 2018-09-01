#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <stack>
#include "util.h"
#include "Object.h"
#include "Component.h"

namespace Kiwi {

/**
 * Base class for all entities.
 */
class GameObject : public Object {
public:
	GameObject(String name);

	// Vector of components
	Vector<Component *> components;

	GameObject *parent;

	// Vector of children game objects (this is essentially a scene graph)
	Vector<GameObject *> children;

	/** @name AddComponent
	 * Add a new component of the specified type to th is game object and
	 * return a pointer to the new component. It is recommended to use the
	 * templatized function. Otherwise, the returned pointer should be
	 * casted to the desired type.
	 */
	/**@{*/
	/**
	 * @tparam T the type of Component to add
	 * @return   pointer to the newly created Component
	 */
	template <class T>
	T *AddComponent();
	/**
	 * @param type the name of the type of Component to add
	 * @return     pointer to the newly created Component
	 */
	Component *AddComponent(const String &type);
	/**@}*/

	/** @name GetComponent
	 * Returns the component of the specified type if the game object
	 * has one attached, nullptr if it doesn't. It is recommended to use
	 * the templatized function. Otherwise, the returned pointer should
	 * be casted to the desired type.
	 */
	/**@{*/
	/**
	 * @tparam T the type of Component to retrieve
	 * @return   pointer to the Component if one exists, nullptr otherwise
	 */
	template <class T>
	T *GetComponent();
	/**
	 * @param type the name of the type of Component to retrieve
	 * @return     pointer to the Component if one exists, nullptr otherwise
	 */
	Component *GetComponent(const String &type);
	/**@}*/

	/** @name GetComponentInChildren
	 * Returns the component of the specified type in this game object or
	 * any of its descendants in a depth first order if one is attached,
	 * nullptr otherwise. It is recommended to use the templatized function.
	 * Otherwise, the returned pointer should be casted to the desired
	 * type.
	 */
	/**@{*/
	/**
	 * @tparam T the type of Component to retrieve
	 * @return   pointer to the Component if one exists, nullptr otherwise
	 */
	template <class T>
	T *GetComponentInChildren();
	/**
	 * @param type the name of the type of Component to retrieve
	 * @return     pointer to the Component if one exists, nullptr otherwise
	 */
	Component *GetComponentInChildren(const String &type);
	/**@}*/

	/** @name GetComponentInParent
	 * Returns the component of the specified type in this game object or
	 * any of its ancestors if one is attached, nullptr otherwise.
	 */
	/**@{*/
	/**
	 * @tparam T the type of Component to retrieve
	 * @return   pointer to the Component if one exists, nullptr otherwise
	 */
	template <class T>
	T *GetComponentInParent();
	/**
	 * @param type the name of the type of Component to retrieve
	 * @return     pointer to the Component if one exists, nullptr otherwise
	 */
	Component *GetComponentInParent(const String &type);
	/**@}*/

	/**@{*/
	template <class T>
	void GetComponents(Vector<T *> &components);

	void GetComponents(const String &type, Vector<Component *> &components);
	/**@}*/

	/**@{*/
	template <class T>
	void GetComponentsInChildren(Vector<T *> &components);

	void GetComponentsInChildren(const String &type, Vector<Component *> &components);
	/**@}*/

	/**@{*/
	template <class T>
	void GetComponentsInParent(Vector<T *> &components);

	void GetComponentsInParent(const String &type, Vector<Component *> &components);
	/**@}*/

	void SetParent(GameObject *parent);
};

template <class T>
T *GameObject::AddComponent() {
	Component *newComp = Component::getPrototypes().at(typeid(T))->clone();
	GameObject::components.push_back(newComp);
	newComp->gameObject = this;
	return static_cast<T *>(newComp);
}

template <class T>
T *GameObject::GetComponent() {
	std::type_index idx = typeid(T);
	for (auto &comp : components) {
		if (comp->getType() == idx) {
			return static_cast<T *>(comp);
		}
	}
	return nullptr;
}

template <class T>
T *GameObject::GetComponentInChildren() {
	std::stack<GameObject *> stack;
	stack.push(this);
	Component *ret = nullptr;
	while (!stack.empty()) {
		GameObject *currObj = stack.top();
		stack.pop();
		if ((ret = currObj->GetComponent<T>()) != nullptr) {
			return ret;
		}
		for (auto &child : currObj->children) {
			stack.push(child);
		}
	}
	return static_cast<T *>(ret);
}

template <class T>
T *GameObject::GetComponentInParent() {
	Component *ret = nullptr;
	GameObject *currObj = this;
	while (currObj != nullptr && ret == nullptr) {
		ret = currObj->GetComponent<T>();
		currObj = currObj->parent;
	}
	return static_cast<T>(ret);
}

template <class T>
void GameObject::GetComponents(Vector<T *> &components) {

}

template <class T>
void GameObject::GetComponentsInChildren(Vector<T *> &components) {

}

template <class T>
void GameObject::GetComponentsInParent(Vector<T *> &components) {

}

}

#endif // __GAMEOBJECT_H__