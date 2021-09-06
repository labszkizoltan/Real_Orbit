
#include "TestLayer.h"
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>


#define BIND_EVENT_FN(x) std::bind(&TestLayer::x, this, std::placeholders::_1)



// vertex shader:
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0f);\n"
"   ourColor = aColor;\n"
"}\0";

// fragment shader
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\0";




TestLayer::TestLayer()
	: Layer("TestLayer")
{
	LOG_INFO("TestLayer constructed");
}

void TestLayer::OnAttach()
{
	LOG_INFO("TestLayer attached");

	// Hello Triangle //

	// Vertices with their own color:
	float vertices[] = {
		// position			// color
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 0.7f, 1.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2,   // first triangle
	2, 1, 3,    // second triangle
	0, 2, 3,
	1, 0, 3
	};

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return;
	}

	LOG_INFO("Querying OpenGL errors: {0}", glGetError());
	LOG_INFO("Querying OpenGL errors: {0}", glGetError());

	// create the shaders
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	LOG_INFO("Querying OpenGL errors: {0}", glGetError());
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	LOG_INFO("Querying OpenGL errors: {0}", glGetError());
	glCompileShader(vertexShader);
	LOG_INFO("Querying OpenGL errors: {0}", glGetError());

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	m_ShaderProgram = glCreateProgram();

	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
	}

	glUseProgram(m_ShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// rest of the opengl code
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// copy vertex data
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// copy index data
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// set the vertex attribute pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//---------------------------------------------//

}

void TestLayer::OnDetach()
{
	LOG_INFO("TestLayer detached");
}

void TestLayer::OnUpdate(Timestep ts)
{
//	LOG_INFO("TestLayer updated");

	if (Input::IsKeyPressed(sf::Keyboard::Key::Left)) { LOG_INFO("TestLayer OnUpdate: left arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Right)) { LOG_INFO("TestLayer OnUpdate: right arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Up)) { LOG_INFO("TestLayer OnUpdate: Up arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Down)) { LOG_INFO("TestLayer OnUpdate: down arrow is pressed"); }

	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Left)) { LOG_INFO("TestLayer OnUpdate: Left mouse button is pressed"); }
	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Right)) { LOG_INFO("TestLayer OnUpdate: Right mouse button is pressed"); }
	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Middle))
	{
		LOG_INFO("TestLayer OnUpdate: Middle mouse button is pressed");
		sf::Vector2i mouse_pos = Input::GetMousePosition();
		LOG_INFO("TestLayer OnUpdate: window mouse position: x: {0}, y: {1}", mouse_pos.x, mouse_pos.y);
		sf::Vector2i global_mouse_pos = Input::GetGlobalMousePosition();
		LOG_INFO("TestLayer OnUpdate: global mouse position: x: {0}, y: {1}", global_mouse_pos.x, global_mouse_pos.y);
	}


	// Render
	// Clear color buffer
	glClearColor(0.9f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ShaderProgram);
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

}

void TestLayer::OnEvent(Event& event)
{
	LOG_INFO("TestLayer event received");

	event.Dispatch<sf::Event::EventType::LostFocus>				(BIND_EVENT_FN(OnLoosingFocus));
	event.Dispatch<sf::Event::EventType::GainedFocus>			(BIND_EVENT_FN(OnGainingFocus));
	event.Dispatch<sf::Event::EventType::KeyPressed>			(BIND_EVENT_FN(OnKeyPressed));
	event.Dispatch<sf::Event::EventType::KeyReleased>			(BIND_EVENT_FN(OnKeyReleased));
	event.Dispatch<sf::Event::EventType::MouseWheelScrolled>	(BIND_EVENT_FN(MouseWheelScrolled));
	event.Dispatch<sf::Event::EventType::MouseButtonPressed>	(BIND_EVENT_FN(OnMouseButtonPressed));
	event.Dispatch<sf::Event::EventType::MouseButtonReleased>	(BIND_EVENT_FN(OnMouseButtonReleased));
}

/***********************************
***** Private member functions *****
************************************/

bool TestLayer::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer received LostFocus event");
	return false;
}

bool TestLayer::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer received GainFocus event");
	return false;
}

bool TestLayer::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer received OnTextEntered event");
	return false;
}

bool TestLayer::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return true; // setting the flag to handled, so the subsequent layers wont process this event when they receive it
}

bool TestLayer::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer received OnKeyReleased event");
	return false;
}

bool TestLayer::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer received OnMouseButtonPressed event");
	return false;
}

bool TestLayer::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer received OnMouseButtonReleased event");
	return false;
}

bool TestLayer::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer received OnMouseMoved event");
	return false;
}

bool TestLayer::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer received OnMouseEntered event");
	return false;
}

bool TestLayer::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer received OnMouseLeft event");
	return false;
}
