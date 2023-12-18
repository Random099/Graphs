#pragma once

#include "Graph.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <utility>

class Interface
{
public:
    Interface() : points(), currentPoint(0) {}
	int run();
private:
    std::vector<ImVec2> points;
    std::vector< std::pair<ImVec2, ImVec2> > edges;
    int currentPoint;

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
    {
        Interface* interface = static_cast<Interface*>(glfwGetWindowUserPointer(window));
        if (interface && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            interface->addPoint(ImVec2(static_cast<float>(x), static_cast<float>(y)));
        }
        else if (interface && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && interface->points.size() > 0)
        {
            if (interface->edges.size() > 0)
                interface->edges.pop_back();
            interface->points.pop_back();
            interface->currentPoint--;
        }
    }

    void addPoint(const ImVec2& point) {
        points.push_back(point);
        currentPoint++;
        if (currentPoint > 1)
            edges.push_back(std::make_pair(points[currentPoint - 2], points[currentPoint - 1]));
    }
};

