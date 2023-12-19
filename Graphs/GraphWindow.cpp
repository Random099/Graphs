#include "GraphWindow.h"

GraphWindow::GraphWindow(const std::string& name) : _name(name), _graph(Graph()), _edgeBuffer(std::make_pair(nullptr, nullptr)),
	_currentVertex(0), _mousePos(ImVec2(0, 0)), _edges(std::make_unique< std::vector < std::pair<ImVec2, ImVec2> > >()){}


void GraphWindow::addPoint(const ImVec2& point)
{
	if (_edgeBuffer.first == nullptr)
	{
		_edgeBuffer.first = std::make_shared<ImVec2>(point);
	}
	else if (_edgeBuffer.second == nullptr)
	{
		_edgeBuffer.second = std::make_shared<ImVec2>(point);
		int32_t weight = static_cast<int32_t>(std::sqrt(std::pow(_edgeBuffer.second->x - _edgeBuffer.first->x, 2.0f) + std::pow(_edgeBuffer.second->y - _edgeBuffer.first->y, 2.0f)));
		_graph.edgeAdd(Edge{ _currentVertex, _currentVertex + 1, weight });
		_edges->push_back(std::make_pair(*_edgeBuffer.first, *_edgeBuffer.second));
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
	ImU32 color = IM_COL32(255, 255, 255, 255);
	float thickness = 2.0f;
	_mousePos = ImGui::GetCursorScreenPos();
	for (std::pair<ImVec2, ImVec2>& edge : *_edges)
	{
		ImVec2 vertex1 = ImVec2(edge.first.x + _mousePos.x, edge.first.y + _mousePos.y);
		ImVec2 vertex2 = ImVec2(edge.second.x + _mousePos.x, edge.second.y + _mousePos.y);
		ImGui::GetWindowDrawList()->AddLine(vertex1, vertex2, color, thickness);
	}
	ImGui::End();
}

void GraphWindow::handlePoints()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.MouseClicked[0])
	{
		ImVec2 mousePos = io.MousePos;
		this->addPoint(ImVec2{ mousePos.x - _mousePos.x, mousePos.y - _mousePos.y });
	}
	else if (io.MouseClicked[1])
	{
		if (this->edgesGet()->size() > 0)
			this->edgesGet()->pop_back();
	}
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