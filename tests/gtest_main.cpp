#include <stdio.h>

#include "gtest/gtest.h"

/*
#include "VCFFileTests.hpp"
#include "BuildGraphTests.hpp"
#include "VariantsTest.hpp"
#include "ReferenceTest.hpp"
#include "RegionTests.hpp"
#include "FileTests.hpp"
#include "BamAlignmentTests.hpp"
#include "VariantListVCFPreloadedTests.hpp"
*/
#include "SequenceManagerTests.hpp"

#include "plugins/TestIncludes.h"


#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
