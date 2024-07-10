
#include "GameLogic.h"

GameLogic::~GameLogic()
{
	AXLOG("~GameLogic");

	if (m_view)
	{
		m_view->setOnClickVertexClb(nullptr);
		m_view->setOnClickChipClb(nullptr);
        m_view->release();
	}
}

bool GameLogic::init(GraphView* view, GraphConfig* graph)
{
	if (!view || !graph)
		return false;

	if (m_view)
	{
		m_view->setOnClickVertexClb(nullptr);
		m_view->setOnClickChipClb(nullptr);
        m_view->release();
	}

	m_graph = graph;
	m_view = view;
    m_view->retain();

	m_view->setOnClickVertexClb(std::bind(&GameLogic::onVertexClick, this, std::placeholders::_1));
	m_view->setOnClickChipClb(std::bind(&GameLogic::onChipClick, this, std::placeholders::_1));

	m_selected_chip_id = -1;

	return true;
}

void GameLogic::onVertexClick(tID id)
{
	if (m_selected_chip_id < 0)
		return;

	moveChip(m_selected_chip_id, id);
}

void GameLogic::onChipClick(tChipId chip_id)
{
	setSelectedChip(chip_id);
}

void GameLogic::setSelectedChip(tChipId chip_id)
{
	if (m_selected_chip_id == chip_id)
		return;

	auto& chips = m_view->getChips();

	if (m_selected_chip_id >= 0)
		chips.at(m_selected_chip_id)->setSelected(false);

	m_selected_chip_id = chip_id;

	if (m_selected_chip_id >= 0)
		chips.at(m_selected_chip_id)->setSelected(true);

	glowAvailableVertexes();
}

void GameLogic::moveChip(tChipId chip_id, tID id)
{
	std::vector<tID> path;
	const auto from_id = m_graph->getVertexId(chip_id);
	const auto success = m_graph->moveChip(chip_id, id, OUT path);
	if (success)
	{
		const auto& chips = m_view->getChips();

		Vector<FiniteTimeAction*> actions;
		for (auto id : path)
		{
			const auto [x, y] = m_graph->getPosition(id);
			auto move_to = MoveTo::create(0.2f, Point(x, y));
			actions.pushBack(move_to);
		}

		actions.pushBack(CallFunc::create([this]() { this->checkFinish(); }));
		
		auto sequence = Sequence::create(actions);
		chips.at(chip_id)->runAction(sequence);
	}

	glowAvailableVertexes();
}

void GameLogic::glowAvailableVertexes()
{
	if (m_selected_chip_id < 0)
		return;

	const auto vert_id = m_graph->getVertexId(m_selected_chip_id);
	const auto& m_vertexes = m_view->getVertexes();

	auto available_verts = m_graph->getAvailableVertexes(vert_id);
	for (auto [id, v] : m_vertexes)
	{
		bool available = available_verts.find(id) != available_verts.end();
		v->setGlow(available);
		v->setEnabled(available);
	}
}

void GameLogic::checkFinish()
{
	if (m_graph->isAllChipsOK())
	{
		if (m_finish_clb)
			m_finish_clb();
	}
}
