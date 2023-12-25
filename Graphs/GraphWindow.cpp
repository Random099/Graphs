#include "GraphWindow.h"

GraphWindow::GraphWindow(const std::string& name) : 
	_name{ name },
	_graph{ Graph() },
	_points{ std::make_unique<std::map<uint32_t, ImVec2> >() },
	_edges{ std::make_unique<std::map<uint32_t, std::pair<ImVec2, ImVec2> > >() },
	_edgeMap{ std::make_unique<std::map<uint32_t, Edge> >() },
	_edgesMST{ std::make_unique<std::vector<std::shared_ptr<std::pair<ImVec2, ImVec2> > > >() },
	_algorithmDurations{ std::shared_ptr<std::map<std::string, double> >{ nullptr } },
	_edgeBufferFirst{ std::make_pair(std::shared_ptr<uint32_t>{ nullptr }, std::shared_ptr<ImVec2>{ nullptr }) },
	_edgeBufferSecond{ std::make_pair(std::shared_ptr<uint32_t>{ nullptr }, std::shared_ptr<ImVec2>{ nullptr }) },
	_selectedPoint{ std::make_pair(std::shared_ptr<uint32_t>{ nullptr }, std::shared_ptr<ImVec2>{ nullptr }) },
	_windowOffset{ ImVec2{ 0, 0 } },
	_mousePos{ ImVec2{ 0, 0 } },
	_displayingMinSpanTree{ false },
	_displayingMinSpanTreeTime{ false },
	_randomGraphEdgeCount{ 0 }
{}

void GraphWindow::pointAdd(const ImVec2& point)
{
	if (_edgeBufferFirst.second == nullptr)
	{
		_edgeBufferFirst.second = std::make_shared<ImVec2>(point);
		if (_selectedPoint.first == nullptr)
		{
			_edgeBufferFirst.first = std::make_shared<uint32_t>(static_cast<uint32_t>(_points->size()));
			(*_points)[*_edgeBufferFirst.first] = *_edgeBufferFirst.second;
			this->_graph.vertexAdd();
			this->buffersReset();
		}
	}
	else if (_edgeBufferSecond.second == nullptr)
	{
		bool vertexConnectionExists{ false };
		_edgeBufferSecond.second = std::make_shared<ImVec2>(point);
		(*_points)[*_edgeBufferSecond.first] = *_edgeBufferSecond.second;
		auto edgesPtr{ _graph.edgeSetGet() };
		vertexConnectionExists = std::find_if(edgesPtr->begin(), edgesPtr->end(),
			[&](const Edge& edge) -> bool
			{
				return (edge.verticesGet().first == *_edgeBufferFirst.first && edge.verticesGet().second == *_edgeBufferSecond.first)
					|| (edge.verticesGet().first == *_edgeBufferSecond.first && edge.verticesGet().second == *_edgeBufferFirst.first);
			}
		) != std::end(*edgesPtr);
		if (!vertexConnectionExists)
		{
			int32_t weight = helper::Distance<int32_t>(*_edgeBufferFirst.second, *_edgeBufferSecond.second);
			_graph.edgeAdd(Edge{ *_edgeBufferFirst.first, *_edgeBufferSecond.first, weight });
			(*_edges)[static_cast<uint32_t>(_edges->size())] = std::make_pair(*_edgeBufferFirst.second, *_edgeBufferSecond.second);
			(*_edgeMap)[static_cast<uint32_t>(_edges->size() - 1)] = Edge{*_edgeBufferFirst.first, *_edgeBufferSecond.first, weight};
			if (_displayingMinSpanTree || _edgesMST->size() != 0) 
				this->minSpanTreeUpdate();
		}
		this->buffersReset();
	}
}

