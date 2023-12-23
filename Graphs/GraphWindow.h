#pragma once

#include "Graph.h"
#include "extras.h"

#include "imgui.h"
#include "imgui_stdlib.h"

#include <utility>
#include <memory>
#include <string>
#include <cmath>
#include <map>

class GraphWindow
{
public:
	GraphWindow(const std::string&);
	void draw();
	void handlePoints();
	const std::string& nameGet() const;
	const ImVec2& mousePosGet() const;
	const std::unique_ptr<std::map<uint32_t, std::pair<ImVec2, ImVec2> > >& edgesGet() const;
	const std::unique_ptr<std::map<uint32_t, ImVec2> >& pointsGet() const;
private:
	void pointAdd(const ImVec2&);
	bool pointSelect(const ImVec2&);
	bool edgeSelect(const ImVec2&);
	inline void buffersReset();
	void displayMinSpanTree();

	std::string _name;
	Graph _graph;
	std::unique_ptr<std::map<uint32_t, std::pair<ImVec2, ImVec2> > > _edges;
	std::unique_ptr<std::map<uint32_t, ImVec2> > _points;
	std::unique_ptr<std::map<uint32_t, Edge> > _edgeMap;
	std::pair<std::shared_ptr<uint32_t>, std::shared_ptr<ImVec2> > _edgeBufferFirst;
	std::pair<std::shared_ptr<uint32_t>, std::shared_ptr<ImVec2> > _edgeBufferSecond;
	std::pair<std::shared_ptr<uint32_t>, std::shared_ptr<ImVec2> > _selectedPoint;
	ImVec2 _windowOffset;
	ImVec2 _mousePos;
	bool _displayingMST;
};

