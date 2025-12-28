#include "CopperItem.h"

// 创建实例
CopperItem* CopperItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) CopperItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 初始化
bool CopperItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::COPPER, tileCoord)) {
        return false;
    }
    this->setTexture(COPPER_ITEM_TEXTURE_PATH);
    return true;
}