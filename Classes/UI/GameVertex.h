
#pragma once
#include "cocos2d.h"
#include "Logic/Graph.h"

USING_NS_CC;

class  GameVertex : public MenuItem
{
	using PARENT_CLASS = MenuItem;

public:
	const tID id;

public:
	GameVertex(tID _id) : id(_id) {}
	~GameVertex();
	bool init();
	static GameVertex* create(tID _id);

	void setGlowImage(Node* image);

	void setGlow(bool glow);

protected:
	bool m_is_glowing = false;
	Node* m_glow_image = nullptr;
	Label* m_debug_label = nullptr;
};

