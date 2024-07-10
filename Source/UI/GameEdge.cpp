
#include "GameEdge.h"

// UI component

static constexpr float width = 6.f;

GameEdge* GameEdge::create(const GraphConfig* config, const tID id1, const tID id2)
{
	auto ret = new (std::nothrow) GameEdge();
	ret->init(config, id1, id2);
	ret->autorelease();
	return ret;
}

bool GameEdge::init(const GraphConfig* config, const tID id1, const tID id2)
{
	if (!PARENT_CLASS::initWithColor(Color4B::GRAY))
		return false;

	setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	setIgnoreAnchorPointForPosition(false);

	const auto [x1, y1] = config->getPosition(id1);
    const auto [x2, y2] = config->getPosition(id2);
	
	setPosition(x1, y1);

	const auto diff = Point(x2-x1, y2-y1);

	setContentSize({ diff.length(), width });

	const float angle_rad = diff.getAngle();
    const float angle_deg = AX_RADIANS_TO_DEGREES(angle_rad);
	
	setRotation(-angle_deg);

	return true;
}
