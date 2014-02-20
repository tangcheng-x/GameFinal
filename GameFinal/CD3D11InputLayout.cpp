#include "stdafx.h"
#include "CD3D11InputLayout.h"

CD3D11InputLayout::CD3D11InputLayout(u32 sortCode,
	ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pd3dDeviceContext,
	ID3D10Blob* vertexShaderBuffer,
	const std::vector<SInputLayoutElement>& elements, u32 hashcode)
	:IInputLayout(sortCode, elements, hashcode)
	, md3dDeviceContext(pd3dDeviceContext)
	, m_pd3dInputLayout(nullptr)
{
	HRESULT hr;
	u32 elementCount = elements.size();

	std::vector<D3D11_INPUT_ELEMENT_DESC> elementsDescs(elementCount);
	elementsDescs.resize(elementCount);

	for (u32 i = 0; i < elementCount; i++)
	{
		const SInputLayoutElement& element = elements[i];
		elementsDescs[i].SemanticName = element.SemanticName.c_str();
		elementsDescs[i].SemanticIndex = element.SemanticIndex;
		elementsDescs[i].Format = getDxgiFormat(element.Format);
		elementsDescs[i].InputSlot = element.Slot;
		elementsDescs[i].AlignedByteOffset = element.Offset;
		if (element.InstanceData)
		{
			elementsDescs[i].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementsDescs[i].InstanceDataStepRate = element.InstanceDataStepRate;
		}
		else
		{
			elementsDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementsDescs[i].InstanceDataStepRate = 0;
		}
	}

	hr = pd3dDevice->CreateInputLayout(&elementsDescs[0], elementCount, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_pd3dInputLayout);

	if (FAILED(hr))
	{
		throw std::runtime_error("Create InputLayout Failed!");
	}
}


CD3D11InputLayout::~CD3D11InputLayout()
{
	ReleaseCOM(m_pd3dInputLayout);
}


void CD3D11InputLayout::apply()
{
	return md3dDeviceContext->IASetInputLayout(m_pd3dInputLayout);
}
