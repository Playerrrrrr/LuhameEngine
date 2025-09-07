#pragma once
#pragma once

#include "Luhame/Core/Core.h"
#include "layer.h"

#include <vector>

namespace Luhame {

	class LUHAME_API layer_stack
	{
	public:
		layer_stack();
		~layer_stack();

		void Pushlayer(layer* layer);
		void PushOverlay(layer* overlay);
		void Poplayer(layer* layer);
		void PopOverlay(layer* overlay);

		std::vector<layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<layer*>::iterator end() { return m_layers.end(); }
	private:
		std::vector<layer*> m_layers;
		std::vector<layer*>::iterator m_layerInsert;
	};

}

