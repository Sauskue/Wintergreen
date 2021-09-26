#include "pch.h"
#include "Demos.h"


/*A Star
* 2-D grid of nodes
* f(n) = g(n) + h(n)
* node cost = distance from start to current + heuristic
* heuristic = shortest distance(straight line distance) to end node
* open set = nodes that haven't been evaluated yet
* closed set = nodes that have been evaluated
* algorithm finishes when open set is empty(no solution) or when optimal route is found
* open set starts with start node, closed set starts empty
* NODE CLASS
* neighbors,
* parent,
* local(distance from start node to current node),
* global(heuristic - straight line distance to end goal)
* 
* RULES
* start node -> global dist
* discover start node neighbors
* 
*/

static int del_index = -1;

float distance(AStar::Node* a, AStar::Node* b)
{
	return sqrtf(((b->x - a->x) * (b->x - a->x)) + ((b->y - a->y) * (b->y - a->y)));
}

AStar::Node::Node(int x, int y):
	x(x),
	y(y),
	local_goal(INT_MAX),
	global_goal(INT_MAX),
	visited(false),
	parent(nullptr),
	neighbors(std::vector<Node*>())
{}

bool AStar::Node::operator<(const AStar::Node& other)
{
	return (this->global_goal < other.global_goal);
}

void AStar::OnCreate()
{
	SetSize(width, height);
	SetTitle(L"A* Pathfinding");

	HRESULT hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory),
		(void**)&d2d1_factory
	);
	hr = d2d1_factory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(main_window_handle, D2D1::SizeU(width, height)),
		&d2d1_rt
	);
	hr = d2d1_rt->CreateSolidColorBrush
	(
		D2D1::ColorF(D2D1::ColorF::White),
		&d2d1_brush
	);
	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&dwrite_factory
	);
	hr = dwrite_factory->CreateTextFormat
	(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f,
		L"en-us",
		&dwrite_tf
	);
	//start_node->local_goal = 0;
	//start_node->global_goal = distance(start_node, end_node);
}

void AStar::OnUpdate()
{
	while (!Event::IsBufferEmpty())
	{
		Event e = Event::ReadBuffer();
		if (e.GetType() == Event::Type::KeyPress)
		{
			if (e.GetKeyCode() == KeyCode::Shift)
			{
				if(selected_node && selected_node != end_node)
				{
					if (selected_node == start_node)
						start_node = nullptr;
					else
						start_node = selected_node;
						
				}
			}
			else if (e.GetKeyCode() == KeyCode::Control)
			{
				if (selected_node && selected_node != start_node)
				{
					if (selected_node == end_node)
						end_node = nullptr;
					else
						end_node = selected_node;
				}
			}
			else if (e.GetKeyCode() == KeyCode::Backspace)
			{
				if (selected_node)
				{
					if (selected_node == start_node)
						start_node = nullptr;
					if (selected_node == end_node)
						end_node = nullptr;
					for (Node* neighbor : selected_node->neighbors)
					{
						for (int i = 0; i < neighbor->neighbors.size(); i++)
						{
							if (neighbor->neighbors[i] == selected_node)
							{
								neighbor->neighbors.erase(neighbor->neighbors.begin() + i);
								i--;
							}
						}
					}
					delete nodes[del_index];
					nodes.erase(nodes.begin() + del_index);
					selected_node = nullptr;
				}
			}
		}
		else if (e.GetType() == Event::Type::MouseLeftPress)
		{
			int m_x = e.GetX();
			int m_y = e.GetY();
			bool mark_for_delete = false;
			bool mark_for_create = true;
			for (int i = 0; i < nodes.size(); i++)
			{
				Node* current_node = nodes[i];
				int node_x = current_node->x;
				int node_y = current_node->y;
				//check for node-click
				if
				(
					m_x >= node_x - node_radius - node_border &&
					m_x <= node_x + node_radius + node_border &&
					m_y >= node_y - node_radius - node_border &&
					m_y <= node_y + node_radius + node_border
				)
				{
					if
					(
						m_x >= node_x - node_radius &&
						m_x <= node_x + node_radius &&
						m_y >= node_y - node_radius &&
						m_y <= node_y + node_radius
					)
					{
						if (selected_node == current_node)
							selected_node = nullptr;
						else
							selected_node = current_node;
						del_index = i;
					}
					mark_for_create = false;
				}
			}
			if (mark_for_create)
			{
				nodes.push_back(new Node(m_x, m_y));
			}
		}
		else if (e.GetType() == Event::Type::MouseRightPress)
		{
			if (selected_node)
			{
				int m_x = e.GetX();
				int m_y = e.GetY();
				for (int i = 0; i < nodes.size(); i++)
				{
					Node* current_node = nodes[i];
					int node_x = nodes[i]->x;
					int node_y = nodes[i]->y;
					if
						(
							m_x >= node_x - node_radius &&
							m_x <= node_x + node_radius &&
							m_y >= node_y - node_radius &&
							m_y <= node_y + node_radius
							)
					{
						if (current_node == selected_node)
							continue;
						bool add1 = true;
						int j1;
						int j2;
						for (int j = 0; j < current_node->neighbors.size(); j++)
						{
							if (current_node->neighbors[j] == selected_node)
							{
								j1 = j;
								add1 = false;
							}
						}
						if (add1)
							current_node->neighbors.push_back(selected_node);
						else
							current_node->neighbors.erase(current_node->neighbors.begin() + j1);
						bool add2 = true;
						for (int j = 0; j < selected_node->neighbors.size(); j++)
						{
							if (selected_node->neighbors[j] == current_node)
							{
								j2 = j;
								add2 = false;
							}
						}
						if (add2)
							selected_node->neighbors.push_back(current_node);
						else
							selected_node->neighbors.erase(selected_node->neighbors.begin() + j2);
					}
				}
			}
		}
	}
	RunAlgorithm();
}

