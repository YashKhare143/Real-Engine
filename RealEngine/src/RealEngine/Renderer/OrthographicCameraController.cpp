#include "repch.h"
#include "RealEngine/Core/Input.h"
#include "RealEngine/Core/KeyCodes.h"
#include "RealEngine/Renderer/OrthographicCameraController.h"

namespace RealEngine {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		RE_PROFILE_FUNC();


		if (Input::IsKeyPressed(RE_KEY_W) || Input::IsKeyPressed(RE_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(RE_KEY_S) || Input::IsKeyPressed(RE_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(RE_KEY_A) || Input::IsKeyPressed(RE_KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(RE_KEY_D) || Input::IsKeyPressed(RE_KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		if (m_Rotation) {

			if (Input::IsKeyPressed(RE_KEY_Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(RE_KEY_E))
			{
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}	

			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;	
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		RE_PROFILE_FUNC();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));

		dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		RE_PROFILE_FUNC();

		m_ZoomLevel -= e.GetYOffset() * 0.5;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		RE_PROFILE_FUNC();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}