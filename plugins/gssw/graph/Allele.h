#ifndef GWIZ_GSSW_ALLELE_H
#define GWIZ_GSSW_ALLELE_H

#include "core/variant/IAllele.h"

namespace gwiz
{
namespace gssw
{
	class Allele : public IAllele
	{
	public:
		typedef shared_ptr< Allele > SharedPtr;
	    Allele(Sequence::SharedPtr sequencePtr) :
		    IAllele(sequencePtr), m_prefix(0), m_suffix(0) { }
		~Allele() {}

	private:
		uint32_t m_prefix;
		uint32_t m_suffix;
	};
}
}

#endif //GWIZ_GSSW_ALLELE_H
