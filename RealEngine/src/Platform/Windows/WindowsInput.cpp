#include "repch.h"
#include "WindowsInput.h"
#include "RealEngine/Core/Application.h"
#include "RealEngine/Core/Input.h"
#include <GLFW/glfw3.h>

namespace RealEngine {

	Input* Input::s_Instance = new WindowsInput(); // statically initialize Input class

	bool WindowsInput::IsKeyPressedImpl(int keycode) {

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()); // Casting void window pointer to GLFW window pointer

		auto state = glfwGetKey(window, keycode); // Get Key state (press, not pressed amd hold)

		return state == GLFW_PRESS || state ==  GLFW_REPEAT; // returns true or false
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()); // Casting void window pointer to GLFW window pointer

		auto state = glfwGetMouseButton(window, button); // Get Key state (press, not pressed amd hold)

		return state == GLFW_PRESS ;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl() 
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()); // Casting void window pointer to GLFW window pointer
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos); // Get Key state (press, not pressed amd hold)

		return {(float)xPos ,(float)yPos };
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto [xPos, yPos] = GetMousePositionImpl();
		return xPos;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [xPos, yPos] = GetMousePositionImpl();
		return yPos;
	}
}