void GraphWindow::draw()
{
	ImGui::Begin(_name.c_str());
	ImGui::SetWindowSize(constant::DEFAULT_GRAPH_WINDOW_SIZE);
	_windowOffset = ImGui::GetCursorScreenPos();

	if(ImGui::IsWindowHovered())
		this->handlePoints();

	ImGui::Checkbox("Display MST", &_displayingMinSpanTree);
	ImGui::SameLine();
	if (ImGui::Button("Generate random graph") && _randomGraphEdgeCount > 0)
	{
		this->randomGraphGen(_randomGraphEdgeCount);
	}
	ImGui::SameLine();
	ImGui::InputInt("Edge count", &_randomGraphEdgeCount);

	if (_displayingMinSpanTree)
	{
		this->minSpanTreeDisplay();
	}
	for (const auto& [n, edge] : *_edges)
	{
		ImVec2 vertex1{ edge.first.x + _windowOffset.x, edge.first.y + _windowOffset.y };
		ImVec2 vertex2{ edge.second.x + _windowOffset.x, edge.second.y + _windowOffset.y };
		ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, ImGuiColors::WHITE, constant::LINE_THICKNESS);

		int32_t weight{ helper::Distance<int32_t>(vertex1, vertex2) };
		ImGui::GetWindowDrawList()->AddText(helper::MiddlePoint(vertex1, vertex2), ImGuiColors::GREEN, std::to_string(weight).c_str());
	}
	
	for (const auto& [n, point] : *_points)
	{
		ImVec2 vertex{ point.x + _windowOffset.x, point.y + _windowOffset.y };
		ImGui::GetWindowDrawList()->AddCircleFilled(vertex, constant::POINT_RADIUS, ImGuiColors::WHITE);
		ImGui::GetWindowDrawList()->AddText(ImVec2{ vertex.x - 3.5f, vertex.y - 20.0f }, ImGuiColors::YELLOW, std::to_string(n).c_str());
	}

	if (_selectedPoint.first != nullptr)
	{
		ImGui::GetWindowDrawList()->AddLine(ImVec2{ _selectedPoint.second->x + _windowOffset.x, _selectedPoint.second->y + _windowOffset.y }, _mousePos, ImGuiColors::WHITE_TRANSPARENT, 2.0f);
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2{ _selectedPoint.second->x + _windowOffset.x, _selectedPoint.second->y + _windowOffset.y }, constant::POINT_RADIUS, ImGuiColors::RED);
	}
	ImGui::End();
}

void GraphWindow::handlePoints()
{
	ImGuiIO& io{ ImGui::GetIO() };
	_mousePos = io.MousePos;
	
	if ( (_mousePos.y - _windowOffset.y) > 40.0f)
	{
		if (io.MouseClicked[0] && _selectedPoint.second == nullptr)
		{
			if (!this->pointSelect(_mousePos))
				this->pointAdd(ImVec2{ _mousePos.x - _windowOffset.x, _mousePos.y - _windowOffset.y });
			else
			{
				_edgeBufferFirst.second = _selectedPoint.second;
				_edgeBufferFirst.first = _selectedPoint.first;
			}
		}
		else if (io.MouseClicked[0] && _selectedPoint.second != nullptr)
		{
			if (this->pointSelect(_mousePos))
			{	
				_edgeBufferSecond.first = _selectedPoint.first;
				this->pointAdd(ImVec2{ *_selectedPoint.second });
			}
			else
			{
				_edgeBufferSecond.first = std::make_shared<uint32_t>(static_cast<uint32_t>(_points->size()));
				this->pointAdd(ImVec2{ _mousePos.x - _windowOffset.x, _mousePos.y - _windowOffset.y });
			}
		}
		else if (io.MouseClicked[1])
		{
			this->buffersReset();
			uint32_t edgeToRemoveId = (this->_edges->size() > 0) ? (--this->_edges->end())->first : 0;
			bool removeVertex = _points->empty() ? false : _graph.data()[std::distance(this->_points->begin(), --this->_points->end())].size() == 0;
			if (this->_points->size() > 0 && removeVertex)
			{
				this->_points->erase(--this->_points->end());
				this->_graph.vertexRemove(static_cast<uint32_t>(this->_points->size()));
				if (this->_edges->size() > 0)
				{
					this->_edges->erase(edgeToRemoveId);
					this->_graph.edgeRemove((*_edgeMap)[edgeToRemoveId]);
					this->_edgeMap->erase(edgeToRemoveId);
				}
			}
			else if (this->_edges->size() > 0)
			{
				this->_edges->erase(edgeToRemoveId);
				this->_graph.edgeRemove((*_edgeMap)[edgeToRemoveId]);
				this->_edgeMap->erase(edgeToRemoveId);
			}
			if (_displayingMinSpanTree)
				this->minSpanTreeUpdate();
		}
	}
}

