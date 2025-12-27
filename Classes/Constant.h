#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

// [Section 1] 基础全局设置 (Global Settings)

// 地图与物理
constexpr float TILED_MAP_SCALE = 5.0f;
constexpr float DEFAULT_SPEED = 250.0f;
constexpr int CAMERA_POSZ = 200;

// 农舍尺寸
constexpr int FARM_HOUSE_WIDTH = 20;
constexpr int FARM_HOUSE_HEIGHT = 12;

// 初始数值
constexpr int INITIAL_PLAYER_MONEY = 500;

// Tag 标识
constexpr int FISHING_GAME_TAG = 8998;
constexpr int SHOP_MENU_TAG = 9999;
constexpr int TAG_DIALOGUE_LAYER = 9998;

// 技能水平显示
constexpr char* LEVEL_FILLED1 = "/Player/red block.png";
constexpr char* LEVEL_EMPTY1 = "/Player/empty block.png";
constexpr char* LEVEL_FILLED2 = "/Player/orange block.png";
constexpr char* LEVEL_EMPTY2 = "/Player/long block.png";
constexpr int MAX_LEVEL = 100;    // 最高经验值
constexpr int SKILL_COUNT = 4;    // 技能个数

// 好感度显示
constexpr char* HEART_FILLED = "/NPC/red heart.png";
constexpr char* HEART_EMPTY = "/NPC/empty heart.png";

// 背包常量
constexpr int INVENTORY_SIZE = 36;    // 背包总大小
constexpr int ITEM_TAG_BASE = 1000;   // 物品精灵的tag
constexpr int COUNT_TAG_BASE = 2000;  // 数量标签的tag
constexpr int BUTTON_TAG_BASE = 3000; // 背包按钮的tag

// [Section 2] 枚举定义 (Enums)

// --- 基础状态 ---
enum class Direction {
    DOWN = 0, RIGHT = 1, UP = 2, LEFT = 3
};

enum class Season {
    Spring = 0, Summer, Fall, Winter
};

enum class WeatherType {
    Sunny, Rain, Snow, Wind
};

enum class DayOfWeek {
    Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
};

// --- 地图与环境 ---
enum class MapType {
    NONE = 0, FARM_HOUSE, FARM, BARN, MINES, TOWN
};

enum class SoilStatus {
    DRY, WET
};

enum class CropStatus {
    SEEDS, GROWING, MATURE, DEAD
};

enum class EnvironmentItemType {
    NONE, WOOD, GRASS, STONE, COPPER, LEEK, DAFFODILS, DRY, WET, CULTIVATED_SOIL
};

enum class FarmItemType {
    CULTIVATED
};

// --- 角色与生物 ---
enum class CharacterType {
    Evelyn = 0, Haley, Sam, CHARACTER_COUNT
};

enum class AnimalType {
    CHICKEN, COW
};

enum class SkillType {
    FARMING = 0, MINING, FORAGING, FISHING, SKILL_COUNT
};

// --- 物品类型 ---
enum class ItemType {
    NONE = -1,
    // 工具
    HOE = 0,            // 锄头
    AXE,                // 斧头
    WATERING_CAN,       // 浇水壶
    PICKAXE,            // 镐
    SCYTHE,             // 镰刀
    FISHINGROD,         // 钓鱼竿
    // 资源
    STONE,              // 石头
    WOOD,               // 木头
    COPPER_ORE,         // 铜矿石
    FIBER,              // 纤维
    HAY,                // 干草
    FERTILIZER,         // 肥料
    // 种子
    PARSNIP_SEED,       // 防风草种子
    CAULIFLOWER_SEED,   // 花椰菜种子
    POTATO_SEED,        // 土豆种子
    // 农作物/采集物
    PARSNIP,            // 防风草
    CAULIFLOWER,        // 花椰菜
    POTATO,             // 土豆
    DAFFODILS,          // 黄水仙
    LEEK,               // 韭葱
    // 产物/食物
    EGG,                // 鸡蛋
    FRIED_EGG,          // 煎鸡蛋
    MILK,               // 牛奶
    CARP,               // 鲤鱼
    SALAD,              // 田园沙拉

    COUNT,              // 总数

    // 特殊 ID：不进背包
    ANIMAL_CHICKEN_TAG = 10001,
    ANIMAL_COW_TAG = 10002,
};

// --- 输入与交互 ---
enum class KeyBoardEvent {
    NONE = 0, CHANGE_INVENTORY
};

enum class MouseEvent {
    NONE = 0,
    // 交互
    NPC_CONVERSATION,
    USE_TOOL,
    SLEEP,
    FISHING,

    // 采集/获取
    GET_WOOD, GET_GRASS, GET_STONE, GET_COPPER,
    GET_DAFFODILS, GET_LEEK,
    GET_PARSNIP, GET_POTATO, GET_CAULIFLOWER,
    GET_EGG, GET_MILK,

