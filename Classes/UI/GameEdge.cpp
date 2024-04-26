
#include "GameEdge.h"

// UI component

const float GameEdge::width = 6;

GameEdge* GameEdge::create(const GraphConfig* config, const tID id1, const tID id2)
{
	auto ret = new (std::nothrow) GameEdge();
	ret->init(config, id1, id2);
	ret->autorelease();
	return ret;
}

GameEdge::~GameEdge()
{
}

bool GameEdge::init(const GraphConfig* config, const tID id1, const tID id2)
{
	if (!PARENT_CLASS::initWithColor(Color4B::GRAY))
		return false;

	setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	setIgnoreAnchorPointForPosition(false);

	auto [x1, y1] = config->getPosition(id1);
	auto [x2, y2] = config->getPosition(id2);
	
	setPosition(x1, y1);

	const auto diff = Point(x2-x1, y2-y1);

	setContentSize({ diff.length(), width });

	float angle_rad = diff.getAngle();
	float angle_deg = CC_RADIANS_TO_DEGREES(angle_rad);
	
	setRotation(-angle_deg);

	return true;
}
