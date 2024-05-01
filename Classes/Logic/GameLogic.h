#pragma once

#include "cocos2d.h"
#include "UI/GraphView.h"

class  GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    bool init(std::shared_ptr<GraphView> view, GraphConfig* graph);
    void setOnFinishClb(std::function<void()> clb) { m_finish_clb = clb; }

protected:
    
    void onVertexClick(tID id);
    void onChipClick(tChipId chip_id);
    void setSelectedChip(tChipId chip_id);
    void moveChip(tChipId chip_id, tID id);
	void glowAvailableVertexes();
	void checkFinish();

protected:

    std::shared_ptr<GraphView> m_view = nullptr;
    GraphConfig* m_graph = nullptr;

    tChipId m_selected_chip_id = -1;

    std::function<void()> m_finish_clb = nullptr;
};

