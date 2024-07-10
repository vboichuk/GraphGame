
#pragma once

#include "axmol.h"

#include "UI/GameVertex.h"
#include "UI/GameEdge.h"
#include "UI/GameChip.h"
#include "UI/GraphView.h"
#include "Logic/Graph.h"
#include "Logic/GameLogic.h"

USING_NS_AX;

class GameScene final : public Scene
{
    using PARENT_CLASS = Scene;
    
public:
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
	GraphView* m_graph_view = nullptr;
	GraphView* m_preview = nullptr;
    
	GameLogic* m_logic = nullptr;
	Label* m_error_label = nullptr;
};

