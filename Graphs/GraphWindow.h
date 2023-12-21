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
	void draw();
	void handlePoints();
	const std::string& nameGet() const;
	const ImVec2& mousePosGet() const;
	const std::unique_ptr<std::vector<std::pair<ImVec2, ImVec2> > >& edgesGet() const;
	const std::unique_ptr<std::vector<ImVec2> >& pointsGet() const;
private:
	void pointAdd(const ImVec2&);
	bool pointSelect(const ImVec2&);
	bool edgeSelect(const ImVec2&);

	std::string _name;
	Graph _graph;
	std::unique_ptr<std::vector<std::pair<ImVec2, ImVec2> > > _edges;
	std::unique_ptr<std::vector<ImVec2> > _points;
	std::pair<std::shared_ptr<ImVec2>, std::shared_ptr<ImVec2> > _edgeBuffer;
	uint32_t _currentVertex;
	ImVec2 _windowOffset;
	ImVec2 _mousePos;
	std::shared_ptr<ImVec2> _selectedPoint;
};

