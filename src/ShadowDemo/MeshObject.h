#include "Core/GameObject.h"
#include "Rendering/Mesh.h"
#include "Rendering/MeshRenderer.h"

using namespace Kiwi;

class MeshObject : public GameObject {
public:
	MeshObject(const String &name, Mesh *mesh, Material *material, Scene *scene) :
		GameObject(name)
	{
		AddComponent<Kiwi::MeshFilter>(mesh);
		AddComponent<Kiwi::MeshRenderer>(*scene).material = material;
	}
};