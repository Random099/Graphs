#pragma once

#include "Graph.h"
#include "GraphWindow.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <memory>

class Interface
{
public:
    Interface() : activeWindow(0) {}
	int run();
private:
    static void keyboardButtonCallback(GLFWwindow*, int, int, int, int);

    std::vector<GraphWindow> graphWindows;
    int activeWindow;
};

