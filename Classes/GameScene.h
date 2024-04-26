
#pragma once

#include "cocos2d.h"
#include "UI/GameVertex.h"
#include "UI/GameEdge.h"
#include "UI/GameChip.h"
#include "Logic/Graph.h"

#include <map>

class GameScene : public cocos2d::Scene
{
protected:
	enum eZOrder
	{
		edges = 0,
		chip_tip = 1,
		vertex = 2,
		chips = 3,
	};

public:
	static cocos2d::Scene* createScene();
	virtual ~GameScene();
	virtual bool init();
	
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	CREATE_FUNC(GameScene);

protected:
	GraphConfig* m_graph = nullptr;
	GameChip* m_selected_item = nullptr;

	bool initGame();
	bool initEdges();
	bool initVertexes();
	bool initChips();

	bool initBackground();
	bool initMenu();

	void onVertexClick(Ref* obj, tID id);
	void onChipClick(Ref* obj, tChipId chip_id);
	void onLoadClick(Ref* obj);

	void setSelectedChip(tChipId chip_id);
	void moveChip(tChipId chip_id, tID vert_id);

	void glowAvailableVertexes();

protected:
	std::map<tID, GameVertex*> m_vertexes;
	std::map<tChipId, GameChip*> m_chips;
	tChipId m_selected_chip_id = -1;
};