bool GraphWindow::pointSelect(const ImVec2& mousePos)
{
	_selectedPoint.first = nullptr;
	_selectedPoint.second = nullptr;
	for (auto& [n, point] : *_points)
	{
		uint32_t distance{ helper::Distance<uint32_t>(ImVec2{ _windowOffset.x + point.x, _windowOffset.y + point.y }, mousePos) };
		if (distance < constant::POINT_SELECTOR_RADIUS)
		{
			_selectedPoint.second = std::make_shared<ImVec2>(point);
			_selectedPoint.first = std::make_shared<uint32_t>(n);
			return true;
		}
	}
	return false;
}

bool GraphWindow::edgeSelect(const ImVec2& mousePos) //TODO
{
	for (uint32_t i = 0; i < _points->size(); ++i)
	{
		uint32_t distance{ helper::Distance<uint32_t>(ImVec2{ _windowOffset.x + (*_points)[i].x, _windowOffset.y + (*_points)[i].y}, mousePos) };
		if (distance < constant::POINT_SELECTOR_RADIUS)
		{
			_selectedPoint.second = std::make_shared<ImVec2>((*_points)[i]);
			*_selectedPoint.first = i;
			return true;
		}
	}
	return false;
}

void GraphWindow::minSpanTreeDisplay()
{
	ImGui::Begin((_name + " MST").c_str());
	ImGui::SetWindowSize(constant::DEFAULT_GRAPH_WINDOW_SIZE);
	ImVec2 windowOffsetMST{ ImGui::GetCursorScreenPos() };
	
	if (_edgesMST->size() == 0)
	{
		this->minSpanTreeUpdate();
	}
	if (_edges->size() > 0)
	{
		ImGui::Checkbox("Display duration", &_displayingMinSpanTreeTime);

		if (_displayingMinSpanTreeTime)
		{
			ImGui::SameLine();
			if (ImGui::Button("Recalculate durations"))
			{
				this->minSpanTreeTime("Kruskal");
			}
			if (_algorithmDurations != nullptr)
			{
				for (const auto& [algorithmName, duration] : *_algorithmDurations)
				{
					ImGui::Text((algorithmName + ": %fs").c_str(), duration);
				}
			}
		}

		for (std::shared_ptr<std::pair<ImVec2, ImVec2> > edge : *_edgesMST)
		{
			ImVec2 vertex1{ edge->first.x + windowOffsetMST.x, edge->first.y + windowOffsetMST.y };
			ImVec2 vertex2{ edge->second.x + windowOffsetMST.x, edge->second.y + windowOffsetMST.y };
			ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, ImGuiColors::GREEN, constant::LINE_THICKNESS);
		}
		for (const auto& [n, point] : *_points)
		{
			ImVec2 vertex{ point.x + windowOffsetMST.x, point.y + windowOffsetMST.y };
			ImGui::GetWindowDrawList()->AddCircleFilled(vertex, constant::POINT_RADIUS, ImGuiColors::WHITE);
			ImGui::GetWindowDrawList()->AddText(ImVec2{ vertex.x - 3.5f, vertex.y - 20.0f }, ImGuiColors::YELLOW, std::to_string(n).c_str());
		}
	}
	ImGui::End();
}

