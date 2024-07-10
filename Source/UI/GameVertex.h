
#pragma once
#include "cocos2d.h"

USING_NS_CC;

using tID = int;

class  GameVertex : public MenuItem
{
	using PARENT_CLASS = MenuItem;

public:
	const int id;

public:
	GameVertex(tID _id) : id(_id) { }
    GameVertex() : MenuItem(), id(0) { }
    
	~GameVertex();
	bool init();
	static GameVertex* create(tID _id);

	void setGlowImage(Node* image);

	void setGlow(bool glow);

protected:
	bool m_is_glowing = false;
	Node* m_glow_image = nullptr;
};

