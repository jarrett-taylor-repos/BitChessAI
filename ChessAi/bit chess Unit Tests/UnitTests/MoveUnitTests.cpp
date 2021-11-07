#include "pch.h"
#include "CppUnitTest.h"
#include "../../bit chess/main.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MoveUnitTests
{
	TEST_CLASS(MoveUnitTests) 
	{
	public:
		TEST_METHOD(TestDefaultConstructor) {
			Move m;
			Assert::AreEqual(-1, m.getStartSq());
			Assert::AreEqual(-1, m.getTargetSq());
		}

		TEST_METHOD(TestParameterConstructor) {
			Move m(0, 1);
			Assert::AreEqual(0, m.getStartSq());
			Assert::AreEqual(1, m.getTargetSq());
		}

		TEST_METHOD(TestCopyConstructor) {
			Move m(0, 1);
			Move cpy = m;
			Assert::AreEqual(cpy.getStartSq(), m.getStartSq());
			Assert::AreEqual(cpy.getTargetSq(), m.getTargetSq());
		}
		TEST_METHOD(TestEqualOperator) {
			Move m(0, 1);
			Move cpy;
			cpy = m;
			Assert::AreEqual(cpy.getStartSq(), m.getStartSq());
			Assert::AreEqual(cpy.getTargetSq(), m.getTargetSq());
		}
		TEST_METHOD(TestEqualityOperator) {
			Move m(0, 1);
			Move m2(0, 1);
			bool equal = (m2 == m);
			Assert::IsTrue(equal);
			Move m3(1, 0);
			bool notequal = (m2 == m3);
			Assert::IsFalse(notequal);
		}
		TEST_METHOD(TestGetStartSq) {
			Move m(0, 1);
			int start = m.getStartSq();
			Assert::AreEqual(0, start);
		}
		TEST_METHOD(TestGetTargetSq) {
			Move m(0, 1);
			int target = m.getTargetSq();
			Assert::AreEqual(1, target);
		}

	};
}