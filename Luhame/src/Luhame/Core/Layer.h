#pragma once
#include"Luhame/Core/Core.h"
#include"Luhame/Core/Event/Event.h"
#include"Luhame/Core/TimeStep.h"
namespace Luhame {
	class LUHAME_API layer
	{
	public:
		layer(const std::string& name = "layer");
		virtual ~layer();

		virtual void on_attach() = 0;
		virtual void on_detach() = 0;
		virtual void on_update(const time_step&) = 0;
		virtual void on_event(event& event) = 0;
		virtual void on_ui_render() = 0;

		inline const std::string& get_name() const { return m_debug_name; }
	protected:
		std::string m_debug_name;
	};
}

