#pragma once
#include "RealEngine/Layer.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/MouseEvent.h"
#include "RealEngine/Events/KeyEvent.h"

namespace RealEngine {

class RE_API ImGuiLayer :public Layer

{
public:
	ImGuiLayer();
	~ImGuiLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	void Begin();
	void End();

private:
	float m_time;

};


}