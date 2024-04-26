#pragma once

#include "cocos2d.h"
#include "Logic/Graph.h"

USING_NS_CC;

class GameChip : public MenuItem
{
	using PARENT_CLASS = MenuItem;

protected:
	GameChip() { };
	bool init(const Color3B& color);

public:
	static GameChip* create(const Color3B& color);
	void setSelected(bool selected);

protected:
	bool m_is_selected = false;
	Node* m_normal_state = nullptr;
	Node* m_selected_state = nullptr;
};

