#include "Interface.h"

Interface::Interface() :
	_activeWindow{ 0 }
{}

Interface::~Interface()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

int Interface::run()
{
	if (!glfwInit())
		return -1;
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "GraphDraw", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, Interface::keyboardButtonCallback);
	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { glfwSetWindowShouldClose(window, GLFW_FALSE); });
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gl3wInit();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	for (;;) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Menu");
		if (ImGui::Button("Add Graph"))
		{
			_graphWindows.push_back(GraphWindow("Graph" + std::to_string(_graphWindows.size() + 1)));
		}
		ImGui::End();

		if (_graphWindows.size() > 0)
		{
			_graphWindows[_activeWindow].handlePoints();
		}

		for (auto& graphWindow : _graphWindows)
		{
			graphWindow.draw();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void Interface::keyboardButtonCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Interface* interface{ static_cast<Interface*>(glfwGetWindowUserPointer(window)) };
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		if (interface->_activeWindow < interface->_graphWindows.size() - 1)
			interface->_activeWindow++;
		else
			interface->_activeWindow = 0;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		if (interface->_activeWindow > 0)
			interface->_activeWindow--;
		else
			interface->_activeWindow = static_cast<int>(interface->_graphWindows.size()) - 1;
	}	
}
