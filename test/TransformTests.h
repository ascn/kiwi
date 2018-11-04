#include <gtest/gtest.h>
#include "Component.h"
#include "Transform.h"
#include "ServiceLocator.h"

#define ASSERT_VEC3_EQ(u, v, epsilon) \
Vector3 diff = glm::abs((u) - (v)); \
ASSERT_LT(diff.x, epsilon); \
ASSERT_LT(diff.y, epsilon); \
ASSERT_LT(diff.z, epsilon);

using namespace Kiwi;

#ifndef __TC1__
#define __TC1__
class TestComponent_1 : public Component {
public:
K_COMPONENT_H(TestComponent_1)

	~TestComponent_1() override {}

	Component *clone() override {
		return new TestComponent_1();
	}
};
K_COMPONENT_S(TestComponent_1)
#endif

TEST(TransformTest, CloneTransformTest) {
	Transform *transform = new Transform();
	transform->SetPosition(Vector3(6, 6, 6));
	Component *clonedComp = transform->clone();
	auto badCast = dynamic_cast<TestComponent_1 *>(clonedComp);
	ASSERT_EQ(badCast, nullptr);
	auto goodCast = dynamic_cast<Transform *>(clonedComp);
	ASSERT_NE(goodCast, nullptr);
	ASSERT_EQ(goodCast->GetPosition(), Vector3(6, 6, 6));
}

TEST(TransformTest, TransformDirectionTest) {
	Transform *transform = new Transform();
	transform->SetPosition(Vector3(0, 1, 0));
	transform->SetRotation(Vector3(0, 0, 30));
	Vector3 dir = Vector3(0, 2, 0);
	Vector3 transformed = transform->TransformDirection(dir);
	ASSERT_VEC3_EQ(Vector3(-1, 1.732051, 0), transformed, 1e-5);
}

TEST(TransformTest, LookAtTest) {
	Transform *transform = new Transform();
	transform->SetPosition(Vector3(1, 0, 0));
	transform->LookAt(Vector3(0, 0, 0), Transform::worldUp);
	Transform *freshTransform = new Transform();
	freshTransform->Rotate(Vector3(0, 90, 0));
}
