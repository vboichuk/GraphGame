
#include "GameScene.h"

#include <iostream>

USING_NS_CC;

GameScene::GameScene()
{
	std::cout << "~GameScene" << std::endl;

	m_graph = new GraphConfig();
	m_logic = new GameLogic();

	m_logic->setOnFinishClb(std::bind(&GameScene::onFinish, this));
}

GameScene::~GameScene()
{
	cocos2d::log("~GameScene");

	delete m_logic, m_logic = nullptr;
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
	
	initBackground();
	initMenu();
	initGame();

	return true;
}

bool GameScene::initGame()
{
	cocos2d::log("GameScene::initGame");

	if (m_graph_view == nullptr)
	{
		m_graph_view = std::make_shared<GraphView>();
		addChild(m_graph_view.get());
	}
	
	if (m_preview == nullptr)
	{
		m_preview = std::make_shared<GraphView>();
		m_preview->setScale(0.3f);
		m_preview->setPosition(600, 500);
		addChild(m_preview.get());
	}

	m_graph->fromFile(FileUtils::getInstance()->fullPathForFilename("Levels/level.txt").c_str());
	
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
	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto normal = Sprite::create("Load.png");
	auto selected = Sprite::create("Load.png");
	auto btn = MenuItemSprite::create(normal, selected);
	btn->setCallback(std::bind(&GameScene::onLoadClick, this, std::placeholders::_1));
	btn->setPosition(100 + btn->getContentSize().width / 2, 700);
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