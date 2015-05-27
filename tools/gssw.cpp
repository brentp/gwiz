#include "core/alignment/BamAlignmentReader.h"
#include "core/alignment/BamAlignmentReaderManager.h"
#include "core/alignment/BamAlignmentReaderPreloadManager.h"
#include "core/variant/VCFFileReader.h"
#include "core/variant/IVariant.h"
#include "core/variant/VariantListVCFPreloaded.h"
#include "core/reference/FastaReference.h"
#include "core/util/Parameters.h"
#include "gssw/graph/GraphManager.h"
#include "gssw/graph/GSSWAdjudicator.h"


int main(int argc, char** argv)
{
	gwiz::Parameters::Instance()->setParams(argc, argv);
	std::string fastaPath = gwiz::Parameters::Instance()->getFastaPath();
	std::string vcfPath = gwiz::Parameters::Instance()->getInVCFPath();
	std::string bamPath = gwiz::Parameters::Instance()->getBAMPath();
	std::string outputVCFPath = gwiz::Parameters::Instance()->getOutVCFPath();

	gwiz::Region::SharedPtr regionPtr = std::make_shared< gwiz::Region >("20");
	auto fastaReferencePtr = std::make_shared< gwiz::FastaReference >(fastaPath, regionPtr);
	auto vcfFileReaderPtr = std::make_shared< gwiz::VariantListVCFPreloaded >(vcfPath, regionPtr);
	auto bamAlignmentReaderPreloadManager = std::make_shared< gwiz::BamAlignmentReaderPreloadManager >(bamPath, regionPtr);
	vcfFileReaderPtr->loadVariantsFromFile();

	std::cout << "Finished loading BAM and VCF" << std::endl;

	auto gsswAdjudicator = std::make_shared< gwiz::gssw::GSSWAdjudicator >();
	auto gsswGraphManager = std::make_shared< gwiz::gssw::GraphManager >(fastaReferencePtr, vcfFileReaderPtr, bamAlignmentReaderPreloadManager, gsswAdjudicator);
	auto variantList = gsswGraphManager->buildGraphs(fastaReferencePtr->getRegion(), 3000, 1000, 100);

	std::cout << "starting to print vcf" << std::endl;

	if (outputVCFPath.size() > 0)
	{
		std::ofstream outVCF;
		outVCF.open(outputVCFPath, std::ios::out | std::ios::trunc);
		variantList->printToVCF(outVCF);
		outVCF.close();
	}
	else
	{
		variantList->printToVCF(std::cout);
	}
	return 0;
}
