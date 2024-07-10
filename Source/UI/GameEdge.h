#pragma once

#include "cocos2d.h"
#include "Logic/Graph.h"

// UI component

USING_NS_CC;

class  GameEdge final : public LayerColor
{
    using PARENT_CLASS = LayerColor;

public:
    bool init(const GraphConfig* config, const tID id1, const tID id2);
    static GameEdge* create(const GraphConfig* config, const tID id1, const tID id2);
};

