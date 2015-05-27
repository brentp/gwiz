#ifndef GWIZ_IVARIANT_H
#define GWIZ_IVARIANT_H

#include "core/allele/IAllele.h"

#include <boost/noncopyable.hpp>

#include <memory>

namespace gwiz
{
    class IVariant : private boost::noncopyable
    {
        public:
            typedef std::shared_ptr<IVariant> SharedPtr;
            IVariant()
            {
                static uint32_t idCounter = 0;
                this->m_variant_id = ++idCounter;
            }
            virtual ~IVariant() {}

			/* static IVariant::SharedPtr */

            virtual size_t getSmallestAlleleSize() = 0;
            virtual size_t getLargestAlleleSize() = 0;
            uint32_t getVariantID() { return this->m_variant_id; }

			virtual void printVariant(std::ostream& out) = 0;

			IAllele::SharedPtr getReferenceAllelePtr() { return this->m_ref_allele_ptr; }
			std::vector< IAllele::SharedPtr > getAltAllelePtrs() { return this->m_alt_allele_ptrs; }
        private:
			IAllele::SharedPtr m_ref_allele_ptr;
			std::vector< IAllele::SharedPtr > m_alt_allele_ptrs;
            uint32_t m_variant_id;
    };
}

#endif// GWIZ_IVARIANT_H
