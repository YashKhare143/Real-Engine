#pragma once
#include "RealEngine/Renderer/OrthographicCamera.h"

#include "RealEngine/Core/Timestep.h"

#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/MouseEvent.h"

namespace RealEngine {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; };
		const OrthographicCamera& GetCamera() const { return m_Camera; };

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
		 
	};
}