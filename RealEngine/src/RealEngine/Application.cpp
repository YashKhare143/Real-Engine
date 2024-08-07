#include "repch.h"
#include "Application.h"
#include "RealEngine/Log.h"

#include <glad/glad.h>
#include "Input.h"

namespace RealEngine {

#define BIND_EVENT_FN(f) std::bind(&Application::f, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	

	Application::Application() {
		RE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());  //Creating Vertex Array

		float vertices[3 * 7] = {						// Defining Vertex Data

			////// vertex position ////// 		////// vertex Color //////	
				-0.5f,-0.5f,0.0f,					1.0f, 0.0f, 0.0f, 1.0f,
				0.5f,-0.5f,0.0f,					0.0f, 1.0f, 0.0f, 1.0f,
				0.0f,0.5f,0.0f,						0.0f, 0.0f, 1.0f, 1.0f
			////////////////////////////      ///////////////////////////////

		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //Creating Vertex Buffer

		
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color", true}
		};

		m_VertexBuffer->SetLayour(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		
		uint32_t indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));  //Creating Index Buffer
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		//Creating Shader

		std::string vertexSrd = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
	
		)";

		std::string fragmentSrd = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
	
		)";

		m_Shader.reset(new Shader(vertexSrd, fragmentSrd));

	};
	Application::~Application() {};
	
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
		
	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//RE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}

	};

	void Application::Run() {
		
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->WindowOnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{

		m_Running = false;
		return true;

	}

}