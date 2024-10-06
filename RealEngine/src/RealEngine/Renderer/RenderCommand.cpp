#include "repch.h"
#include "RenderCommand.h"
#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace RealEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}