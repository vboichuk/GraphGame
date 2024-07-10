
#include "GameVertex.h"

GameVertex* GameVertex::create(int _id)
{
	auto ret = new (std::nothrow) GameVertex(_id);
	ret->init();
	ret->autorelease();
	return ret;
}

GameVertex::~GameVertex()
{
}

bool GameVertex::init()
{
	if (!PARENT_CLASS::initWithCallback(nullptr))
		return false;

	setAnchorPoint(Point::ANCHOR_MIDDLE);
	setContentSize({ 64, 64 });
	{
		auto node = LayerColor::create(Color4B::YELLOW, 36.f, 36.f);
		setGlowImage(node);
	}

	{
		auto node = LayerColor::create(Color4B::GRAY, 32.f, 32.f);
		node->setAnchorPoint(Point::ANCHOR_MIDDLE);
		node->setIgnoreAnchorPointForPosition(false);
		node->setPosition(_contentSize / 2);
		addChild(node);
	}

	return true;
}

void GameVertex::setGlow(bool glow)
{
	if (m_is_glowing == glow)
		return;

	m_is_glowing = glow;

	if (m_glow_image)
		m_glow_image->setVisible(m_is_glowing);
}

void GameVertex::setGlowImage(Node* image)
{
	if (image == m_glow_image)
		return;

	if (image)
	{
		addChild(image);
		image->setAnchorPoint(Point::ANCHOR_MIDDLE);
		image->setIgnoreAnchorPointForPosition(false);
		image->setPosition(_contentSize / 2);
	}

	if (m_glow_image)
		removeChild(m_glow_image, true);

	m_glow_image = image;

	if (m_glow_image)
		m_glow_image->setVisible(m_is_glowing);
}
