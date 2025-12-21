#include "CopperItem.h"

CopperItem* CopperItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) CopperItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool CopperItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::COPPER, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Copper.png");

    return true;
}
