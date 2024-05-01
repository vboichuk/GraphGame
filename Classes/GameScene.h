
#pragma once

#include "cocos2d.h"
#include "UI/GameVertex.h"
#include "UI/GameEdge.h"
#include "UI/GameChip.h"
#include "UI/GraphView.h"
#include "Logic/Graph.h"
#include "Logic/GameLogic.h"

#include <map>

class GameScene : public cocos2d::Scene
{
protected:

public:
	static cocos2d::Scene* createScene();
	
	GameScene();
	~GameScene();
	bool init() override;
		
	CREATE_FUNC(GameScene);

protected:
	GraphConfig* m_graph = nullptr;
	GameChip* m_selected_item = nullptr;

	bool initGame();

	bool initBackground();
	bool initMenu();

	void onLoadClick(Ref* obj);
	void onFinish();

protected:

	std::shared_ptr<GraphView> m_graph_view = nullptr;
	std::shared_ptr<GraphView> m_preview = nullptr;
	GameLogic* m_logic = nullptr;
};

