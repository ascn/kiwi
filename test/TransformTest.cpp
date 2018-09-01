#include <gtest/gtest.h>
#include "Component.h"
#include "Transform.h"

using namespace Kiwi;

class TestComponent_1 : public Component {
public:
K_COMPONENT_H(TestComponent_1)

	~TestComponent_1() override {}

	Component *clone() override {
		return new TestComponent_1();
	}
};
K_COMPONENT_S(TestComponent_1, "TestComponent_1")

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
	transform->SetPosition()
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	Kiwi::Logger::Initialize();
	return RUN_ALL_TESTS();
}