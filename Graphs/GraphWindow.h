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
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

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
	void minSpanTreeDisplay();
	void minSpanTreeUpdate();
	void minSpanTreeUpdateThreadStart();
	void minSpanTreeTime(const std::string&);
	void randomGraphGen(const uint32_t&);
	void menuDisplay();
	void menuDisplayMST();
	inline void buffersReset();
	inline void graphReset();

	std::string _name;
	Graph _graph;
	std::unique_ptr<std::map<uint32_t, ImVec2> > _points;
	std::unique_ptr<std::map<uint32_t, std::pair<ImVec2, ImVec2> > > _edges;
	std::unique_ptr<std::map<uint32_t, Edge> > _edgeMap;
	std::unique_ptr<std::vector<std::shared_ptr<std::pair<ImVec2, ImVec2> > > > _edgesMST;
	std::shared_ptr<std::map<std::string, double> > _algorithmDurations;
	std::pair<std::shared_ptr<uint32_t>, std::shared_ptr<ImVec2> > _edgeBufferFirst;
	std::pair<std::shared_ptr<uint32_t>, std::shared_ptr<ImVec2> > _edgeBufferSecond;
	std::pair<std::shared_ptr<uint32_t>, std::shared_ptr<ImVec2> > _selectedPoint;
	ImVec2 _windowOffset;
	ImVec2 _mousePos;
	bool _displayingMinSpanTree;
	bool _displayingMinSpanTreeGraph;
	bool _displayingMinSpanTreeTime;
	bool _displayingGraph;
	int _randomGraphEdgeCount;
	mutable std::shared_mutex _mstUpdateMutex;
	std::condition_variable_any _mstUpdateCondition;
};

