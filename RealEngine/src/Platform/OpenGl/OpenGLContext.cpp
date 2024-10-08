#include "repch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace RealEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):	m_WindowHandle(windowHandle)
	{
		RE_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}
	void OpenGLContext::Init()
	{
		RE_PROFILE_FUNC();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RE_CORE_ASSERT(status, "Failed to initialize Glad!");

		RE_CORE_INFO("OpenGl Renderer: ");
		RE_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		RE_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		RE_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));

		#ifdef RE_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				RE_CORE_ASSERT(versionMajor > 4 || versionMajor == 4 && versionMinor >= 5), "RealEngine requires at least OpenGL version 4.6!");
		#endif // RE_ENABLE_ASSERTS

	}
	void OpenGLContext::SwapBuffers()
	{
		RE_PROFILE_FUNC();

		glfwSwapBuffers(m_WindowHandle);
	}
}
