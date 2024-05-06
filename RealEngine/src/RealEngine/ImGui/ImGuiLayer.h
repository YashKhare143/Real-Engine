#pragma once
#include "RealEngine/Layer.h"

namespace RealEngine {

class RE_API ImGuiLayer :public Layer

{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnEvent(Event& event);
private:
	float m_time;
};


}