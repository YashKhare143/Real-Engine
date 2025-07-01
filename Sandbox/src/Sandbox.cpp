#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include "Platform/OpenGl/OpenGLShader.h"
#include "Sandbox2D.h"

	class ExampleLayer : public RealEngine::Layer
	{
	public:
		ExampleLayer() : Layer("Example"), m_CameraController(1280.0f/720.0f)
		{

			m_SquareVA = RealEngine::VertexArray::Create();

			float SquareVertices[5 * 4] = {						// Defining Vertex Data

				////// vertex position ////// 		
					-0.5f,-0.5f, 0.0f,	0.0f, 0.0f,				
					 0.5f,-0.5f, 0.0f,	1.0f, 0.0f,
					 0.5f, 0.5f, 0.0f,	1.0f, 1.0f,
					-0.5f, 0.5f, 0.0f,	0.0f, 1.0f
				////////////////////////////      

			};

			RealEngine::Ref<RealEngine::VertexBuffer> m_SquareVB;
			m_SquareVB = RealEngine::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));

			RealEngine::BufferLayout SquareVBlayout = {
				{RealEngine::ShaderDataType::Float3, "a_Position"},
				{RealEngine::ShaderDataType::Float2, "a_TexCoord"}
			};

			m_SquareVB->SetLayour(SquareVBlayout);
			m_SquareVA->AddVertexBuffer(m_SquareVB);


			uint32_t SquareIndices[6] = { 0, 1, 2, 2, 3, 0 };
			RealEngine::Ref<RealEngine::IndexBuffer> m_SquareIB;
			m_SquareIB = (RealEngine::IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));  //Creating Index Buffer

			m_SquareVA->SetIndexBuffer(m_SquareIB);



		//Creating Square Flat Shader

			auto m_FlatColorShader = m_Shaderlibrary.Load("assets/shaders/FlatShader.glsl");

		///////////////////////

		//Creating Square Texture Shader

			auto m_TextureShader = m_Shaderlibrary.Load("assets/shaders/Texture.glsl");

			m_Texture = RealEngine::Texture2D::Create("assets/textures/Heart.png");
			m_DotTexture = RealEngine::Texture2D::Create("assets/textures/Dot.png");

			std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

		///////////////////////

		}
		void OnUpdate(RealEngine::Timestep ts) override
		{
			//RE_TRACE("DeltaTime: {0}, MilliSeconds: {1}", ts.GetDeltaTime(), ts.GetMilliSeconds());

			m_CameraController.OnUpdate(ts);



			RealEngine::RenderCommand::SetClearColor({ 0.203, 0.176, 0.2 , 1 });
			RealEngine::RenderCommand::Clear();



			RealEngine::Renderer::BeginScene(m_CameraController.GetCamera());

				static const int gridSize = 11;
				static std::map<int, std::vector<int>> columns;
				columns[0] = { 6,7,8 };

				columns[1] = { 5,9 };
				columns[2] = { 4,9 };
				columns[3] = { 3,5,6,9 };

				columns[4] = { 2,5,6,8 };

				columns[5] = { 1,3,7 };

				columns[6] = { 2,5,6,8 };

				columns[7] = { 3,5,6,9 };
				columns[8] = { 4,9 };
				columns[9] = { 5,9 };

				columns[10] = { 6,7,8 };

				if (Spacing <= 0.06f && reset == false) {
					reset = false;
					Spacing += 0.03f * ts;
				}
				else if (Spacing >= 0.04f) {
					reset = true;
					Spacing -= 0.03f * ts;
				}
				else {
					reset = false;
				}

				glm::mat4 HeartScale = glm::scale(glm::mat4(1.0f), glm::vec3(Spacing));
				glm::mat4 FaceScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05));

				auto m_TextureShader	= m_Shaderlibrary.Get("Texture");
				auto m_FlatColorShader = m_Shaderlibrary.Get("FlatShader");

				for (int i = 0; i < gridSize; i++) {
					int j = 0;
					for (int value : columns[i]) {
						if (j = value) {

							glm::vec3 pos(i* 0.055f - 1.7f, j * 0.055f + 0.4f, 0.0f);

							glm::mat4 HeartTransform = glm::translate(glm::mat4(1.0f), pos) * HeartScale;
							glm::mat4 FaceTransform = glm::translate(glm::mat4(1.0f), pos) * FaceScale;

							if ((value == 5 && i == 3) || (value == 6 && i == 3) || (value == 5 && i == 4) || (value == 6 && i == 4) || (value == 5 && i == 6) || (value == 6 && i == 6) || (value == 5 && i == 7) || (value == 6 && i == 7)) {
								std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", EyesColor);
								RealEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, FaceTransform);
							}
							else if ((value == 3 && i == 5)) {
								std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", MouthColor);
								RealEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, FaceTransform);
							}
							else {
								std::dynamic_pointer_cast<RealEngine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

								m_Texture->Bind();
								RealEngine::Renderer::Submit(m_TextureShader, m_SquareVA, HeartTransform);
								
							}

							
						}
					}
					j++;
				}

			//m_Texture->Bind();  // Bind the Texture
			m_DotTexture->Bind();
			RealEngine::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
			
			RealEngine::Renderer::EndScene();

		}

		virtual void  OnImGuiRender() override {

			ImGui::Begin("Settings");
				ImGui::ColorEdit3("Eye Color", glm::value_ptr(EyesColor));
				ImGui::ColorEdit3("Mouth Color", glm::value_ptr(MouthColor));
			ImGui::End();

		}

		void OnEvent(RealEngine::Event& event) override
		{
			m_CameraController.OnEvent(event);
		}

	private:
		RealEngine::Shaderlibrary m_Shaderlibrary;

		RealEngine::Ref<RealEngine::Texture2D> m_Texture;
		RealEngine::Ref<RealEngine::Texture2D> m_DotTexture;

		RealEngine::Ref<RealEngine::VertexArray> m_SquareVA;

		RealEngine::OrthographicCameraController m_CameraController;



		float Spacing = 0.05f;
		bool reset= false;


		glm::vec3 HeartColor = { 0.145f, 0.799f, 0.453f };
		glm::vec3 EyesColor = {0.054f, 0.047f, 0.050f};
		glm::vec3 MouthColor = {0.921, 0.2, 0.290};

	};

class Sandbox : public RealEngine::Application
{
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		//PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}

};


RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}

	