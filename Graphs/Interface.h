#pragma once

#include "Graph.h"

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
    static void mouseButtonCallback(GLFWwindow*, int, int, int);
    static void keyboardButtonCallback(GLFWwindow*, int, int, int, int);
    void addPoint(const ImVec2&, const std::string&);
    void drawGraphs();

    //std::map<std::string, std::vector<ImVec2> > points;
    std::map<std::string, std::vector<std::pair<ImVec2, ImVec2> > > edges;
    std::map<std::string, std::vector<ImVec2> > points;
    //std::vector<std::pair<ImVec2, ImVec2> >  edges;
    std::vector<std::tuple<std::string, ImVec2, int> > windows;
    int activeWindow;
    //ImVec2 cursorPos;
};

