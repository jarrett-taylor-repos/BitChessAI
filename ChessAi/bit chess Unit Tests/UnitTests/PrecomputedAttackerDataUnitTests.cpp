#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PrecomputedAttackerDataUnitTests
{
	TEST_CLASS(PrecomputedAttackerDataUnitTests)
	{
	public:
		TEST_METHOD(TestDefaultConstructor) {
			precomputedAttackerData pre;
			Assert::AreEqual(-1, pre.getTargetSq());
			Assert::AreEqual((char)-1, pre.getPiece());
			Assert::AreEqual(false, pre.getPossiblePin());
			Assert::AreEqual(0, pre.getNumSquaresAway());
		}

		TEST_METHOD(TestParamterConstructor) {
			precomputedAttackerData pre(1, char(2), 1, 3);
			Assert::AreEqual(1, pre.getTargetSq());
			Assert::AreEqual((char)2, pre.getPiece());
			Assert::AreEqual(true, pre.getPossiblePin());
			Assert::AreEqual(3, pre.getNumSquaresAway());
		}

		TEST_METHOD(TestCopyConstructor) {
			precomputedAttackerData pre(1, 2, true, 3);
			precomputedAttackerData cpy = pre;
			Assert::AreEqual(1, cpy.getTargetSq());
			Assert::AreEqual((char)2, cpy.getPiece());
			Assert::AreEqual(true, cpy.getPossiblePin());
			Assert::AreEqual(3, cpy.getNumSquaresAway());
		}

		TEST_METHOD(TestEqualOperator) {
			precomputedAttackerData pre(1, 2, true, 3);
			precomputedAttackerData cpy;
			cpy = pre;
			Assert::AreEqual(1, cpy.getTargetSq());
			Assert::AreEqual((char)2, cpy.getPiece());
			Assert::AreEqual(true, cpy.getPossiblePin());
			Assert::AreEqual(3, cpy.getNumSquaresAway());
		}

		TEST_METHOD(TestParaenthesisOperator) {
			precomputedAttackerData pre;
			precomputedAttackerData cpy = pre(1, char(), true, 3);
			Assert::AreEqual(1, cpy.getTargetSq());
			Assert::AreEqual((char)2, cpy.getPiece());
			Assert::AreEqual(true, cpy.getPossiblePin());
			Assert::AreEqual(3, cpy.getNumSquaresAway());
		}

		TEST_METHOD(TestGetTargetSq) {
			precomputedAttackerData pre;
			Assert::AreEqual(-1, pre.getTargetSq());
		}

		TEST_METHOD(TestGetPiece) {
			const char piece = 0b1;
			precomputedAttackerData pre(0, piece, 1, 1);
			Assert::AreEqual(piece, pre.getPiece());
		}

		TEST_METHOD(TestGetPossiblePin) {
			bool pin = true;
			precomputedAttackerData pre(int(), char(), true, 1);
			Assert::AreEqual(true, pre.getPossiblePin());
		}

		TEST_METHOD(TestGetNumSquaresAway) {
			precomputedAttackerData pre(int(), char(), false, 2);
			Assert::AreEqual(2, pre.getNumSquaresAway());
		}
	};
}
		