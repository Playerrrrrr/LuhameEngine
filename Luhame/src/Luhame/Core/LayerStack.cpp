#include "pch.h"
#include "LayerStack.h"

namespace Luhame {
	layer_stack::layer_stack()
	{
		m_layerInsert = m_layers.begin();
	}

	layer_stack::~layer_stack()
	{
		for (layer* layer : m_layers)
			delete layer;
	}

	void layer_stack::push_layer(layer* layer)//pushµΩÕ∑…œ
	{
		m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	}

	void layer_stack::push_over_layer(layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void layer_stack::pop_layer(layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsert--;
		}

	}

	void layer_stack::pop_over_layer(layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
			m_layers.erase(it);
	}

}