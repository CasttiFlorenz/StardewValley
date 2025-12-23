#include "StoneItem.h"

StoneItem* StoneItem::create(const cocos2d::Vec2& tileCoord) {
    // 工厂方法：安全创建并自动管理生命周期
    auto p = new (std::nothrow) StoneItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool StoneItem::init(const cocos2d::Vec2& tileCoord) {
    // 先初始化基类，绑定类型与瓦片坐标
    if (!EnvironmentItem::init(EnvironmentItemType::STONE, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Stone.png");

    return true;
}
