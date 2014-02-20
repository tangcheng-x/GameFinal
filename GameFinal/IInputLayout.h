#ifndef __IINPUTLAYOUT_H__
#define __IINPUTLAYOUT_H__

#include "gfTypes.h"
#include "IReferenceCounted.h"
#include "gfGIFormat.h"
#include <vector>

class IInputLayoutManager;


struct SInputLayoutElement
{
	std::string		SemanticName;
	u32				SemanticIndex;
	E_GI_FORMAT		Format;
	u32				Slot;
	u32				Offset;
	bool			InstanceData;
	u32				InstanceDataStepRate;

	SInputLayoutElement()
	{
		SemanticIndex = 0;
		Slot = 0;
		Offset = 0;
		InstanceData = false;
		InstanceDataStepRate = 0;
	}

	bool operator==(const SInputLayoutElement& e) const
	{
		if (SemanticName != e.SemanticName ||
			SemanticIndex != e.SemanticIndex ||
			Format != e.Format ||
			Slot != e.Slot ||
			Offset != e.Offset || 
			InstanceData != e.InstanceData ||
			InstanceDataStepRate != e.InstanceDataStepRate)
		{
			return false;
		}
		
		return true;
	}

	bool operator!=(const SInputLayoutElement& e) const
	{
		return !((*this) == e);
	}

	u32 hashCode() const
	{
		u32 h = 0;
		u32 i = 0;
		const char* s = SemanticName.c_str();
		while (s[i] != 0)
		{
			h = h * 17 + s[i++];
		}

		h = h * 31 + SemanticIndex;
		h = h * 31 + Format;
		h = h * 31 + Slot;
		h = h * 31 + Offset;
	
		return h;
	}

	u32 getSize() const
	{
		switch (Format)
		{
		case EGF_R32G32B32A32_FLOAT: 
			return 128;
		case EGF_R32G32B32_FLOAT:
			return 96;
		case EGF_R32G32_FLOAT:
			return 64;
		}
		return 0;
	}
};

class IInputLayout : public IReferenceCounted
{
public:
	IInputLayout(u32 sortCode, 
		const std::vector<SInputLayoutElement>& elements, u32 hashcode)
		: mSortCode(sortCode)
		, m_layoutElements(elements)
		, m_hashcode(hashcode)
		, m_vertexSize(0)
	{
		m_next = nullptr;
		for (u32 i = 0; i < elements.size(); i++)
			m_vertexSize += elements[i].getSize();
	}

	bool equals(const std::vector<SInputLayoutElement>& elements) const
	{
		if (elements.size() != m_layoutElements.size())
			return false;

		u32 elementCount = m_layoutElements.size();

		for (u32 i = 0; i < elementCount; i++)
		{
			if (m_layoutElements[i] != elements[i])
				return false;
		}
		return true;

	}

	u32 getHashCode() const
	{
		return m_hashcode;
	}

	u32 getVertexSize() const
	{
		return m_vertexSize;
	}

	u32 getSortCode() const
	{
		return mSortCode;
	}

	virtual void apply() = 0;

private:
	const u32							m_hashcode;
	const u32							mSortCode;

	std::vector<SInputLayoutElement>	m_layoutElements;
	u32									m_vertexSize;
	IInputLayout*						m_next;
};

#endif
