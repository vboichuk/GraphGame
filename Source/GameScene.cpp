
#include "GameScene.h"

USING_NS_AX;

GameScene::GameScene()
{
    AXLOG("GameScene");

	m_graph = new GraphConfig();
	m_logic = new GameLogic();

	m_logic->setOnFinishClb([this] { onFinish(); });
}

GameScene::~GameScene()
{
    AXLOG(__FUNCTION__);

    m_graph_view->release();
    m_preview->release();

	delete m_logic, m_logic = nullptr;
	delete m_graph, m_graph = nullptr;
}

bool GameScene::init()
{
	if ( !PARENT_CLASS::init() )
		return false;

	{
		auto label = Label::createWithTTF("", "fonts/arial.ttf", 20);
		label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		label->setIgnoreAnchorPointForPosition(false);
		label->setPosition(20, 20);
		label->setColor(Color3B::RED);
		label->setVisible(false);
		addChild(label);
		m_error_label = label;
	}

	initBackground();
	initMenu();
	initGame();

	return true;
}

bool GameScene::initGame()
{
    AXLOG("GameScene::initGame");

    const auto size = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

	if (m_graph_view == nullptr)
	{
        m_graph_view = new GraphView();
        m_graph_view->setPosition(origin + size / 2);
		addChild(m_graph_view);
	}

	if (m_preview == nullptr)
	{
		m_preview = new GraphView();
		m_preview->setScale(0.3f);
        m_preview->setPosition(origin + size * 0.85f);
		addChild(m_preview);
	}

    constexpr auto path = "Levels/level.txt";
    const auto fullpath = FileUtils::getInstance()->fullPathForFilename(path);
    auto text = FileUtils::getInstance()->getStringFromFile(fullpath);

	m_graph->fromFile(text);
	m_error_label->setString(std::string("Invalid file format '") + path + "'");
	m_error_label->setVisible(m_graph->empty());

	m_graph_view->init(m_graph, m_graph->getChips());
	m_preview->init(m_graph, m_graph->getChipsOrder());
	m_logic->init(m_graph_view, m_graph);

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
    const auto origin = Director::getInstance()->getVisibleOrigin();
    const auto size = Director::getInstance()->getVisibleSize();

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto normal = Sprite::create("Load.png");
	auto selected = Sprite::create("Load.png");
	auto btn = MenuItemSprite::create(normal, selected);
	btn->setCallback(std::bind(&GameScene::onLoadClick, this, std::placeholders::_1));
	btn->setPositionX(origin.x + btn->getContentSize().width / 2 + 20);
	btn->setPositionY(origin.y + size.y - btn->getContentSize().height / 2 - 20);
	menu->addChild(btn);

	return true;
}

void GameScene::onLoadClick(Ref*)
{
	initGame();
}

void GameScene::onFinish()
{
	auto label = Label::createWithTTF("You win!", "fonts/arial.ttf", 20);
	label->setAnchorPoint(Point::ANCHOR_MIDDLE);
	label->setIgnoreAnchorPointForPosition(false);
	label->setPosition(_contentSize / 2);
	label->setColor(Color3B::BLACK);
	addChild(label, getChildrenCount());

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(ScaleTo::create(2.f, 2.f));
	actions.pushBack(DelayTime::create(2.f));
	actions.pushBack(FadeOut::create(1.f));
	actions.pushBack(RemoveSelf::create());
	auto sequence = Sequence::create(actions);
	label->runAction(sequence);
}
