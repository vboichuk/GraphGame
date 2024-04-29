
#include "GameScene.h"

USING_NS_CC;

GameScene::~GameScene()
{
	if (m_graph)
		delete m_graph, m_graph = nullptr;
}

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if ( !Scene::init() )
	{
		return false;
	}

	m_graph = new GraphConfig();

	initBackground();
	initMenu();

	initGame();

	return true;
}

bool GameScene::initGame()
{
	m_graph->fromFile(FileUtils::getInstance()->fullPathForFilename("Levels/level1.txt").c_str());

	initEdges();
	initVertexes();
	initChips();

	return true;
}

bool GameScene::initEdges()
{
	removeChildByName("Edges");

	auto lc = Node::create();
	this->addChild(lc, eZOrder::edges, "Edges");

	const auto& edges = m_graph->getEdges();
	
	for (auto [id1, id2] : edges)
	{
		 auto edge = GameEdge::create(m_graph, id1, id2);
		 lc->addChild(edge);
	}
	return true;
}

bool GameScene::initVertexes()
{
	m_vertexes.clear();
	removeChildByName("Vertexes", true);

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, eZOrder::vertex, "Vertexes");

	const auto ids = m_graph->getVertexIds();
	for (auto id : ids)
	{
		auto vert = GameVertex::create(id);
		auto clb = std::bind(&GameScene::onVertexClick, this, std::placeholders::_1, id);
		vert->setCallback(clb);
		auto [x, y] = m_graph->getPosition(id);
		vert->setPosition(x, y);
		menu->addChild(vert);
		m_vertexes[id] = vert;
	}

	return true;
}

static const std::vector<Color3B> colors {
	Color3B(20, 255, 20), 
	Color3B(255, 20, 20), 
	Color3B(20, 20, 255), 
	Color3B(255, 255, 20), 
	Color3B(20, 255, 255), 
	Color3B(255, 20, 255), 
	Color3B(20, 255, 20), 
};

bool GameScene::initChips()
{
	m_selected_chip_id = -1;
	m_chips.clear();
	this->removeChildByName("Chips", true);
	this->removeChildByName("ChipsOrder", true);

	{
		auto node = Node::create();
		this->addChild(node, eZOrder::chip_tip, "ChipsOrder");

		auto& chips = m_graph->getChipsOrder();
		for (size_t i = 0; i < chips.size(); i++)
		{
			auto chip = GameChip::create(colors[i]);
			chip->setScale(1.3f);
			chip->setOpacity(80);
			chip->setEnabled(false);
			auto [x, y] = m_graph->getPosition(chips[i]);
			chip->setPosition(x, y);
			node->addChild(chip);
		}
	}

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, eZOrder::chips, "Chips");

	auto& chips = m_graph->getChips();
	for (size_t i = 0; i < chips.size(); i++)
	{
		auto chip = GameChip::create(colors[i]);
		auto [x, y] = m_graph->getPosition(chips[i]);
		chip->setPosition(x, y);
		auto clb = std::bind(&GameScene::onChipClick, this, std::placeholders::_1, i);
		chip->setCallback(clb);
		menu->addChild(chip);
		m_chips[i] = chip;
	}

	return true;
}

bool GameScene::initBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto lc = LayerColor::create(Color4B::WHITE);
	lc->setContentSize(visibleSize);
	lc->setPosition(origin);
	this->addChild(lc);
	
	return true;
}

bool GameScene::initMenu()
{
	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto btn = MenuItemImage::create("Load.png", "Load.png");
	btn->setCallback(std::bind(&GameScene::onLoadClick, this, std::placeholders::_1));
	btn->setPosition(100, 500);
	menu->addChild(btn);

	return true;
}

void GameScene::setSelectedChip(tChipId chip_id)
{
	if (m_selected_chip_id == chip_id)
		return;

	if (m_selected_chip_id >= 0)
	{
		m_chips.at(m_selected_chip_id)->setSelected(false);
	}

	m_selected_chip_id = chip_id;

	if (m_selected_chip_id >= 0)
	{
		m_chips.at(m_selected_chip_id)->setSelected(true);
	}

	glowAvailableVertexes();
}

void GameScene::onVertexClick(Ref*, tID id)
{
	if (m_selected_chip_id < 0)
		return;

	moveChip(m_selected_chip_id, id);
}

void GameScene::onChipClick(Ref*, tChipId chip_id)
{
	CCLOG("click (%d)", chip_id);
	setSelectedChip(chip_id);
}

void GameScene::onLoadClick(Ref*)
{
	initGame();
}

void GameScene::moveChip(tChipId chip_id, tID id)
{
	std::vector<tID> path;
	const auto from_id = m_graph->getVertexId(chip_id);
	const auto success = m_graph->moveChip(chip_id, id, OUT path);
	if (success)
	{
		Vector<FiniteTimeAction*> actions;
		for (auto id : path)
		{
			const auto [x, y] = m_graph->getPosition(id);
			auto move_to = MoveTo::create(0.2f, Point(x, y));
			actions.pushBack(move_to);
		}
		auto sequence = Sequence::create(actions);
		m_chips[m_selected_chip_id]->runAction(sequence);
	}

	glowAvailableVertexes();
}

void GameScene::glowAvailableVertexes()
{
	if (m_selected_chip_id < 0)
		return;

	const auto vert_id = m_graph->getVertexId(m_selected_chip_id);

	auto available_verts = m_graph->getAvailableVertexes(vert_id);
	for (auto [id, v] : m_vertexes)
	{
		bool available = available_verts.find(id) != available_verts.end();
		v->setGlow(available);
		v->setEnabled(available);
	}
}