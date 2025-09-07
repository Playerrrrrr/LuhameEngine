#pragma once
#include"Luhame/Core/Core.h"
#include"Luhame/Core/Event/Event.h"
namespace Luhame {
	class LUHAME_API layer
	{
	public:
		layer(const std::string& name = "layer");
		virtual ~layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(event& event) {}

		inline const std::string& GetName() const { return m_debug_name; }
	protected:
		std::string m_debug_name;
	};
}

