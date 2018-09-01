#include <gtest/gtest.h>
#include "Component.h"

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

class TestComponent_2 : public Component {
public:
K_COMPONENT_H(TestComponent_2)

	~TestComponent_2() override {}

	Component *clone() override {
		return new TestComponent_2();
	}
};
K_COMPONENT_S(TestComponent_2, "TestComponent_2")

TEST(ComponentTest, GetTypeTest) {
	Component *c1 = new TestComponent_1();
	Component *c2 = new TestComponent_2();
	ASSERT_EQ(std::type_index(typeid(TestComponent_1)), c1->getType());
	ASSERT_EQ(std::type_index(typeid(TestComponent_2)), c2->getType());
	ASSERT_NE(std::type_index(typeid(TestComponent_1)), c2->getType());
	ASSERT_NE(std::type_index(typeid(TestComponent_2)), c1->getType());
	String test;
	try {
		test = Component::getRegistry().get(typeid(Component));
	} catch (const std::out_of_range &e) {
		test = "";
	}
	ASSERT_EQ(test, "");
}

TEST(ComponentTest, RegistrarTest) {
	ASSERT_EQ(Component::getRegistry().get(typeid(TestComponent_1)), "TestComponent_1");
	ASSERT_EQ(Component::getRegistry().get("TestComponent_1"), typeid(TestComponent_1));
	ASSERT_EQ(Component::getRegistry().get(typeid(TestComponent_2)), "TestComponent_2");
	ASSERT_EQ(Component::getRegistry().get("TestComponent_2"), typeid(TestComponent_2));
}

TEST(ComponentTest, RegistrarGetTypeIntegrationTest) {
	Component *c1 = new TestComponent_1();
	Component *c2 = new TestComponent_2();
	ASSERT_EQ(Component::getRegistry().get("TestComponent_1"), c1->getType());
	ASSERT_NE(Component::getRegistry().get("TestComponent_2"), c1->getType());
}

TEST(ComponentTest, PrototypeTest) {
	Component *clonedFromProto = Component::getPrototypes().at(typeid(TestComponent_1))->clone();
	ASSERT_EQ(typeid(*clonedFromProto), typeid(TestComponent_1));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	Kiwi::Logger::Initialize();
	return RUN_ALL_TESTS();
}