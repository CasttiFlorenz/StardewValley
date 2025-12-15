#ifndef __CONSTANT_H__
#define __CONSTANT_H__
#include <map>
#include <string>


USING_NS_CC;

// 地图缩放比例
constexpr float TILED_MAP_SCALE = 5.0f;
// 玩家默认速度
constexpr float DEFAULT_SPEED = 250.0f;

// 农舍地图宽高（瓦片数）
constexpr int FARM_HOUSE_WIDTH = 20;
constexpr int FARM_HOUSE_HEIGHT = 12;

// 方向枚举
enum class Direction
{
    DOWN = 0,
    RIGHT = 1,
    UP = 2,
    LEFT = 3
};

enum class MouseEvent
{
    NONE=0,
    NPC_CONSERVATION,
    USE_TOOL,
    GET_WOOD,
    GET_GRASS
};

enum class KeyBoardEvent
{
    NONE = 0,
    CHANGE_INVENTORY
};

constexpr int CAMERA_POSZ = 200;

#endif // __CONSTANT_H__