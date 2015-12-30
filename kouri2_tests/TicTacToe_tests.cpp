#include "stdafx.h"
#include "CppUnitTest.h"
#include "../kouri2/TicTacToe.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace kouri_v20_tests
{		
	TEST_CLASS(TicTacToe_tests)
	{
	public:
		
		TEST_METHOD(Test_is_gameOver)
		{
			// TODO: Your test code here
			TicTacToe ttt;
			Assert::IsFalse(ttt.is_gameOver());
		}

	};
}