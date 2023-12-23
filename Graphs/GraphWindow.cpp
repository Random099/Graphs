#include "GraphWindow.h"

GraphWindow::GraphWindow(const std::string& name) : 
	_name{ name },
	_graph{ Graph() },
	_edgeBufferFirst{ std::make_pair(std::shared_ptr<uint32_t>{ nullptr }, std::shared_ptr<ImVec2>{ nullptr }) },
	_edgeBufferSecond{ std::make_pair(std::shared_ptr<uint32_t>{ nullptr }, std::shared_ptr<ImVec2>{ nullptr }) },
	_selectedPoint{ std::make_pair(std::shared_ptr<uint32_t>{ nullptr }, std::shared_ptr<ImVec2>{ nullptr }) },
	_windowOffset{ ImVec2{ 0, 0 } },
	_mousePos{ ImVec2{ 0, 0 } },
	_edges{ std::make_unique<std::map<uint32_t, std::pair<ImVec2, ImVec2> > >() },
	_points{ std::make_unique<std::map<uint32_t, ImVec2> >() },
	_edgeMap{ std::make_unique<std::map<uint32_t, Edge> >() },
	_displayingMST{ false }
{}


void GraphWindow::pointAdd(const ImVec2& point)
{
	if (_edgeBufferFirst.second == nullptr)
	{
		_edgeBufferFirst.second = std::make_shared<ImVec2>(point);
		if (_points->size() == 0)
		{
			_edgeBufferFirst.first = std::make_shared<uint32_t>(0);
			(*_points)[*_edgeBufferFirst.first] = *_edgeBufferFirst.second;
			this->_graph.data().push_back(std::vector<Edge>());
		}
	}
	else if (_edgeBufferSecond.second == nullptr)
	{
		bool vertexConnectionExists = false;
		_edgeBufferSecond.second = std::make_shared<ImVec2>(point);
		(*_points)[*_edgeBufferSecond.first] = *_edgeBufferSecond.second;
		auto edgesPtr = _graph.edgeSetGet();
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
		}
		this->buffersReset();
	}
}

void GraphWindow::draw()
{
	ImGui::Begin(_name.c_str());
	ImGui::SetWindowSize(ImVec2{ 500, 500 });
	ImGui::Checkbox("Display MST", &_displayingMST);
	if (_displayingMST)
	{
		this->displayMinSpanTree();
	}
	_windowOffset = ImGui::GetCursorScreenPos();
	
	for (const auto& [n, edge] : *_edges)
	{
		ImVec2 vertex1 = ImVec2{ edge.first.x + _windowOffset.x, edge.first.y + _windowOffset.y };
		ImVec2 vertex2 = ImVec2{ edge.second.x + _windowOffset.x, edge.second.y + _windowOffset.y };
		ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, ImGuiColors::WHITE, constant::LINE_THICKNESS);

		int32_t weight = static_cast<int32_t>(std::sqrt(std::pow(vertex2.x - vertex1.x, 2.0f) + std::pow(vertex2.y - vertex1.y, 2.0f)));
		ImGui::GetWindowDrawList()->AddText(helper::MiddlePoint(vertex1, vertex2), ImGuiColors::GREEN, std::to_string(weight).c_str());
	}
	
	for (const auto& [n, point] : *_points)
	{
		ImVec2 vertex = ImVec2(point.x + _windowOffset.x, point.y + _windowOffset.y);
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
	ImGuiIO& io = ImGui::GetIO();
	_mousePos = io.MousePos;
	if (io.MouseClicked[0] && _points->empty())
	{
		this->pointAdd(ImVec2{ _mousePos.x - _windowOffset.x, _mousePos.y - _windowOffset.y });
	}
	else if (io.MouseClicked[0] && _points->size() > 0 && _selectedPoint.second == nullptr)
	{
		this->pointSelect(_mousePos);
		if (_selectedPoint.second != nullptr)
		{
			_edgeBufferFirst.second = _selectedPoint.second;
			_edgeBufferFirst.first = _selectedPoint.first;
		}
	}
	else if (io.MouseClicked[0] && _points->size() > 0 && _selectedPoint.second != nullptr)
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
			this->_graph.removeVertex(static_cast<uint32_t>(this->_points->size()));
			if (this->_edges->size() > 0)
			{
				this->_edges->erase(edgeToRemoveId);
				this->_graph.removeEdge((*_edgeMap)[edgeToRemoveId]);
				this->_edgeMap->erase(edgeToRemoveId);
			}
		}
		else if (this->_edges->size() > 0)
		{
			this->_edges->erase(edgeToRemoveId);
			this->_graph.removeEdge((*_edgeMap)[edgeToRemoveId]);
			this->_edgeMap->erase(edgeToRemoveId);
		}
	}
}

bool GraphWindow::pointSelect(const ImVec2& mousePos)
{
	for (auto& [n, point] : *_points) 
	{
		uint32_t distance = helper::Distance<uint32_t>(ImVec2{ _windowOffset.x + point.x, _windowOffset.y + point.y }, mousePos);
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
		uint32_t distance = helper::Distance<uint32_t>(ImVec2{ _windowOffset.x + (*_points)[i].x, _windowOffset.y + (*_points)[i].y}, mousePos);
		if (distance < constant::POINT_SELECTOR_RADIUS)
		{
			_selectedPoint.second = std::make_shared<ImVec2>((*_points)[i]);
			*_selectedPoint.first = i;
			return true;
		}
	}
	return false;
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

inline void GraphWindow::buffersReset()
{
	_selectedPoint.first = nullptr;
	_selectedPoint.second = nullptr;
	_edgeBufferFirst.first = nullptr;
	_edgeBufferSecond.first = nullptr;
	_edgeBufferFirst.second = nullptr;
	_edgeBufferSecond.second = nullptr;
}

void GraphWindow::displayMinSpanTree()
{
	Graph minSpanTree = _graph.kruskal();
	ImGui::Begin((_name + " MST").c_str());
	ImGui::SetWindowSize(ImVec2{ 500, 500 });
	ImVec2 windowOffsetMST = ImGui::GetCursorScreenPos();
	for (const auto& [n, edge] : *_edges)
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
			ImVec2 vertex1 = ImVec2{ edge.first.x + windowOffsetMST.x, edge.first.y + windowOffsetMST.y };
			ImVec2 vertex2 = ImVec2{ edge.second.x + windowOffsetMST.x, edge.second.y + windowOffsetMST.y };
			ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, ImGuiColors::GREEN, constant::LINE_THICKNESS);
		}	
	}
	for (const auto& [n, point] : *_points)
	{
		ImVec2 vertex = ImVec2(point.x + windowOffsetMST.x, point.y + windowOffsetMST.y);
		ImGui::GetWindowDrawList()->AddCircleFilled(vertex, constant::POINT_RADIUS, ImGuiColors::WHITE);
		ImGui::GetWindowDrawList()->AddText(ImVec2{ vertex.x - 3.5f, vertex.y - 20.0f }, ImGuiColors::YELLOW, std::to_string(n).c_str());
	}
	ImGui::End();
}