void GraphWindow::minSpanTreeUpdate()
{
	_edgesMST = std::make_unique<std::vector<std::shared_ptr<std::pair<ImVec2, ImVec2> > > >();
	if (_edges->size() > 0)
	{
		Graph minSpanTree{ _graph.kruskal() };
		for (auto& [n, edge] : *_edges)
		{
			if (std::find_if(minSpanTree.data().begin(), minSpanTree.data().end(),
				[&](const std::vector<Edge>& vertex) -> bool
				{
					return std::find_if(vertex.begin(), vertex.end(),
					[&](const Edge& e) -> bool
					{
						return e == (*_edgeMap)[n];
					}
					) != vertex.end();
				}
				) != minSpanTree.data().end())
			{
				_edgesMST->push_back(std::make_shared<std::pair<ImVec2, ImVec2> >(edge));
			}
		}
	}
}

void GraphWindow::minSpanTreeTime(const std::string& algorithmName)
{
	_algorithmDurations = std::make_shared<std::map<std::string, double> >();
	Graph _graphTemp{ _graph };
	const auto start{ std::chrono::high_resolution_clock::now() };
	_graphTemp.kruskal();
	const auto end{ std::chrono::high_resolution_clock::now() };
	(*_algorithmDurations)[algorithmName] = std::chrono::duration<double>(end - start).count();
}

const std::string& GraphWindow::nameGet() const
{
	return _name;
}

const ImVec2& GraphWindow::mousePosGet() const
{
	return _windowOffset;
}

const std::unique_ptr<std::map<uint32_t, std::pair<ImVec2, ImVec2> > >& GraphWindow::edgesGet() const
{
	return _edges;
}

const std::unique_ptr<std::map<uint32_t, ImVec2> >& GraphWindow::pointsGet() const
{
	return _points;
}

void GraphWindow::randomGraphGen(const uint32_t& edgeCount)
{
	std::mt19937 gen{ std::random_device{}() };
	std::uniform_real_distribution<float> distX{ 5.0f, constant::DEFAULT_GRAPH_WINDOW_SIZE.x - 20.0f };
	std::uniform_real_distribution<float> distY{ 50.0f, constant::DEFAULT_GRAPH_WINDOW_SIZE.y - 50.0f };

	while (_edges->size() < edgeCount)
	{
		ImVec2 randPos{ distX(gen), distY(gen) };
		ImVec2 randPosOffset{ randPos.x + _windowOffset.x, randPos.y + _windowOffset.y };

		if (_selectedPoint.second == nullptr)
		{
			if (!this->pointSelect(randPosOffset))
				this->pointAdd(ImVec2{ randPos.x, randPos.y });
			else
			{
				_edgeBufferFirst.second = _selectedPoint.second;
				_edgeBufferFirst.first = _selectedPoint.first;
			}
		}
		else if (_selectedPoint.second != nullptr)
		{
			if (this->pointSelect(randPosOffset))
			{
				_edgeBufferSecond.first = _selectedPoint.first;
				this->pointAdd(ImVec2{ *_selectedPoint.second });
			}
			else
			{
				_edgeBufferSecond.first = std::make_shared<uint32_t>(static_cast<uint32_t>(_points->size()));
				this->pointAdd(ImVec2{ randPos.x, randPos.y });
			}
		}
	}
}

inline void GraphWindow::buffersReset()
{
	_selectedPoint.first = nullptr;
	_selectedPoint.second = nullptr;
	_edgeBufferFirst.first = nullptr;
	_edgeBufferSecond.first = nullptr;
	_edgeBufferFirst.second = nullptr;
	_edgeBufferSecond.second = nullptr;
}

inline void GraphWindow::graphReset()
{
	this->buffersReset();
	_points = std::make_unique<std::map<uint32_t, ImVec2> >();
	_edges = std::make_unique<std::map<uint32_t, std::pair<ImVec2, ImVec2> > >();
	_edgeMap = std::make_unique<std::map<uint32_t, Edge> >();
	_edgesMST = std::make_unique<std::vector<std::shared_ptr<std::pair<ImVec2, ImVec2> > > >();
	_algorithmDurations = std::shared_ptr<std::map<std::string, double> >{ nullptr };
}
