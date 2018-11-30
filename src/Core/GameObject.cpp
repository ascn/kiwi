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
		components[i] = std::unique_ptr<Component>(other.components[i]->clone());
	}

	for (int i = 0; i < other.children.size(); ++i) {
		children[i] = std::make_unique<GameObject>(*other.children[i]);
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
}

GameObject &GameObject::operator=(GameObject other) {
	swap(*this, other);
	return *this;
}

void GameObject::SetParent(GameObject *parent) {

}

}