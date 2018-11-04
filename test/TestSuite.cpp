#include "ComponentTests.h"
#include "GameObjectTests.h"
#include "TransformTests.h"


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	Kiwi::ServiceLocator::initLoggers();
	return RUN_ALL_TESTS();
}