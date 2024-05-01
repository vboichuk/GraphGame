#pragma once

#include "cocos2d.h"
#include "Logic/Graph.h"
#include "UI/GameChip.h"
#include "UI/GameVertex.h"
#include "UI/GameEdge.h"

USING_NS_CC;

using tVertexId_clb = std::function<void(tID)>;
using tChipId_clb = std::function<void(tChipId)>;

class GraphView : public Node
{
	using PARENT_CLASS = Node;

protected:
	bool initEdges();
	bool initVertexes();
	bool initChips(const std::vector<tID>& order);

	void onVertexClick(Ref* obj, tID id);
	void onChipClick(Ref* obj, tChipId chip_id);

	static Color3B getColor(int i, int max);

public:
	~GraphView();
	bool init(const GraphConfig* graph, const std::vector<tID>& order);
	void setOnClickVertexClb(tVertexId_clb clb) { m_click_vertex_clb = clb; }
	void setOnClickChipClb(tVertexId_clb clb) { m_click_chip_clb = clb; }

	const auto& getChips() const { return m_chips; }
	const auto& getVertexes() const { return m_vertexes; }

protected:
	const GraphConfig* m_graph = nullptr;

	std::map<tID, GameVertex*> m_vertexes;
	std::map<tChipId, GameChip*> m_chips;

	tVertexId_clb m_click_vertex_clb = nullptr;
	tChipId_clb m_click_chip_clb = nullptr;
};

