#pragma once

#include "Graph.h"
#include "extras.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include <utility>
#include <memory>
#include <string>
#include <cmath>

class GraphWindow
{
public:
	GraphWindow(const std::string&);
	void pointAdd(const ImVec2&);
	bool pointSelect(const ImVec2& mousePos);
	void draw();
	void handlePoints();
	const std::string& nameGet() const;
	ImVec2& mousePosGet();
	std::unique_ptr<std::vector<std::pair<ImVec2, ImVec2> > >& edgesGet();
	std::unique_ptr<std::vector<ImVec2> >& pointsGet();
private:
	std::string _name;
	Graph _graph;
	std::unique_ptr<std::vector<std::pair<ImVec2, ImVec2> > > _edges;
	std::unique_ptr<std::vector<ImVec2> > _points;
	std::pair<std::shared_ptr<ImVec2>, std::shared_ptr<ImVec2> > _edgeBuffer;
	uint32_t _currentVertex;
	ImVec2 _mousePos;
	std::shared_ptr<ImVec2> _selectedPoint;
};

