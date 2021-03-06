#ifndef GWIZ_IGRAPH_H
#define GWIZ_IGRAPH_H

#include "core/reference/IReference.h"
#include "core/variants/IVariantList.h"
#include "core/variants/IVariant.h"
#include "core/graph/INode.h"

#include <boost/noncopyable.hpp>

#include <list>
#include <string>
#include <memory>

namespace gwiz
{
	class IGraph : private boost::noncopyable
	{
	public:
		typedef std::shared_ptr<IGraph> SharedPtr;

	    IGraph(IReference::SharedPtr referencePtr, IVariantList::SharedPtr variantListPtr) :
		    m_reference_ptr(referencePtr), m_variant_list_ptr(variantListPtr)
		{
		}
		virtual ~IGraph() {}

	protected:

		virtual void constructGraph() = 0;

		IReference::SharedPtr m_reference_ptr;
		IVariantList::SharedPtr m_variant_list_ptr;

		// This is used in conjunction with getCompoundNode.
		// This stores the next variant so we don't have to
		// get the variant twice
		Variant::SharedPtr m_next_variant;
		VariantParser< const char* > m_vcf_parser;
	private:


	};
} // end namespace gwiz

#endif // GWIZ_IGRAPH_H
