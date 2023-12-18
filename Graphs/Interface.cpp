#include "Interface.h"


int Interface::run()
{
	if (!glfwInit())
		return -1;
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "glfw window", glfwGetPrimaryMonitor(), nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetMouseButtonCallback(window, Interface::mouseButtonCallback);
	glfwSetKeyCallback(window, Interface::keyboardButtonCallback);
	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { glfwSetWindowShouldClose(window, GL_FALSE); });
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gl3wInit();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	for (;;) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Menu");
		if (ImGui::Button("Add Graph"))
		{
			windows.push_back(make_tuple("Graph" + std::to_string(windows.size() + 1), ImVec2(0, 0), 0));
		}
		ImGui::End();

		drawGraphs();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void Interface::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Interface* interface = static_cast<Interface*>(glfwGetWindowUserPointer(window));
	if (interface->windows.size() > 0)
	{
		if (interface && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			interface->addPoint(ImVec2(static_cast<float>(x - std::get<1>(interface->windows[interface->activeWindow]).x), static_cast<float>(y - std::get<1>(interface->windows[interface->activeWindow]).y)), std::get<0>(interface->windows[interface->activeWindow]));
		}
		else if (interface && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && interface->points.size() > 0)
		{
			if (interface->edges[std::get<0>(interface->windows[interface->activeWindow])].size() > 0)
				interface->edges[std::get<0>(interface->windows[interface->activeWindow])].pop_back();
			interface->points[std::get<0>(interface->windows[interface->activeWindow])].pop_back();
			std::get<2>(interface->windows[interface->activeWindow])--;
		}
	}
}

void Interface::keyboardButtonCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Interface* interface = static_cast<Interface*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		if (interface->activeWindow < interface->windows.size() - 1)
			interface->activeWindow++;
		else
			interface->activeWindow = 0;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		if (interface->activeWindow > 0)
			interface->activeWindow--;
		else
			interface->activeWindow = interface->windows.size() - 1;
	}
	//std::cout << interface->windowNames[interface->activeWindow] << '\n';
}

void Interface::addPoint(const ImVec2& point, const std::string& name) {
	points[name].push_back(point);
	int currentPoint = std::get<2>(windows[activeWindow]);
	++currentPoint;
	std::get<2>(windows[activeWindow]) = currentPoint;
	if (currentPoint > 1)
		edges[name].push_back(std::make_pair(points[name][currentPoint - 2], points[name][currentPoint - 1]));
}

void Interface::drawGraphs()
{
	for (auto& [name, pos, point] : windows)
	{
		ImGui::Begin(name.c_str());

		ImU32 color = IM_COL32(255, 255, 255, 255); // White color
		float thickness = 2.0f;
		pos = ImGui::GetCursorScreenPos();
		for (std::pair<ImVec2, ImVec2>& edge : this->edges[name])
		{
			ImVec2 vertex1 = ImVec2(edge.first.x + pos.x, edge.first.y + pos.y);
			ImVec2 vertex2 = ImVec2(edge.second.x + pos.x, edge.second.y + pos.y);
			ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, color, thickness);
		}

		ImGui::End();
	}
}