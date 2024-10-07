#include "repch.h"
#include "WindowsWindow.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"


#include "Platform/OpenGl/OpenGLContext.h"
//#include <GLFW/glfw3.h>

namespace RealEngine {

	static bool s_GLFWInitialized = false;
	static int s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error_code, const char* description) {
		RE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		RE_PROFILE_FUNC();
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		RE_PROFILE_FUNC();
		Shutdown();
	}
	
	void WindowsWindow::Init(const WindowProps& props)
	{
		RE_PROFILE_FUNC();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		RE_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			RE_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			RE_ASSERT(success,"Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback); 
		}

		{
			RE_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = new OpenGLContext(m_Window); 
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				default:
					break;
				}
		});


		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
				
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			default:
				break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) 
		{
		
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(xoffset, yoffset);
			data.EventCallback(event);

		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event(xpos, ypos);
				data.EventCallback(event);

		});

	}

	void WindowsWindow::Shutdown()
	{
		RE_PROFILE_FUNC();
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::WindowOnUpdate()
	{
		RE_PROFILE_FUNC();

		glfwPollEvents();

		m_Context->SwapBuffers();

	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		RE_PROFILE_FUNC();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}