#include <gtest/gtest.h>
#include "Component.h"
#include "GameObject.h"

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

#ifndef __TC2__
#define __TC2__
class TestComponent_2 : public Component {
public:
K_COMPONENT_H(TestComponent_2)

	~TestComponent_2() override {}

	Component *clone() override {
		return new TestComponent_2();
	}
};
K_COMPONENT_S(TestComponent_2)
#endif

TEST(GameObjectTest, AddComponentTypeTest) {
	GameObject obj("GameObject_1");
	TestComponent_1 *tc1 = obj.AddComponent<TestComponent_1>();
	ASSERT_EQ(obj.components.size(), 1);
	ASSERT_EQ(typeid(*(obj.components[0])), typeid(*tc1));
	TestComponent_2 *tc2 = obj.AddComponent<TestComponent_2>();
	ASSERT_EQ(obj.components.size(), 2);
}

TEST(GameObjectTest, AddComponentStringTest) {
	GameObject obj("GameObject_1");
	Component *tc1 = obj.AddComponent("TestComponent_1");
	ASSERT_EQ(obj.components.size(), 1);
	ASSERT_EQ(typeid(*(obj.components[0])), typeid(*tc1));
	Component *tc2 = obj.AddComponent("TestComponent_2");
	ASSERT_EQ(obj.components.size(), 2);
}

TEST(GameObjectTest, GetComponentTest) {
	GameObject obj("GameObject_1");
	Component *tc1 = obj.AddComponent("TestComponent_1");
	ASSERT_EQ(obj.GetComponent("TestComponent_1"), tc1);
	Component *tc1_2 = obj.AddComponent("TestComponent_1");
	ASSERT_NE(obj.GetComponent("TestComponent_1"), tc1_2);
	ASSERT_EQ(obj.GetComponent("TestComponent_1"), tc1);
}