    // 商店交互
    SHOP_SALE, SHOP_PIERRE, SHOP_MARNIE,

    // 对话触发
    CONVERSATION_HALEY, CONVERSATION_SAM, CONVERSATION_EVELYN,

    // 使用物品
    USE_PARSNIP_SEED, USE_POTATO_SEED, USE_CAULIFLOWER_SEED, USE_HAY
};


// [Section 3] 资源路径 (Resource Paths)

// 天气
const std::string PATH_IMG_FLOWER = "Weather/flower.png";
const std::string PATH_IMG_LEAF = "Weather/leaf.png";
const std::string PATH_IMG_RAINDROP = "Weather/raindrop.png";
const std::string PATH_IMG_FALLBACK_KEY = "fallback_white_box";

// UI - 时钟与天气图标
const std::string PATH_UI_CLOCK_BG = "Clock/Clock.png";
const std::string PATH_UI_CLOCK_HAND = "Clock/hand.png";
const std::string PATH_UI_WEATHER_SUNNY = "Clock/sunny.png";
const std::string PATH_UI_WEATHER_RAINY = "Clock/rainy.png";
const std::string PATH_UI_WEATHER_SNOWY = "Clock/snowy.png";
const std::string PATH_UI_WEATHER_WINDY_SPRING = "Clock/windy_spring.png";
const std::string PATH_UI_WEATHER_WINDY_AUTUMN = "Clock/windy_autumn.png";
const std::string PATH_UI_SLEEP_BG = "CreateScene/Background-1.png";

// UI - 商店
const std::string PATH_SHOP_BG = "Shop/background.png";
const std::string PATH_SHOP_MINUS = "Shop/minus.png";
const std::string PATH_SHOP_PLUS = "Shop/plus.png";
const std::string PATH_SHOP_BUY = "Shop/buy.png";
const std::string PATH_SHOP_CLOSE = "Shop/close.png";
const std::string PATH_SHOP_MENU_PREFIX = "Shop/Shop_";

// UI - 背包
const std::string PATH_INVENTORY_SLOT_BG = "Items/inventory.jpg";

// UI - 睡觉
const std::string PATH_UI_YES = "Shop/ok.png";
const std::string PATH_UI_NO = "Shop/no.png";
const std::string PATH_UI_SELECT = "Shop/SelectDialogue.png";

// UI-对话
const std::string PATH_DIALOGUE_BG_DEFAULT = "Shop/dialogue.png";
const std::string PATH_DIALOGUE_PREFIX = "Shop/Dialogue_";

// NPC
const std::string NPC_NAME_HALEY = "Haley";
const std::string NPC_NAME_SAM = "Sam";
const std::string NPC_NAME_EVELYN = "Evelyn";
const std::string SHOP_NAME_PIERRE = "Pierre";
const std::string SHOP_NAME_MARNIE = "Marnie";
const std::string PATH_NPC_SAM = "NPC/Sam.png";
const std::string PATH_NPC_EVELYN = "NPC/Evelyn.png";
const std::string PATH_NPC_HALEY = "NPC/Haley.png";

// 字体
const std::string PATH_FONT_ARIAL = "fonts/arial.ttf";
const std::string PATH_FONT_PIXEL = "fonts/pixel.ttf";
const std::string FONT_DEFAULT_SYSTEM = "Arial";


// [Section 4] 逻辑常量与字符串 (Logic Constants)

// 场景切换标识
const std::string GO_TO_FARM = "goToFarm";
const std::string GO_TO_TOWN = "goToTown";
const std::string GO_TO_BARN = "goToBarn";
const std::string GO_TO_MINES = "goToMines";
const std::string GO_TO_HOUSE = "goToHouse";

// 显示文本
const std::string STR_TIME_AM = "am";
const std::string STR_TIME_PM = "pm";
const std::string STR_SEASON_SPRING = "Spring";
const std::string STR_SEASON_SUMMER = "Summer";
const std::string STR_SEASON_FALL = "Fall";
const std::string STR_SEASON_WINTER = "Winter";
const std::string STR_UNKNOWN = "Unknown";
const std::vector<std::string> STR_DAYS_OF_WEEK = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };


// [Section 5] 数值参数配置 (Configuration Parameters)

// --- 渲染层级 (ZOrder) ---
constexpr int ZORDER_SHOP_BG = 0;
constexpr int ZORDER_SHOP_CONTENT = 5;
constexpr int ZORDER_MAP_OBJECTS = 20;
constexpr int ZORDER_UI_LABEL = 10;
constexpr int ZORDER_SHOP_BUTTONS = 10;
constexpr int ZORDER_UI_WEATHER = 15;
constexpr int ZORDER_UI_HAND = 20;
constexpr int ZORDER_UI_CONTAINER = 100;
constexpr int ZORDER_SLEEP_MASK = 10000;
constexpr int ZORDER_SHOP_MENU = 999;

