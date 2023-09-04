#include "pch.h"
#include "CppUnitTest.h"

#include "OpenLog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OpenLogTests
{

	TEST_CLASS(Tag)
	{
	public:
		
		TEST_METHOD(Name_Parameter_Passed_Successfully)
		{
			std::string expected{ "Test case" };

			OpenLog::Tag testTag(expected);
			Assert::AreEqual(expected, testTag.str());
		}

		TEST_METHOD(Name_Parameter_Not_Given)
		{
			std::string expected{ "TAG" };

			OpenLog::Tag testTag;
			Assert::AreEqual(expected, testTag.str());
		}
		TEST_METHOD(Name_Parameter_OperatorOverride)
		{
			std::string expected{ "case1" };

			OpenLog::Tag testTag(expected);

			std::ostringstream actual; actual << testTag;
			Assert::AreEqual(expected, actual.str());
		}
	};

	TEST_CLASS(Log) {
	public:

	};
}
