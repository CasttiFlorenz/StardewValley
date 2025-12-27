#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 所有地图场景的基类
// 负责：地图加载、坐标转换、碰撞检测、地图切换、基础交互接口
class GameMap : public Scene
{
public:
     // 初始化地图（子类必须实现）
    virtual bool init() = 0;

    // 每帧更新（子类可选实现）
    virtual void update(float dt) {}

    // 进入地图时的初始化处理（位置、缩放等）
    virtual void IntoMap(MapType lastMap) = 0;

     // 获取当前地图类型
    MapType getMapName() const { return _mapName; }

    // 获取 Tiled 地图对象
    TMXTiledMap* getTiledMap() const { return _map; }

    // 是否启用摄像机跟随
    virtual bool isCameraFollow() const { return false; }

    // 是否为室外地图（影响天气、时间系统等）
    virtual bool isOutdoor() { return false; }

     // 获取玩家在本地图的起始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) = 0;

    // 根据玩家位置和朝向判断是否需要切换地图
    virtual MapType leaveMap(const Vec2& curPos,
        bool isStart,
        const Direction& direction) = 0;

     // 判断世界坐标是否可通行
    virtual bool isCollidable(Vec2 worldPos);

    // 世界坐标 → 瓦片坐标
    Vec2 calMapPos(Vec2 worldPos);

    // 瓦片坐标 → 世界坐标
    Vec2 calWorldPos(const Vec2& tileCoord);

    // 从 TMX 对象层中获取指定对象的矩形区域
    Rect getObjectRect(const std::string& objectName);

     // 鼠标左键交互（默认使用工具）
    virtual MouseEvent onLeftClick(const Vec2& playerPos,
        const Direction direction,
        ItemType objects)
    {
        return MouseEvent::USE_TOOL;
    }

    // 鼠标右键交互（默认无行为）
    virtual MouseEvent onRightClick(const Vec2& playerPos,
        const Direction direction)
    {
        return MouseEvent::NONE;
    }

     // 根据玩家朝向，对瓦片坐标进行偏移（前方一格）
    virtual void ApplyDirectionOffset(Vec2& basePos, Direction direction);

    // 判断 TMX 属性值是否为 true（兼容多种类型）
    virtual bool IsTrueProperty(const Value& v);

protected:
    TMXTiledMap* _map = nullptr;   // 当前地图的 Tiled 对象
    MapType _mapName;              // 当前地图类型
};

#endif // __GAME_MAP_H__
