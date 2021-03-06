#include "Variant.h"
#include "core/alignments/IAlignment.h"

namespace gwiz
{
	Variant::Variant() :
		m_total_allele_count_low_quality(0),
		m_total_allele_count(0)
	{
		static uint32_t uniqueID;
		m_unique_id = uniqueID;
		++uniqueID;
	}

	Variant::~Variant()
	{
	}

	void Variant::initializeAlleleCounters()
	{
		std::lock_guard< std::mutex > lock(this->m_allele_count_mutex);
		m_allele_count[getRef()] = std::make_tuple(0, 0);
		for (const auto& alt : getAlt())
		{
			m_allele_count[alt] = std::make_tuple(0, 0);
		}
	}

	void Variant::incrementLowQualityCount(std::shared_ptr< IAlignment > alignmentPtr)
	{
		std::lock_guard< std::mutex > lock(this->m_allele_count_mutex);
		auto alignmentID = alignmentPtr->getID();
		if (this->m_alignment_ids_low_quality.find(alignmentID) != this->m_alignment_ids_low_quality.end()) { return; } // because of graph overlap we make sure we aren't counting alignments we've already counted
		this->m_alignment_ids_low_quality.emplace(alignmentID, true);
		++m_total_allele_count_low_quality;
	}

	void Variant::increaseCount(std::shared_ptr< IAlignment > alignmentPtr)
	{
		std::lock_guard< std::mutex > lock(this->m_allele_count_mutex);
		std::string allele = alignmentPtr->getVariantAllele(getVariantID());
		if (!alignmentPtr->isReverseStrand())
		{
			++std::get< 0 >(m_allele_count[allele]);
		}
		else
		{
			++std::get< 1 >(m_allele_count[allele]);
		}
		++this->m_total_allele_count;
	}

	size_t Variant::getSmallestAlleleSize()
	{
		size_t smallest = this->m_ref.size();
		for (auto variant : this->m_alt)
		{
			if (variant.size() < smallest) { smallest = variant.size(); }
		}
		return smallest;
	}

	size_t Variant::getLargestAlleleSize()
	{
		size_t largest = this->m_ref.size();
		for (auto& variant : this->m_alt)
		{
			if (variant.size() > largest) { largest = variant.size(); }
		}
		return largest;
	}

	std::string Variant::getAlleleCountString()
	{
		std::lock_guard< std::mutex > lock(this->m_allele_count_mutex);
		std::string alleleCountString = "";
		auto refAlleleTuple = this->m_allele_count[getRef()];
		alleleCountString += std::to_string(std::get< 0 >(refAlleleTuple)) + ",";
		alleleCountString += std::to_string(std::get< 1 >(refAlleleTuple));
		for (auto& alt : getAlt())
		{
			auto altAlleleTuple = this->m_allele_count[alt];
			alleleCountString += ",";
			alleleCountString += std::to_string(std::get< 0 >(altAlleleTuple)) + ",";
			alleleCountString += std::to_string(std::get< 1 >(altAlleleTuple));
		}
		return alleleCountString;
	}

	std::string Variant::alleleString()
	{
		std::string alleleString = "";
		alleleString += getRef() + "\t";
		for (auto& alt : getAlt())
		{
			alleleString += alt + ",";
		}
		alleleString.pop_back(); // removes the last space off the end
		return alleleString;
	}

	bool Variant::hasAlts()
	{
		std::lock_guard< std::mutex > lock(this->m_allele_count_mutex);
		for (auto& alt : getAlt())
		{
			auto ac = this->m_allele_count.find(alt);
			if ((ac != this->m_allele_count.end() && std::get< 0 >(ac->second) > 0) ||
				(ac != this->m_allele_count.end() && std::get< 1 >(ac->second) > 0))
			{
				return true;
			}
		}
		return false;
	}

	void Variant::addPotentialAlignment(const IAlignment::SharedPtr alignmentPtr)
	{
		std::lock_guard< std::mutex > lock(this->m_potential_alignment_mutex);
		this->m_potential_alignments.emplace_back(alignmentPtr);
	}

	void Variant::calculateAlleleCounts()
	{
		std::lock_guard< std::mutex > lock(this->m_potential_alignment_mutex);
		std::unordered_map< IAlignment::SharedPtr, bool > seenAlignments;
		for (const auto alignmentPtr : this->m_potential_alignments)
		{
			if (seenAlignments.find(alignmentPtr) != seenAlignments.end()) { continue; }
			seenAlignments[alignmentPtr] = true;
			int32_t mappingScore = alignmentPtr->getVariantMappingScore(getVariantID());
			if (0 <= mappingScore)
			{
				increaseCount(alignmentPtr);
			}
		}
	}

	std::string Variant::getGenotype()
	{
		std::lock_guard< std::mutex > lock(this->m_allele_count_mutex);
		float thresholdPercent = 0.3; // if there isn't at least this percent represented in the allele count then we don't consider it
		std::vector< std::tuple< std::string, uint32_t > > potentialGenotypes;

		// get the reference count
		auto rc = this->m_allele_count[this->getRef()];
		uint32_t rCount = std::get< 0 >(rc) + std::get< 1 >(rc);
		if (rCount > 3 && rCount >= (this->m_total_allele_count * thresholdPercent)) // if the reference count meets our criteria then add it to our potential genotypes
		{
			potentialGenotypes.emplace_back(std::make_tuple("0", rCount));
		}

		for (uint32_t i = 0; i < this->m_alt.size(); ++i)
		{
			auto ac = this->m_allele_count[this->m_alt[i]];
			uint32_t aCount = std::get< 0 >(ac) + std::get< 1 >(ac);
			if (aCount > 3 && aCount >= (this->m_total_allele_count * thresholdPercent)) // if the alt count meets our criteria then add it to our potential genotypes
			{
				uint32_t alleleIndex = i + 1; // we add 1 because 0 is the ref and m_alt is 0 based
				potentialGenotypes.emplace_back(std::make_tuple(std::to_string(alleleIndex), aCount));
			}
		}

		// sort the genotypes in decending order. Then we just worry about the first 2 indices (if they exist)
		std::sort(potentialGenotypes.begin(), potentialGenotypes.end(), [](const std::tuple< std::string, uint32_t >& a, const std::tuple< std::string, uint32_t >& b)
				  {
					  return std::get< 1 >(a) > std::get< 1 >(b);
				  });
		std::string genotype;
		// based on the size of the genotypes we generate the genotype
		switch (potentialGenotypes.size())
		{
		case 0: // there isn't a decernible genotype
			genotype = "./.";
			break;
		case 1: // if there is only one then we know this is a homozygous
			genotype = std::get< 0 >(potentialGenotypes[0]) + "/" + std::get< 0 >(potentialGenotypes[0]);
			break;
		default: // at least 2 elements we know (since it's sorted) that the 2 highest must be at index 0 and 1 respectively (heterozygous)
			genotype = std::get< 0 >(potentialGenotypes[0]) + "/" + std::get< 0 >(potentialGenotypes[1]);
		}
		return genotype;
	}

	void Variant::printVariant(std::ostream& out)
	{
		calculateAlleleCounts();
		uint32_t totalCount = this->m_total_allele_count + this->m_total_allele_count_low_quality;
		out << this->m_chrom << "\t" << getPosition() << "\t.\t" << alleleString() << "\t0\t.\tDP=" << this->m_total_allele_count << ";DP4=" << getAlleleCountString() << ";TC=" << totalCount << "\tGT\t" << getGenotype() <<  std::endl;
	}

}
