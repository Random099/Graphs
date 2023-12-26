#include "Interface.h"

Interface::Interface(){}

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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "GraphDraw", glfwGetPrimaryMonitor(), nullptr);
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
			_graphWindows.push_back(std::make_unique<GraphWindow>("Graph" + std::to_string(_graphWindows.size() + 1)));
		}
		ImGui::End();

		for (auto& graphWindow : _graphWindows)
		{
			graphWindow->draw();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}