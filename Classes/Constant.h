#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include "cocos2d.h"
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <map>
#include <new>
#include <algorithm>
#include <functional>

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
constexpr int ITEM_TAG_BASE = 1000;
constexpr int COUNT_TAG_BASE = 2000;
constexpr int BUTTON_TAG_BASE = 3000;
constexpr int PREVIEW_TAG_BASE = 999;
constexpr int SLEEPING_TAG = 9997;


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

enum class MouseEvent
{
    NONE = 0,
    NPC_CONSERVATION,
    USE_TOOL,
    OPEN_SHOP,
    SLEEP,
    FISHING
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
const std::string PATH_INVENTORY_BG = "Items/background.png";
const std::string PATH_INVENTORY_SLOT_BG = "Items/inventory.jpg";
const std::string PATH_PREVIEW_BOX = "/Items/box.png";
const std::string BUTTON_IMAGES[4] = {     // 按钮图片
    "/Items/button/inventory button.png",
    "/Items/button/skill button.png",
    "/Items/button/social button.png",
    "/Items/button/exit button.png"
};
// 技能水平显示
const std::string LEVEL_FILLED1 = "/Player/red block.png";
const std::string LEVEL_EMPTY1 = "/Player/empty block.png";
const std::string LEVEL_FILLED2 = "/Player/orange block.png";
const std::string LEVEL_EMPTY2 = "/Player/long block.png";

// 好感度显示
const std::string HEART_FILLED = "/NPC/red heart.png";
const std::string HEART_EMPTY = "/NPC/empty heart.png";


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
const std::string FONT_DEFAULT_SYSTEM = "Arial";
const std::string PATH_FONT_ARIAL = "fonts/arial.ttf";
const std::string PATH_FONT_PIXEL = "fonts/pixel.ttf";
const std::string PATH_FONT_LOUIS = "fonts/Louis George Cafe Bold.ttf";
const std::string PATH_FONT_HANDRON = "fonts/Handron-Filled.otf";


// [Section 4] 逻辑常量与字符串 (Logic Constants)

// 场景切换标识
const std::string GAME_START = "start";
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
constexpr int INVENTORT_BUTTON_COUNT = 4;
constexpr int INVENTORT_TOOL_COUNT = 5;
constexpr float INV_GRID_CENTER_X = 400.0f;
constexpr float INV_GRID_START_Y = 220.0f;
constexpr float INV_GRID_GAP_Y = 75.0f;
constexpr float INV_SLOT_BASE_SIZE = 43.0f;
constexpr float INV_SLOT_SCALE = 1.35f;
constexpr float INV_BG_SCALE = 1.45f;
constexpr float INV_SLOT_TOUCH_SIZE = 50.0f;

constexpr int MAX_LEVEL = 100;    // 最高经验值
constexpr int SKILL_COUNT = 4;    // 技能个数
constexpr int TOTAL_HEARTS = 10;  // 好感度显示总个数


// [Section 6] 颜色常量 (Colors)

const cocos2d::Color4B COLOR_MASK_DARK(0, 0, 0, 180);
const cocos2d::Color4B COLOR_ROW_EVEN(255, 255, 255, 50);
const cocos2d::Color4B COLOR_ROW_ODD(0, 0, 0, 20);
const cocos2d::Color4F COLOR_HIGHLIGHT_RED(0.85f, 0.18f, 0.18f, 1.0f);
const cocos2d::Color3B COLOR_NAME_TEXT(100, 50, 20);
const cocos2d::Color3B COLOR_CONTENT_TEXT(0, 0, 0);
const cocos2d::Color4B COLOR_BTN_BG_YES(0, 128, 0, 200);
const cocos2d::Color4B COLOR_BTN_BG_NO(200, 0, 0, 200);
const cocos2d::Color4B COLOR_SKILL_LEVEL_NUM(210, 105, 30, 255);

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

// ==================== 地图路径相关 ====================

// 谷仓地图路径
const std::string TILED_MAP_BARN_PATH = "/TiledMap/Barn/Barn.tmx";

// 农场地图路径
const std::string TILED_MAP_FARM_PATH = "TiledMap/Farm/Farm.tmx";

// 农舍地图路径
const std::string TILED_MAP_FARM_HOUSE_PATH = "TiledMap/FarmHouse/FarmHouse.tmx";

// 矿洞地图路径
const std::string TILED_MAP_MINES_PATH = "/TiledMap/Mines/Mines.tmx";

// 城镇地图路径
const std::string TILED_MAP_TOWN_PATH = "TiledMap/Town/Town.tmx";


// Tiled 图层 / 属性 / 对象通用名称

// 事件层名称
const std::string EVENT_LAYER_NAME = "event";

// 可碰撞属性名
const std::string COLLIDABLE_PROPERTY_NAME = "Collidable";

// 钓鱼属性名
const std::string FISHING_PROPERTY_NAME = "Fishing";

// 已耕种属性名
const std::string CULTIVATED_PROPERTY_NAME = "Cultivated";

// 石头属性名
const std::string STONE_PROPERTY_NAME = "Stone";

// 对象属性字段
const std::string OBJECT_PROPERTY_X = "x";
const std::string OBJECT_PROPERTY_Y = "y";
const std::string OBJECT_PROPERTY_WIDTH = "width";
const std::string OBJECT_PROPERTY_HEIGHT = "height";

// true 的字符串表示（Tiled 中常见）
const std::string TRUE_STRING_VALUE_1 = "true";
const std::string TRUE_STRING_VALUE_2 = "1";

// true 的整数表示
constexpr float TRUE_INTEGER_VALUE = 1;


// ==================== 玩家初始位置 / 偏移 ====================

// 玩家默认出生坐标
constexpr float PLAYER_DEFAULT_POS_X = 0;
constexpr float PLAYER_DEFAULT_POS_Y = 0;

// Y 轴偏移值
constexpr int Y_OFFSET_1 = 1;
constexpr int Y_OFFSET_0 = 0;
constexpr int Y_OFFSET_NEG_1 = -1;


// ==================== 通用数量 / 计数 ====================

// 通用数量 1
constexpr float ITEM_COUNT_1 = 1;

// 干草数量
constexpr float HAY_ITEM_COUNT = 1;

// 牛奶数量
constexpr float MILK_ITEM_COUNT = 1;

// 鸡蛋数量
constexpr float EGG_ITEM_COUNT = 1;

// ==================== 商店 / UI（农场 & 城镇）====================

// 售卖对象名
const std::string SALE_OBJECT_NAME = "sale";

// 商店标题
const std::string SHOP_TITLE_SALE = "Sale";

// 商店菜单层级
constexpr float SHOP_MENU_Z_ORDER = 999;


// 农舍交互对象
// 返回农场对象名
const std::string GO_TO_FARM_OBJECT_NAME = "goToFarm";

// 上床睡觉对象名
const std::string GO_TO_BED_OBJECT_NAME = "goToBed";

// 物品最大堆叠数量
constexpr float ITEM_STACK_SIZE_3 = 3;
constexpr float ITEM_STACK_SIZE_1 = 1;

// 物品缩放
const float ITEM_SCALE_1_0 = 1.0f;
const float ITEM_SCALE_0_0 = 0.0f;


// 商店物品贴图路径
const std::string ITEM_TEXTURE_PARSNIP_SEED = "Items/parsnip seed.png";
const std::string ITEM_TEXTURE_POTATO_SEED = "Items/potato seed.png";
const std::string ITEM_TEXTURE_CAULIFLOWER_SEED = "Items/cauliflower seed.png";
const std::string ITEM_TEXTURE_FERTILIZER = "Items/fertilizer.png";
const std::string ITEM_TEXTURE_SALAD = "Items/salad.png";
const std::string ITEM_TEXTURE_HAY = "Items/hay.png";
const std::string ITEM_TEXTURE_CHICKEN = "Items/chicken.png";
const std::string ITEM_TEXTURE_COW = "Items/cow.png";

// 商店物品价格
constexpr float ITEM_PRICE_PARSNIP_SEED = 20;
constexpr float ITEM_PRICE_POTATO_SEED = 50;
constexpr float ITEM_PRICE_CAULIFLOWER_SEED = 80;
constexpr float ITEM_PRICE_FERTILIZER = 100;
constexpr float ITEM_PRICE_SALAD = 220;
constexpr float ITEM_PRICE_HAY = 50;
constexpr float ITEM_PRICE_CHICKEN = 300;
constexpr float ITEM_PRICE_COW = 500;

// 商店物品名称
const std::string ITEM_NAME_PARSNIP_SEEDS = "Parsnip Seeds";
const std::string ITEM_NAME_POTATO_SEEDS = "Potato Seeds";
const std::string ITEM_NAME_CAULIFLOWER_SEEDS = "Cauliflower Seeds";
const std::string ITEM_NAME_FERTILIZER = "Fertilizer";
const std::string ITEM_NAME_SALAD = "Salad";
const std::string ITEM_NAME_HAY = "Hay";
const std::string ITEM_NAME_CHICKEN = "Chicken";
const std::string ITEM_NAME_COW = "Cow";


// ==================== 动物系统====================
// 喂食层
const std::string FEED_LAYER_NAME = "feeding";

// 巢 / 产出对象前缀
const std::string NEST_OBJECT_PREFIX = "nest";
const std::string PRODUCTION_OBJECT_PREFIX = "production";

// 最大动物数量
constexpr float MAX_ANIMAL_COUNT = 5;

// 巢偏移计算除数
constexpr float NEST_OFFSET_DIVISOR = 6;

// Z Order
constexpr float HAY_SPRITE_Z_ORDER = 50;
constexpr float ANIMAL_SPRITE_Z_ORDER = 60;
constexpr float PRODUCTION_SPRITE_Z_ORDER = 70;


// 鸡（Chicken）
const std::string CHICKEN_DEFAULT_TEXTURE_PATH = "Animal/chicken_0.png";
const std::string CHICKEN_PRODUCE_TEXTURE_PATH = "EnvironmentObjects/egg.png";

const std::string CHICKEN_ANIM_FRAME_0 = "Animal/chicken_0.png";
const std::string CHICKEN_ANIM_FRAME_1 = "Animal/chicken_1.png";
const std::string CHICKEN_ANIM_FRAME_2 = "Animal/chicken_2.png";
const std::string CHICKEN_ANIM_FRAME_3 = "Animal/chicken_3.png";

// 动画参数（鸡 & 牛共用）
constexpr float ANIMATION_INTERVAL = 0.3f;
constexpr float ANIM_FRAME_0_UNITS = 3.0f / ANIMATION_INTERVAL;
constexpr float ANIM_FRAME_2_UNITS = 2.0f / ANIMATION_INTERVAL;
constexpr float ANIM_FRAME_4_UNITS = 2.0f / ANIMATION_INTERVAL;
constexpr float ANIM_FRAME_OTHER_UNITS = 1.0f;
constexpr float   ANIMATION_LOOP_COUNT = 1;


// 牛（Cow）
const std::string COW_DEFAULT_TEXTURE_PATH = "Animal/cow_0.png";
const std::string COW_PRODUCE_TEXTURE_PATH = "EnvironmentObjects/milk.png";

const std::string COW_ANIM_FRAME_0 = "Animal/cow_0.png";
const std::string COW_ANIM_FRAME_1 = "Animal/cow_1.png";
const std::string COW_ANIM_FRAME_2 = "Animal/cow_2.png";
const std::string COW_ANIM_FRAME_3 = "Animal/cow_3.png";
const std::string COW_ANIM_FRAME_4 = "Animal/cow_4.png";
const std::string COW_ANIM_FRAME_5 = "Animal/cow_5.png";


// 农作物（Crop）
const std::string CROP_DEAD_TEXTURE_PATH = "EnvironmentObjects/dead.png";
const std::string CROP_TEXTURE_BASE_PATH_PREFIX = "EnvironmentObjects/";
const std::string CROP_TEXTURE_STAGE_SUFFIX = ".png";
const std::string CROP_TEXTURE_PARSNIP_PREFIX = "Parsnip";
const std::string CROP_TEXTURE_POTATO_PREFIX = "Potato";
const std::string CROP_TEXTURE_CAULIFLOWER_PREFIX = "Cauliflower";
constexpr float PARSNIP_MAX_GROWTH_STAGE = 3;
constexpr float POTATO_MAX_GROWTH_STAGE = 4;
constexpr float CAULIFLOWER_MAX_GROWTH_STAGE = 4;
constexpr float INITIAL_GROWTH_STAGE = 0;


// 土壤（Soil）
const std::string SOIL_DRY_TEXTURE_PATH = "EnvironmentObjects/Dry.png";
const std::string SOIL_WET_TEXTURE_PATH = "EnvironmentObjects/Wet.png";
const float CROP_POSITION_X_SCALE = 0.5f;
const float CROP_POSITION_Y_NUMERATOR = 2.0f;
const float CROP_POSITION_Y_DENOMINATOR = 3.0f;
constexpr float SOIL_SPRITE_Z_ORDER = 100;


// 掉落物 / 资源
const std::string HAY_SPRITE_TEXTURE_PATH = "EnvironmentObjects/Hay.png";
const std::string COPPER_ITEM_TEXTURE_PATH = "EnvironmentObjects/Copper.png";
const std::string DAFFODILS_ITEM_TEXTURE_PATH = "EnvironmentObjects/Daffodils.png";
const std::string GRASS_ITEM_TEXTURE_PATH = "EnvironmentObjects/Grass.png";
const std::string LEEK_ITEM_TEXTURE_PATH = "EnvironmentObjects/Leek.png";
const std::string STONE_ITEM_TEXTURE_PATH = "EnvironmentObjects/Stone.png";
const std::string WOOD_ITEM_TEXTURE_PATH = "EnvironmentObjects/Wood.png";


// Tile / GID / 坐标编码
constexpr long long TILE_COORD_MASK = 0xffffffffLL;

constexpr unsigned int TMX_FLIP_FLAGS_MASK =
0x80000000 | 0x40000000 | 0x20000000;

const unsigned int INVALID_TILE_GID = 0;

// Tile 计算用
constexpr float TILE_SIZE_HALF_DIVISOR = 2;


// 掉落生成 / 随机
constexpr float ITEM_SPRITE_Z_ORDER = 5;
constexpr float MAX_SPAWN_ATTEMPTS = 1000;
constexpr float RANDOM_TYPE_THRESHOLD = 0;


// ==================== 渲染层级 / 相机 / UI ====================
constexpr float MAP_Z_ORDER = 0;
constexpr float PLAYER_Z_ORDER = 4;
constexpr float WEATHER_MANAGER_Z_ORDER = 5;
constexpr float TIME_MANAGER_Z_ORDER = 6;
constexpr float FOLLOW_CAMERA_Z_ORDER = 6;
constexpr float INVENTORY_Z_ORDER = 7;
constexpr float FISHING_GAME_Z_ORDER = 10;
constexpr float SLEEP_DIALOG_Z_ORDER = 9999;

// ==================== 相机参数 ====================
const float CAMERA_NEAR_PLANE = 1.0f;
const float CAMERA_FAR_PLANE = 1000.0f;

// ==================== 钓鱼 / 睡觉对话框 ====================
const std::string SLEEP_DIALOG_NAME = "goToBed";
const std::string DIALOGUE_LAYER_NAME = "DialogueLayer";
const std::string SELECT_DIALOGUE_TEXTURE_PATH = "Shop/SelectDialogue.png";
const std::string OK_BUTTON_TEXTURE_PATH = "Shop/ok.png";
const std::string NO_BUTTON_TEXTURE_PATH = "Shop/no.png";
const std::string PIXEL_FONT_PATH = "fonts/pixel.ttf";
const std::string SLEEP_DIALOG_TEXT = "Do you want to sleep now?";
constexpr int   FONT_SIZE = 10;
constexpr float DIALOGUE_SCALE = 3.0f;
constexpr float BUTTON_SCALE = 0.45f;
constexpr float DIALOGUE_POS_Y = 150.0f;
constexpr float LABEL_POS_Y_RATIO = 0.6f;
constexpr float OK_BUTTON_POS_X_RATIO = 0.3f;
constexpr float NO_BUTTON_POS_X_RATIO = 0.7f;
constexpr float BUTTON_POS_Y_RATIO = 0.3f;

#endif // __CONSTANT_H__

