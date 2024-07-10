#include "GameChip.h"

GameChip* GameChip::create(const Color3B& color)
{
	auto ret = new (std::nothrow) GameChip();
	ret->init(color);
	ret->autorelease();
	return ret;
}

bool GameChip::init(const Color3B& color)
{
	if (! PARENT_CLASS::initWithCallback(nullptr))
		return false;
	
	setAnchorPoint(Point::ANCHOR_MIDDLE);
	setIgnoreAnchorPointForPosition(false);
	setCascadeOpacityEnabled(true);

	m_selected_state = Sprite::create("Chip.png");
	m_selected_state->setAnchorPoint(Point::ANCHOR_MIDDLE);
	m_selected_state->setIgnoreAnchorPointForPosition(false);
	m_selected_state->setColor(Color3B::BLACK);
	m_selected_state->setVisible(m_is_selected);
	addChild(m_selected_state);

	setContentSize(m_selected_state->getContentSize());
	m_selected_state->setPosition(_contentSize / 2.f);

	{
		auto node = Sprite::create("Chip.png");
		node->setAnchorPoint(Point::ANCHOR_MIDDLE);
		node->setIgnoreAnchorPointForPosition(false);
		node->setScale(0.9f);
		node->setPosition(_contentSize / 2.f);
		node->setColor(color);
		addChild(node);
	}

	return true;
}

void GameChip::setSelected(bool selected)
{
	if (selected != m_is_selected)
	{
		m_is_selected = selected;
		m_selected_state->setVisible(m_is_selected);
	}
}