void AStar::OnRender()
{
	d2d1_rt->BeginDraw();
	d2d1_rt->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::RoyalBlue));
	for (Node* node : nodes)
	{
			D2D1_POINT_2F node_point;
			node_point.x = node->x;
			node_point.y = node->y;
		for (Node* neighbor : node->neighbors)
		{
			D2D1_POINT_2F neighbor_point;
			neighbor_point.x = neighbor->x;
			neighbor_point.y = neighbor->y;
			d2d1_rt->DrawLine(node_point, neighbor_point, d2d1_brush);
		}
	}
	if (start_node && end_node)
	{
		d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
		Node* pathNode = end_node;
		while (pathNode->parent)
		{
			D2D1_POINT_2F node_point;
			node_point.x = pathNode->x;
			node_point.y = pathNode->y;
			D2D1_POINT_2F parent_point;
			parent_point.x = pathNode->parent->x;
			parent_point.y = pathNode->parent->y;
			d2d1_rt->DrawLine(node_point, parent_point, d2d1_brush);
			pathNode = pathNode->parent;
		}
	}
	for (Node* node : nodes)
	{
		D2D1_ELLIPSE e;
		e.point.x = node->x;
		e.point.y = node->y;
		e.radiusX = node_radius;
		e.radiusY = node_radius;
		if (node == selected_node)
		{
			D2D1_ELLIPSE se;
			se.point.x = node->x;
			se.point.y = node->y;
			se.radiusX = selection_node_radius;
			se.radiusY = selection_node_radius;
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
			d2d1_rt->DrawEllipse(se, d2d1_brush);
		}
		if (node == start_node)
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
		else if (node == end_node)
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		else if (node->visited)
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
		else
			d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::RoyalBlue));
		d2d1_rt->FillEllipse(e, d2d1_brush);
	}

	d2d1_brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	const wchar_t* text =
		L"LEFT CLICK: ADD POINT\n"
		"RIGHT CLICK: TOGGLE NEIGHBOR\n"
		"SHIFT: SET START NODE\n"
		"CONTROL: SET END NODE\n"
		"CLICK ON POINT: TOGGLE ACTIVE POINT\n"
		"BACKSPACE: DELETE SELECTED POINT\n";
	d2d1_rt->DrawText
	(
		text,
		(UINT32)wcslen(text),
		dwrite_tf,
		D2D1::RectF(0.0f, 0.0f, (float)width, 50.0f),
		d2d1_brush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);
	d2d1_rt->EndDraw();
}

void AStar::OnDestroy()
{
	dwrite_tf->Release();
	dwrite_factory->Release();
	d2d1_brush->Release();
	d2d1_rt->Release();
	d2d1_factory->Release();
	if(start_node)
		delete start_node;
	if(end_node)
		delete end_node;
	open_list.clear();
	closed_list.clear();
}

void AStar::RunAlgorithm()
{
	if (!start_node || !end_node)
		return;

	for (Node* node : nodes)
	{
		node->parent = nullptr;
		node->visited = false;
		node->local_goal = std::numeric_limits<unsigned int>::max();
		node->global_goal = std::numeric_limits<unsigned int>::max();
	}
	open_list.empty();
	closed_list.empty();

	Node* current_node = start_node;
	start_node->local_goal = 0;
	start_node->global_goal = distance(start_node, end_node);
	open_list.push_back(start_node);

	while (!open_list.empty())
	{
		std::sort(open_list.begin(), open_list.end());
		while (!open_list.empty() && open_list.front()->visited)
		{
			open_list.erase(open_list.begin());
		}
		if (open_list.empty())
			break;
		current_node = open_list.front();
		current_node->visited = true;
		for (Node* neighbor : current_node->neighbors)
		{
			if (!neighbor->visited)
				open_list.push_back(neighbor);

			float local_goal_mark = current_node->local_goal + distance(current_node, neighbor);
			if (local_goal_mark < neighbor->local_goal)
			{
				neighbor->parent = current_node;
				neighbor->local_goal = local_goal_mark;
				neighbor->global_goal = neighbor->local_goal + distance(neighbor, end_node);
			}
		}
	}
}