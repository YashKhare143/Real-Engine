#pragma once
#include "RealEngine/Events/Event.h"
#include "RealEngine/Core/Core.h"
#include "RealEngine/Core/Timestep.h"


namespace RealEngine {

	class RE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		//Only for debug purpose
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}