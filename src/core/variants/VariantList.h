#ifndef GWIZ_VARIANTLIST_H
#define GWIZ_VARIANTLIST_H

#include "IVariantList.h"

#include <memory>
#include <vector>
#include <algorithm>

namespace gwiz
{
	class VariantList : public IVariantList
	{
	public:
		typedef std::shared_ptr< VariantList > SharedPtr;
	    VariantList() : m_current_index(0) {}
		virtual ~VariantList() {}

		inline bool getNextVariant(Variant::SharedPtr& variantPtr) override
		{
			bool hasVariants = this->m_current_index < this->m_variants_ptr_list.size();
			if (hasVariants)
			{
				variantPtr = this->m_variants_ptr_list[this->m_current_index];
				++this->m_current_index;
			}
			return hasVariants;
		}

		inline void addVariant(const Variant::SharedPtr variant)
		{
			this->m_variants_ptr_list.emplace_back(variant);
		}

		inline void rewind()
		{
			m_current_index = 0;
		}

		IVariantList::SharedPtr getVariantsInRegion(Region::SharedPtr regionPtr) override
		{
			position startPosition = regionPtr->getStartPosition();
			position endPosition = regionPtr->getEndPosition();
			std::pair< std::vector< Variant::SharedPtr >::iterator, std::vector< Variant::SharedPtr >::iterator > bounds;
			bounds = std::equal_range(this->m_variants_ptr_list.begin(), this->m_variants_ptr_list.end(), nullptr, [startPosition, endPosition](const Variant::SharedPtr& variantPtr, const Variant::SharedPtr& ignore) {
					return startPosition <= variantPtr->getPosition() && variantPtr->getPosition() <= endPosition;
				});
			auto variantListPtr = std::make_shared< VariantList >();
			variantListPtr->m_variants_ptr_list = std::vector< Variant::SharedPtr >(bounds.first, bounds.second);
			return variantListPtr;
		}

		size_t getCount() override { return m_variants_ptr_list.size(); }

	protected:
		size_t m_current_index;
		std::vector< Variant::SharedPtr > m_variants_ptr_list;
	};
}

#endif //GWIZ_VARIANTLIST_H
