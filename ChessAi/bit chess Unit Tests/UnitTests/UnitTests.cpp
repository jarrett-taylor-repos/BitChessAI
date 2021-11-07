#include "pch.h"
#include "CppUnitTest.h"
#include "../../bit chess/main.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(PieceUnitTests)
	{
	public:
		TEST_METHOD(ValueIsNone)
		{
			const char test = None();
			Assert::AreEqual(test, none);
		}
		TEST_METHOD(ValueIsWhitePawn)
		{
			const char test = WhitePawn();
			const char piece = pawn + white;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsWhiteKnight)
		{
			const char test = WhiteKnight();
			const char piece = knight + white;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsWhiteBishop)
		{
			const char test = WhiteBishop();
			const char piece = bishop + white;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsWhiteRook)
		{
			const char test = WhiteRook();
			const char piece = rook + white;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsWhiteQueen)
		{
			const char test = WhiteQueen();
			const char piece = queen + white;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsWhiteKing)
		{
			const char test = WhiteKing();
			const char piece = king + white;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsBlackPawn)
		{
			const char test = BlackPawn();
			const char piece = pawn + black;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsBlackKnight)
		{
			const char test = BlackKnight();
			const char piece = knight + black;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsBlackBishop)
		{
			const char test = BlackBishop();
			const char piece = bishop + black;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsBlackRook)
		{
			const char test = BlackRook();
			const char piece = rook + black;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsBlackQueen)
		{
			const char test = BlackQueen();
			const char piece = queen + black;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(ValueIsBlackKing)
		{
			const char test = BlackKing();
			const char piece = king + black;
			Assert::AreEqual(test, piece);
		}
		TEST_METHOD(TestIfWhite) {
			vector<char> whitePieces = { WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing()};
			for (int i = 0; i < whitePieces.size(); i++) {
				char piece = whitePieces[i];
				bool pieceIsWhite = isWhite(piece);
				Assert::IsTrue(pieceIsWhite);
			}
			vector<char> blackPieces = { BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(), None() };
			for (int i = 0; i < blackPieces.size(); i++) {
				char piece = blackPieces[i];
				bool pieceIsWhite = isWhite(piece);
				Assert::IsFalse(pieceIsWhite);
			}
		}
		TEST_METHOD(TestIfBlack) {
			vector<char> whitePieces = { WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(), None() };
			for (int i = 0; i < whitePieces.size(); i++) {
				char piece = whitePieces[i];
				bool pieceIsBlack = isBlack(piece);
				Assert::IsFalse(pieceIsBlack);
			}
			vector<char> blackPieces = { BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing() };
			for (int i = 0; i < blackPieces.size(); i++) {
				char piece = blackPieces[i];
				bool pieceIsBlack = isBlack(piece);
				Assert::IsTrue(pieceIsBlack);
			}
		}
		TEST_METHOD(TestGetPieceValue) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),  
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> pieceValue = {
				0, 1, 2, 3, 4, 5, 6,
				1, 2, 3, 4, 5, 6,
				1, 2, 3, 4, 5, 6
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				char piecevalue = getPieceValue(p);
				char pVal = pieceValue[i];
				Assert::AreEqual(piecevalue, pVal);
			}
		}
		TEST_METHOD(TestIsKing) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				false,
				false, false, false, false, false, true,
				false, false, false, false, false, true,
				false, false, false, false, false, true,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool testKing = isKing(p);
				bool isKing = isValue[i];
				if (isKing) {
					Assert::IsTrue(testKing);
				}
				else {
					Assert::IsFalse(testKing);
				}
			}
		}
		TEST_METHOD(TestIsQueen) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				false,
				false, false, false, false, true, false,
				false, false, false, false, true, false,
				false, false, false, false, true, false,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isQueen(p);
				bool isPiece = isValue[i];
				if (isPiece) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestIsRook) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				false,
				false, false, false, true, false, false,
				false, false, false, true, false, false,
				false, false, false, true, false, false,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isRook(p);
				bool isPiece = isValue[i];
				if (isPiece) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestIsBishop) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				false,
				false, false, true, false, false, false,
				false, false, true, false, false, false,
				false, false, true, false, false, false,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isBishop(p);
				bool isPiece = isValue[i];
				if (isPiece) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestIsKnight) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				false,
				false, true, false, false, false, false,
				false, true, false, false, false, false,
				false, true, false, false, false, false,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isKnight(p);
				bool isPiece = isValue[i];
				if (isPiece) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestIsPawn) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				false,
				true, false, false, false, false, false,
				true, false, false, false, false, false,
				true, false, false, false, false, false,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isPawn(p);
				bool isPiece = isValue[i];
				if (isPiece) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestIsNone) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			vector<char> isValue = {
				true,
				false, false, false, false, false, false,
				false, false, false, false, false, false,
				false, false, false, false, false, false,
			};
			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isNone(p);
				bool isPiece = isValue[i];
				if (isPiece) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestIsColor) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};
			bool test1 = isColor(white, WhitePawn());
			Assert::IsTrue(test1);
			bool test2 = isColor(black, WhitePawn());
			Assert::IsFalse(test2);
			bool test3 = isColor(none, WhitePawn());
			Assert::IsFalse(test3);
			bool test4 = isColor(none, BlackPawn());
			Assert::IsFalse(test4);
			bool test5 = isColor(black, BlackPawn());
			Assert::IsTrue(test5);
		}
		TEST_METHOD(TestIsSlidingPiece) {
			vector<char> pieces = {
				None(),
				WhitePawn(), WhiteKnight(), WhiteBishop(), WhiteRook(), WhiteQueen(), WhiteKing(),
				BlackPawn(), BlackKnight(), BlackBishop(), BlackRook(), BlackQueen(), BlackKing(),
				pawn, knight, bishop, rook, queen, king
			};

			vector<bool> isSliding = {
				false,
				false, false, true, true, true, false,
				false, false, true, true, true, false,
				false, false, true, true, true, false
			};

			for (int i = 0; i < pieces.size(); i++) {
				char p = pieces[i];
				bool test = isSlidingPiece(p);
				bool isSlidingValue = isSliding[i];
				if (isSlidingValue) {
					Assert::IsTrue(test);
				}
				else {
					Assert::IsFalse(test);
				}
			}
		}
		TEST_METHOD(TestMuiltMapAttackers)
		{
			multimap<int, precomputedAttackerData> attackersOnWhite = precomputtedPossibleAttackers(white);;
			multimap<int, precomputedAttackerData>::iterator itr;
			for (itr = attackersOnWhite.begin(); itr != attackersOnWhite.end(); ++itr) {
				int startSq = itr->first;
				precomputedAttackerData pre = itr->second;
				bool isPin = pre.getPossiblePin();
				if (isPin) {
					int dist = pre.getNumSquaresAway();
					int diff = abs(pre.getTargetSq() - startSq);
					int diff_offset = diff / dist;
					bool isCorrectOffset = (diff_offset == 1) || (diff_offset == 7) || (diff_offset == 8) || (diff_offset == 9);
					if (isCorrectOffset) {
						Assert::IsTrue(isCorrectOffset);
					}
					else {
						Assert::IsFalse(isCorrectOffset);
					}
					
				}
			}
		}
		TEST_METHOD(TestNotationMap) {
			map<int, string> intToStringMap = precomputtedIntToString();
			map<string, int> stringToIntMap = precomputtedStringToInt();
			for (int i = 0; i < 64; i++) {
				multimap<int, string>::iterator it = intToStringMap.find(i);
				string notation = it->second;
				multimap<string, int>::iterator it2 = stringToIntMap.find(notation);
				int notationInt = it2->second;
				Assert::AreEqual(i, notationInt);
			}
		}
	};

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
			precomputedAttackerData cpy = pre(1, char(2), true, 3);
			Assert::AreEqual(1, cpy.getTargetSq());
			Assert::AreEqual(char(2), cpy.getPiece());
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

	TEST_CLASS(AttackerUnitTests) 
	{
	public:
		TEST_METHOD(TestDefaultConstructor) {
			Attacker att;
			Assert::AreEqual(-1, att.getStartSq());
			Assert::AreEqual(-1, att.getTargetSq());
			Assert::AreEqual(char(-1), att.getPiece());
		}

		TEST_METHOD(TestParamConstructor) {
			Attacker att(1, 2, char(3));
			Assert::AreEqual(1, att.getStartSq());
			Assert::AreEqual(2, att.getTargetSq());
			Assert::AreEqual(char(3), att.getPiece());
		}

		TEST_METHOD(TestCopyConstructor) {
			Attacker att(1, 2, char(3));
			Attacker cpy = att;
			Assert::AreEqual(1, cpy.getStartSq());
			Assert::AreEqual(2, cpy.getTargetSq());
			Assert::AreEqual(char(3), cpy.getPiece());
		}

		TEST_METHOD(TestEqualOperator) {
			Attacker att(1, 2, char(3));
			Attacker cpy;
			cpy = att;
			Assert::AreEqual(1, cpy.getStartSq());
			Assert::AreEqual(2, cpy.getTargetSq());
			Assert::AreEqual(char(3), cpy.getPiece());
		}

		TEST_METHOD(TestGetStartSq) {
			Attacker att(1, 2, char(3));
			Assert::AreEqual(1, att.getStartSq());
		}

		TEST_METHOD(TestGetTargetSq) {
			Attacker att(1, 2, char(3));
			Assert::AreEqual(2, att.getTargetSq());
		}

		TEST_METHOD(TestGetPiece) {
			Attacker att(1, 2, char(3));
			Assert::AreEqual(char(3), att.getPiece());
		}

	};

	TEST_CLASS(BoardUnitTests) {
	public:
		TEST_METHOD(TestDefaultConstructor) {

		}
		TEST_METHOD(TestCopyConstructor) {

		}
		TEST_METHOD(TestEqualOperator) {

		}
		TEST_METHOD(TestLoadFEN) {

		}
		TEST_METHOD(TestClearSquare) {

		}
		TEST_METHOD(TestAddSquare) {

		}
		TEST_METHOD(TestMakeMove) {

		}
		TEST_METHOD(TestSetAttckers) {

		}
		TEST_METHOD(TestGenerateMoves) {

		}
		TEST_METHOD(TestIsPiecePinned) {

		}
		TEST_METHOD(TestGetChecks) {

		}
		TEST_METHOD(TestNoPiecesBetween) {

		}
		TEST_METHOD(TestSetMoveColor) {

		}
		TEST_METHOD(TestPieceToString) {

		}
		TEST_METHOD(TestIntToStringNotation) {

		}
		TEST_METHOD(TestStringToIntSquare) {

		}
	};
}
