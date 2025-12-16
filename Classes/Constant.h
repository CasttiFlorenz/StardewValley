#ifndef __CONSTANT_H__
#define __CONSTANT_H__
#include <map>
#include <string>

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
    NONE = 0,
    NPC_CONSERVATION,
    USE_TOOL,
    GET_WOOD,
    GET_GRASS
};

enum class Objects {
    HOE = 0,           // 锄头
    AXE,              // 斧头
    WATERING_CAN,     // 浇水壶
    PICKAXE,          // 镐
    SCYTHE,           // 镰刀
    FISHINGROD,       // 钓鱼竿
    STONE,            // 石头
    WOOD,             // 木头
    COPPER_ORE,       // 铜矿石
    FERTILIZER,       // 肥料
    PARSNIP_SEED,     // 防风草种子 
    CAULIFLOWER_SEED, // 花椰菜种子
    POTATO_SEED,      // 土豆种子
    PARSNIP,          // 防风草
    CAULIFLOWER,      // 花椰菜
    POTATO,           // 土豆
    FIBER,            // 纤维
    DAFFODILS,        // 黄水仙
    LEEK,             // 韭葱
    HAY,              // 干草
    EGG,              // 鸡蛋
    FRIED_EGG,        // 煎鸡蛋
    CARP,             // 鲤鱼
    MILK,             // 牛奶
    SALAD,            // 田园沙拉
    COUNT             // 总数，用于遍历
};


enum class KeyBoardEvent
{
    NONE = 0,
    CHANGE_INVENTORY
};

constexpr int CAMERA_POSZ = 200;

enum class FarmItemType {
    NONE,
    WOOD,
    GRASS,
    CULTIUVATED
};

#endif // __CONSTANT_H__
