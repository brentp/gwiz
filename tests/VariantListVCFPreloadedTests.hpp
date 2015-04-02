#ifndef GWIZ_VARIANTLISTVCFPRELOADEDTESTS_HPP
#define GWIZ_VARIANTLISTVCFPRELOADEDTESTS_HPP

#include <stdexcept>

#include "core/variants/VariantListVCFPreloaded.h"
#include "core/region/Region.h"

#include "TestConfig.h"

TEST(VariantListVCFPreloadedTests, TestVariantListVCFPreloadedNoRegion)
{
	std::string chrom = "Y";
	gwiz::position startPosition = 2655180;
	gwiz::position endPosition = 28770931;
	/*
    std::string regionString = chrom + ":" + std::to_string(startPosition) + "-" + std::to_string(endPosition);
	gwiz::Region::SharedPtr region = std::make_shared< gwiz::Region >(regionString);
	*/
	std::string path = TEST_1KG_CHRY_VCF_FILE;
	auto vcfFileReader = std::make_shared<gwiz::VariantListVCFPreloaded>(path);
	vcfFileReader->loadVariantsFromFile();
	gwiz::Variant::SharedPtr variantPtr;
	gwiz::Variant::SharedPtr prevVariantPtr;
	vcfFileReader->getNextVariant(variantPtr);
	ASSERT_EQ(variantPtr->getPosition(), startPosition);
	size_t count = 1;
	while (vcfFileReader->getNextVariant(variantPtr))
	{
		++count;
		ASSERT_STREQ(chrom.c_str(), variantPtr->getChrom().c_str());
		ASSERT_LE(startPosition, variantPtr->getPosition());
		ASSERT_GE(endPosition, variantPtr->getPosition());
		prevVariantPtr = variantPtr; // so we can check the last position
	}
	ASSERT_EQ(prevVariantPtr->getPosition(), endPosition);
	ASSERT_EQ(count, vcfFileReader->getCount());
}

TEST(VariantListVCFPreloadedTests, TestVariantListVCFPreloadedRegion)
{
	std::string chrom = "Y";
	gwiz::position startPosition = 2658789;
	gwiz::position endPosition = 2662351;
    std::string regionString = chrom + ":" + std::to_string(startPosition) + "-" + std::to_string(endPosition);
	gwiz::Region::SharedPtr regionPtr = std::make_shared< gwiz::Region >(regionString);
	std::string path = TEST_1KG_CHRY_VCF_FILE;
	auto vcfFileReader = std::make_shared<gwiz::VariantListVCFPreloaded>(path, regionPtr);
	vcfFileReader->loadVariantsFromFile();
	gwiz::Variant::SharedPtr variantPtr;
	gwiz::Variant::SharedPtr prevVariantPtr;
	vcfFileReader->getNextVariant(variantPtr);
	ASSERT_EQ(variantPtr->getPosition(), startPosition);
	size_t count = 1;
	while (vcfFileReader->getNextVariant(variantPtr))
	{
		++count;
		ASSERT_STREQ(chrom.c_str(), variantPtr->getChrom().c_str());
		ASSERT_LE(startPosition, variantPtr->getPosition());
		ASSERT_GE(endPosition, variantPtr->getPosition());
		prevVariantPtr = variantPtr; // so we can check the last position
	}
	ASSERT_EQ(prevVariantPtr->getPosition(), endPosition);
	ASSERT_EQ(count, 21);
}

/*
TEST(VCFFileReaderTests, TestVCFFileReaderGetRegionBeforeStart)
{
	std::string chrom = "Y";
	gwiz::position startPosition = 2655179;
	gwiz::position endPosition = 2656000;
	gwiz::position firstPosition = 2655180;
    std::string regionString = chrom + ":" + std::to_string(startPosition) + "-" + std::to_string(endPosition);
	gwiz::Region::SharedPtr region = std::make_shared< gwiz::Region >(regionString);
	std::string path = TEST_1KG_CHRY_VCF_FILE;
	auto vcfFileReader = std::make_shared<gwiz::VCFFileReader>(path, region);
	gwiz::Variant::SharedPtr variantPtr;
	vcfFileReader->getNextVariant(variantPtr);
	ASSERT_EQ(variantPtr->getPosition(), firstPosition);
}

TEST(VCFFileReaderTests, TestVCFFileReaderGetRegionEnd)
{
	std::string chrom = "Y";
	gwiz::position startRegionPosition = 28760931;
	gwiz::position endRegionPosition = 38770931;
	gwiz::position endPosition = 28770931;
    std::string regionString = chrom + ":" + std::to_string(startRegionPosition) + "-" + std::to_string(endRegionPosition);
	gwiz::Region::SharedPtr region = std::make_shared< gwiz::Region >(regionString);
	std::string path = TEST_1KG_CHRY_VCF_FILE;
	auto vcfFileReader = std::make_shared<gwiz::VCFFileReader>(path, region);
	gwiz::Variant::SharedPtr variantPtr;
	gwiz::Variant::SharedPtr prevVariantPtr;
	while (vcfFileReader->getNextVariant(variantPtr))
	{
		prevVariantPtr = variantPtr;
	}
	ASSERT_EQ(prevVariantPtr->getPosition(), endPosition);
}

TEST(VCFFileReaderTests, TestVCFFileReaderGetAfterLastVariantIsNULL)
{
	std::string chrom = "Y";
	gwiz::position startRegionPosition = 28760931;
	gwiz::position endRegionPosition = 38770931;
	gwiz::position endPosition = 28770931;
    std::string regionString = chrom + ":" + std::to_string(startRegionPosition) + "-" + std::to_string(endRegionPosition);
	gwiz::Region::SharedPtr region = std::make_shared< gwiz::Region >(regionString);
	std::string path = TEST_1KG_CHRY_VCF_FILE;
	auto vcfFileReader = std::make_shared<gwiz::VCFFileReader>(path, region);
	gwiz::Variant::SharedPtr variantPtr;
	while (vcfFileReader->getNextVariant(variantPtr))
	{
		ASSERT_TRUE(variantPtr != NULL);
	}
	ASSERT_TRUE(variantPtr == NULL);
}

TEST(VCFFileReaderTests, TestVCFFileReaderWithoutRegion)
{
	std::string path = TEST_1KG_CHRY_VCF_FILE;
	auto vcfFileReader = std::make_shared<gwiz::VCFFileReader>(path);
	gwiz::Variant::SharedPtr variantPtr;
	while (vcfFileReader->getNextVariant(variantPtr))
	{
	}
	ASSERT_TRUE(true); // if we get here then we have completed the test
}
*/
/*
 * Makes sure that a region without any variants throws an exception
 */
/*
TEST(VCFFileReaderTests, TestVCFFileReaderGetNoRegion)
{
	std::string chrom = "Y";
	gwiz::position startRegionPosition = 0;
	gwiz::position endRegionPosition = 200;
    std::string regionString = chrom + ":" + std::to_string(startRegionPosition) + "-" + std::to_string(endRegionPosition);
	gwiz::Region::SharedPtr region = std::make_shared< gwiz::Region >(regionString);
	std::string path = TEST_1KG_CHRY_VCF_FILE;
    ASSERT_ANY_THROW(std::make_shared<gwiz::VCFFileReader>(path, region));
}
*/

#endif // GWIZ_VARIANTLISTVCFPRELOADEDTESTS_HPP
