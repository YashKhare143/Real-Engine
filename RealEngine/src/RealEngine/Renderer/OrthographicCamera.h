#pragma once
#include "glm/glm.hpp"

namespace RealEngine {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		glm::vec3 GetPosition() { return m_Position; };
		void SetPosition(const glm::vec3 position) { m_Position = position; ReCalculateViewMatrix(); };

		float GetRotation() { return m_Rotation; };
		void SetRotation(float rotation) { m_Rotation = rotation; ReCalculateViewMatrix(); };

		const glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; };
		const glm::mat4 GetViewMatrix() { return m_ViewMatrix; };
		const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };

	private:
		void ReCalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = {0.0f,0.0f ,0.0f };
		float m_Rotation = 0.0f;

	};

}
