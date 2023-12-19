#include "GraphWindow.h"

GraphWindow::GraphWindow(const std::string& name) : 
	_name(name), 
	_graph(Graph()), 
	_edgeBuffer(std::make_pair(nullptr, nullptr)),
	_currentVertex(0), 
	_mousePos(ImVec2(0, 0)), 
	_edges(std::make_unique<std::vector<std::pair<ImVec2, ImVec2> > >()),
	_points(std::make_unique<std::vector<ImVec2> >()),
	_selectedPoint(nullptr)
{}


void GraphWindow::pointAdd(const ImVec2& point)
{
	if (_edgeBuffer.first == nullptr)
	{
		_edgeBuffer.first = std::make_shared<ImVec2>(point);
		_points->push_back(*_edgeBuffer.first);
	}
	else if (_edgeBuffer.second == nullptr)
	{
		_edgeBuffer.second = std::make_shared<ImVec2>(point);
		int32_t weight = static_cast<int32_t>(std::sqrt(std::pow(_edgeBuffer.second->x - _edgeBuffer.first->x, 2.0f) + std::pow(_edgeBuffer.second->y - _edgeBuffer.first->y, 2.0f)));
		_graph.edgeAdd(Edge{ _currentVertex, _currentVertex + 1, weight });
		_edges->push_back(std::make_pair(*_edgeBuffer.first, *_edgeBuffer.second));
		_points->push_back(*_edgeBuffer.second);
		if (_graph.vertexCountGet() == 0)
			_currentVertex += 2;
		_currentVertex++;
		_edgeBuffer.first = nullptr;
		_edgeBuffer.second = nullptr;
	}
}

void GraphWindow::draw()
{
	ImGui::Begin(_name.c_str());
	float thickness = 2.0f;
	_mousePos = ImGui::GetCursorScreenPos();

	for (std::pair<ImVec2, ImVec2>& edge : *_edges)
	{
		ImVec2 vertex1 = ImVec2(edge.first.x + _mousePos.x, edge.first.y + _mousePos.y);
		ImVec2 vertex2 = ImVec2(edge.second.x + _mousePos.x, edge.second.y + _mousePos.y);
		ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, ImGuiColors::white, thickness);
	}
	for (ImVec2& point : *_points)
	{
		ImVec2 vertex = ImVec2(point.x + _mousePos.x, point.y + _mousePos.y);
		ImGui::GetWindowDrawList()->AddCircleFilled(vertex, 5.0f, ImGuiColors::white);
	}
	if (_selectedPoint != nullptr)
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2{ _selectedPoint->x + _mousePos.x, _selectedPoint->y + _mousePos.y }, 5.0f, ImGuiColors::red);
	ImGui::End();
}

void GraphWindow::handlePoints()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;
	if (io.MouseClicked[0] && _points->size() == 0)
	{
		this->pointAdd(ImVec2{ mousePos.x - _mousePos.x, mousePos.y - _mousePos.y });
	}
	else if (io.MouseClicked[0] && _points->size() > 0 && _selectedPoint == nullptr)
	{
		this->pointSelect(mousePos);
		if (_selectedPoint != nullptr)
		{
			_edgeBuffer.first = _selectedPoint;
		}
	}
	else if (io.MouseClicked[0] && _points->size() > 0 && _selectedPoint != nullptr)
	{
		if (this->pointSelect(mousePos))
			this->pointAdd(ImVec2{ *_selectedPoint });
		else
			this->pointAdd(ImVec2{ mousePos.x - _mousePos.x, mousePos.y - _mousePos.y });
		_selectedPoint = nullptr;
	}
	else if (io.MouseClicked[1])
	{
		_edgeBuffer.first = nullptr;
		_edgeBuffer.second = nullptr;
		if (this->edgesGet()->size() > 0)
			this->edgesGet()->pop_back();
		if (this->pointsGet()->size() > 0)
			this->pointsGet()->pop_back();
	}
}

bool GraphWindow::pointSelect(const ImVec2& mousePos)
{
	_selectedPoint = nullptr;
	for (ImVec2& point : *_points)
	{
		float distance = std::sqrt(std::pow(mousePos.x - (_mousePos.x + point.x), 2.0f) + std::pow(mousePos.y - (_mousePos.y + point.y), 2.0f));
		if (distance < 15.0f)
		{
			_selectedPoint = std::make_shared<ImVec2>(point);
			return true;
		}
	}
	return false;
}

const std::string& GraphWindow::nameGet() const
{
	return _name;
}

ImVec2& GraphWindow::mousePosGet()
{
	return _mousePos;
}

std::unique_ptr<std::vector<std::pair<ImVec2, ImVec2> > >& GraphWindow::edgesGet()
{
	return _edges;
}

std::unique_ptr<std::vector<ImVec2> >& GraphWindow::pointsGet()
{
	return _points;
}