// --- 资源采集限制 ---
constexpr int MAX_WOOD_COUNT = 15;
constexpr int MAX_GRASS_COUNT = 15;
constexpr int MAX_STONE_COUNT = 15;
constexpr int MAX_COPPER_COUNT = 5;
constexpr int MAX_DAFFODILS_COUNT = 5;
constexpr int MAX_LEEK_COUNT = 3;

// --- 天气管理器参数 ---
constexpr float PARTICLE_LIFE_DEFAULT = 4.0f;
constexpr float PARTICLE_LIFE_VAR = 1.0f;
constexpr float RAIN_LIFE = 1.2f;
constexpr float RAIN_LIFE_VAR = 0.3f;
constexpr int CHANCE_RAIN_LOW = 15;
constexpr int CHANCE_WIND_LOW = 25;
constexpr int CHANCE_WIND_FALL = 35;
constexpr int CHANCE_SNOW = 20;
constexpr int MAX_NIGHT_OPACITY = 200;
constexpr int FALLBACK_TEXTURE_SIZE = 8;

// --- 时间管理器参数 ---
constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY_STD = 24;
constexpr int MAX_GAME_HOUR = 28;
constexpr int DAYS_PER_MONTH = 28;
constexpr int START_YEAR = 1;
constexpr int START_DAY = 1;
constexpr int START_HOUR = 6;
constexpr float TIME_UPDATE_INTERVAL = 0.7f;
constexpr int TIME_PASS_OUT_HOUR = 26;
constexpr float TIME_PASS_OUT_DELAY = 3.0f;
constexpr float ROTATION_PER_MINUTE = 0.15f;
constexpr float ROTATION_BASE_ANGLE = 180.0f;

// --- 商店参数 ---
constexpr int SHOP_MAX_QUANTITY_LIMIT = 99;
constexpr int SHOP_MIN_QUANTITY = 1;
constexpr float SHOP_BG_SCALE = 1.4f;
constexpr float SHOP_ICON_SCALE = 6.5f;
constexpr float SHOP_BTN_SCALE = 3.0f;
constexpr float SHOP_MENU_WIDTH = 800.0f;
constexpr float SHOP_MENU_HEIGHT = 600.0f;
constexpr float SCROLL_SENSITIVITY = 30.0f;

// --- 背包 UI 参数 ---
constexpr int INVENTORY_ROWS = 3;
constexpr int INVENTORY_COLS = 12;
constexpr int INVENTORY_TOTAL_SLOTS = 36;
constexpr float INV_GRID_CENTER_X = 400.0f;
constexpr float INV_GRID_START_Y = 220.0f;
constexpr float INV_GRID_GAP_Y = 75.0f;
constexpr float INV_SLOT_BASE_SIZE = 43.0f;
constexpr float INV_SLOT_SCALE = 1.35f;
constexpr float INV_BG_SCALE = 1.45f;
constexpr float INV_SLOT_TOUCH_SIZE = 50.0f;


// [Section 6] 颜色常量 (Colors)

const cocos2d::Color4B COLOR_MASK_DARK(0, 0, 0, 180);
const cocos2d::Color4B COLOR_ROW_EVEN(255, 255, 255, 50);
const cocos2d::Color4B COLOR_ROW_ODD(0, 0, 0, 20);
const cocos2d::Color4F COLOR_HIGHLIGHT_RED(0.85f, 0.18f, 0.18f, 1.0f);
const cocos2d::Color3B COLOR_NAME_TEXT(100, 50, 20);
const cocos2d::Color3B COLOR_CONTENT_TEXT(0, 0, 0);
const cocos2d::Color4B COLOR_BTN_BG_YES(0, 128, 0, 200);
const cocos2d::Color4B COLOR_BTN_BG_NO(200, 0, 0, 200);

// ==================== UI 尺寸与坐标 ====================
const cocos2d::Size SIZE_DIALOGUE_BG(800, 200);
const cocos2d::Size SIZE_DIALOGUE_BG_EXPANDED(1000, 200);
const cocos2d::Size SIZE_CHOICE_BTN(100, 50);

const cocos2d::Vec2 POS_BG_OFFSET(0, 150);
const cocos2d::Vec2 POS_NAME_LABEL(100, 160);
const cocos2d::Vec2 POS_CONTENT_LABEL(400, 80);
const cocos2d::Vec2 POS_BTN_YES(600, 50);
const cocos2d::Vec2 POS_BTN_NO(700, 50);

// ==================== 文本设置 ====================
constexpr int FONT_SIZE_NAME = 28;
constexpr int FONT_SIZE_CONTENT = 24;
constexpr int FONT_SIZE_BTN = 30;


#endif // __CONSTANT_H__

