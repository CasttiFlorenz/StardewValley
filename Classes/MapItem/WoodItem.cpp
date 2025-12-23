#include "WoodItem.h"

WoodItem* WoodItem::create(const cocos2d::Vec2& tileCoord) {
    // 工厂方法：安全创建并自动管理生命周期
    auto p = new (std::nothrow) WoodItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool WoodItem::init(const cocos2d::Vec2& tileCoord) {
    // 先初始化基类，绑定类型与瓦片坐标
    if (!EnvironmentItem::init(EnvironmentItemType::WOOD, tileCoord)) {
        return false;
    }

    // 使用精灵的文件名设置纹理（保持与项目其他环境物体一致）
    this->setTexture("EnvironmentObjects/Wood.png");

    return true;
}
