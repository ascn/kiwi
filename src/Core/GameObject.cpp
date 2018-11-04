#include <stack>
#include "Core/GameObject.h"
#include "Scene/Transform.h"

namespace Kiwi {

GameObject::GameObject(String name) :
	Object(name),
	components(),
	parent(nullptr),
	children()
{
	AddComponent<Transform>();
}

GameObject::GameObject(const GameObject &other) :
	Object(other.name),
	components(other.components.size()),
	parent(other.parent),
	children(other.children.size())
{
	for (int i = 0; i < other.components.size(); ++i) {
		components[i] = other.components[i]->clone();
	}

	for (int i = 0; i < other.children.size(); ++i) {
		children[i] = other.children[i]->clone();
	}
}

GameObject *GameObject::clone() const {
	return new GameObject(*this);
}

GameObject::GameObject(GameObject &&other) :
	GameObject(other.name)
{
	swap(*this, other);
}

GameObject::~GameObject() {
	for (auto &comp : components) {
		delete comp;
	}
	for (auto &child : children) {
		delete child;
	}
}

GameObject &GameObject::operator=(GameObject other) {
	swap(*this, other);
	return *this;
}

Component *GameObject::AddComponent(const String &type) {
	Component *ret = nullptr;
	try {
		std::type_index idx = Component::getRegistry().get(type);
		Component *newComp = Component::getPrototypes().at(idx)->clone();
		GameObject::components.push_back(newComp);
		newComp->gameObject = this;
		ret = newComp;
	} catch (std::exception &e) {
		ServiceLocator::getLogger().Log(e.what());
	}
	return ret;
}

Component *GameObject::GetComponent(const String &type) {
	Component *ret = nullptr;
	try {
		std::type_index idx = Component::getRegistry().get(type);
		for (auto &comp : components) {
			if (comp->getType() == idx) {
				ret = comp;
				break;
			}
		}
	} catch (std::exception &e) {
		ServiceLocator::getLogger().Log(e.what());
	}
	return ret;
}

Component *GameObject::GetComponentInChildren(const String &type) {
	std::stack<GameObject *> stack;
	stack.push(this);
	Component *ret = nullptr;
	while (!stack.empty()) {
		GameObject *currObj = stack.top();
		stack.pop();
		if ((ret = currObj->GetComponent(type)) != nullptr) {
			break;
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