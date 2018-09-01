#include <stack>
#include "GameObject.h"

namespace Kiwi {

GameObject::GameObject(String name) :
	Object(name),
	components(),
	parent(nullptr),
	children()
	{}

Component *GameObject::AddComponent(const String &type) {
	std::type_index idx = Component::getRegistry().get(type);
	Component *newComp = Component::getPrototypes().at(idx)->clone();
	GameObject::components.push_back(newComp);
	newComp->gameObject = this;
	return newComp;
}

Component *GameObject::GetComponent(const String &type) {
	std::type_index idx = Component::getRegistry().get(type);
	for (auto &comp : components) {
		if (comp->getType() == idx) {
			return comp;
		}
	}
	return nullptr;
}

Component *GameObject::GetComponentInChildren(const String &type) {
	std::stack<GameObject *> stack;
	stack.push(this);
	Component *ret = nullptr;
	while (!stack.empty()) {
		GameObject *currObj = stack.top();
		stack.pop();
		if ((ret = currObj->GetComponent(type)) != nullptr) {
			return ret;
		}
		for (auto &child : currObj->children) {
			stack.push(child);
		}
	}
	return ret;
}

Component *GameObject::GetComponentInParent(const String &type) {
	Component *ret = nullptr;
	GameObject *currObj = this;
	while (currObj != nullptr && ret == nullptr) {
		ret = currObj->GetComponent(type);
		currObj = currObj->parent;
	}
	return ret;
}

void GameObject::GetComponents(const String &type, Vector<Component *> &components) {
	std::type_index idx = Component::getRegistry().get(type);
	for (auto &comp : GameObject::components) {
		if (comp->getType() == idx) {
			components.push_back(comp);
		}
	}
}

void GameObject::GetComponentsInChildren(const String &type, Vector<Component *> &components) {
	std::stack<GameObject *> stack;
	stack.push(this);
	while (!stack.empty()) {
		GameObject *currObj = stack.top();
		stack.pop();
		currObj->GetComponents(type, components);
		for (auto &child : currObj->children) {
			stack.push(child);
		}
	}
}

void GameObject::GetComponentsInParent(const String &type, Vector<Component *> &components) {
	GameObject *currObj = this;
	while (currObj != nullptr) {
		currObj->GetComponents(type, components);
		currObj = currObj->parent;
	}
}

void GameObject::SetParent(GameObject *parent) {

}

}