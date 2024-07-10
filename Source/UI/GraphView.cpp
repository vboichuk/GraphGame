#include "GraphView.h"

static Color3B hueTorgb(int hue)
{
	if (hue > 360.0 || hue < 0)
		return Color3B::BLACK;

	double r, g, b;

	const int _h = (long)(hue / 60.0);
	const double _c = 1.0;
	const double _x = _c * (1 - fabs((_h % 2) - 1));

	if (hue < 60.0)
	{
		r = _c;
		g = _x;
		b = 0;
	}
	else if (hue < 160.0)
	{
		r = _x;
		g = _c;
		b = 0;
	}
	else if (hue < 180.0)
	{
		r = 0;
		g = _c;
		b = _x;
	}
	else if (hue < 240.0)
	{
		r = 0;
		g = _x;
		b = _c;
	}
	else if (hue <= 300.0)
	{
		r = _x;
		g = 0;
		b = _c;
	}
	else
	{
		r = _c;
		g = 0;
		b = _x;
	}

	return Color3B(r * 255, g * 255, b * 255);
}


Color3B GraphView::getColor(int i, int max)
{
	if (max <= 0 || i < 0 || i >= max)
		return Color3B::BLACK;

    const int hue = (i + 1) / (float)max * 360;

    return hueTorgb(hue);
}

GraphView::~GraphView()
{
	AXLOG("~GraphView");
}

bool GraphView::init(const GraphConfig* graph, const std::vector<tID>& order)
{
	if (!PARENT_CLASS::init())
		return false;

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	m_graph = graph;

	initEdges();
	initVertexes();
	initChips(order);

    const auto content_pos = -m_content_rect.origin - m_content_rect.size / 2;
    getChildByName(m_edges_name)->setPosition(content_pos);
    getChildByName(m_vertexes_name)->setPosition(content_pos);
    getChildByName(m_chips_name)->setPosition(content_pos);

	return true;
}

bool GraphView::initEdges()
{
	removeChildByName(m_edges_name);

	auto node = Node::create();
	this->addChild(node, 1, m_edges_name);

	const auto& edges = m_graph->getEdges();

	for (auto [id1, id2] : edges)
	{
		auto edge = GameEdge::create(m_graph, id1, id2);
        node->addChild(edge);
	}
	return true;
}

bool GraphView::initVertexes()
{
	m_vertexes.clear();
	removeChildByName(m_vertexes_name, true);

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1, m_vertexes_name);

	const auto ids = m_graph->getVertexIds();

    m_content_rect = Rect();

	for (auto id : ids)
	{
		auto vert = GameVertex::create(id);
		auto clb = std::bind(&GraphView::onVertexClick, this, std::placeholders::_1, id);
		vert->setCallback(clb);

		auto [x, y] = m_graph->getPosition(id);
		vert->setPosition(x, y);
		menu->addChild(vert);
		m_vertexes[id] = vert;

        const auto box = vert->getBoundingBox();

        if (m_content_rect.size.x <= 0.01f)
        {
            m_content_rect = box;
        }
        else
        {
            m_content_rect = m_content_rect.unionWithRect(box);
        }
	}

	return true;
}

bool GraphView::initChips(const std::vector<tID>& order)
{
	m_chips.clear();
	this->removeChildByName(m_chips_name, true);

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1, m_chips_name);

	const auto& chips = order;
	const auto chips_count = chips.size();
	for (size_t i = 0; i < chips_count; i++)
	{
		auto chip = GameChip::create(getColor(i, chips_count));
		auto [x, y] = m_graph->getPosition(chips[i]);
		chip->setPosition(x, y);
		auto clb = std::bind(&GraphView::onChipClick, this, std::placeholders::_1, i);
		chip->setCallback(clb);
		menu->addChild(chip);
		m_chips[i] = chip;
	}

	return true;
}

void GraphView::onVertexClick(Ref*, tID id)
{
	if (m_click_vertex_clb)
		m_click_vertex_clb(id);
}

void GraphView::onChipClick(Ref*, tChipId chip_id)
{
	if (m_click_chip_clb)
		m_click_chip_clb(chip_id);
}
