/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SkillLevel.cpp
 * File Function: SkillLevel类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#include "SkillLevel.h"

USING_NS_CC;

// 初始化静态成员
SkillData SkillLevel::s_skillData[SKILL_COUNT];
SkillLevel* SkillLevel::_instance = nullptr;

// 获取单例
SkillLevel* SkillLevel::getInstance() {
    if (!_instance) {
        _instance = new SkillLevel();
    }
    return _instance;
}

// 销毁单例
void SkillLevel::destroyInstance()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

// 初始化技能数据
void SkillLevel::initSkillData()
{
    // 初始化技能数据
    s_skillData[static_cast<int>(SkillType::FARMING)] = { "Farming", 0, 4.0f,"/Items/corn.png" };
    s_skillData[static_cast<int>(SkillType::MINING)] = { "Mining", 0,1.2f, "/Items/pickaxe.png" };
    s_skillData[static_cast<int>(SkillType::FORAGING)] = { "Foraging", 0,4.0f, "/Items/cabbage.png" };
    s_skillData[static_cast<int>(SkillType::FISHING)] = { "Fishing", 0, 1.2f,"/Items/anchovy.png" };

}

// 设置技能等级
void SkillLevel::setSkillLevel(SkillType skillType, int level)
{
    if (static_cast<int>(skillType) >= 0 && static_cast<int>(skillType) < SKILL_COUNT) {
        s_skillData[static_cast<int>(skillType)].level = level;
    }
}

// 创建等级显示
Node* SkillLevel::createLevelStars(int level)
{
    Node* starContainer = Node::create();
    float spacing = 30.0f;
    float specialSpacing = spacing * 1.2f;

    // 总宽度计算
    float totalWidth = 4 * spacing + specialSpacing; // 前5个
    totalWidth += 4 * spacing + specialSpacing; // 后5个

    float startX = -totalWidth / 2 + spacing / 2;
    float currentX = startX;

    for (int i = 0; i < MAX_LEVEL / 10; i++) {
        bool isFilled = (i < level);

        // 选择类型
        const std::string filledPath = ((i + 1) % 5 == 0) ? LEVEL_FILLED2 : LEVEL_FILLED1;
        const std::string emptyPath = ((i + 1) % 5 == 0) ? LEVEL_EMPTY2 : LEVEL_EMPTY1;

        auto starSprite = Sprite::create(isFilled ? filledPath : emptyPath);
        starSprite->setPosition(Vec2(currentX, 0));
        starContainer->addChild(starSprite);

        // 更新位置
        if (i == 3 || i == 8 || ((i + 1) % 5 == 0))
            currentX += specialSpacing;
        else
            currentX += spacing;
    }
    return starContainer;
}

// 创建单个技能项
Node* SkillLevel::createSkillItem(SkillType skillType, const Vec2& position)
{
    if (static_cast<int>(skillType) < 0 || static_cast<int>(skillType) >= SKILL_COUNT) {
        return nullptr;
    }

    SkillData& skill = s_skillData[static_cast<int>(skillType)];
    int currentLevel = static_cast<int>(skill.level / 10);

    // 创建技能项容器
    Node* skillItem = Node::create();
    skillItem->setPosition(position);

    // ========== 打印技能名字（最左侧） ==========
    auto nameLabel = Label::createWithTTF(skill.name, PATH_FONT_LOUIS, 24);
    if (nameLabel) {
        nameLabel->setTextColor(Color4B::BLACK);
        nameLabel->setAnchorPoint(Vec2(0, 0.5f));  // 左对齐
        nameLabel->setPosition(Vec2(-430, 0));     // 最左边
        skillItem->addChild(nameLabel);
    }

    // ========== 打印技能图标（名字右侧） ==========
    auto skillIcon = Sprite::create(skill.iconPath);
   
    skillIcon->setScale(skill.scale);
    skillIcon->setPosition(Vec2(-300, 0));  // 名字右侧
    skillItem->addChild(skillIcon);

    // ========== 打印等级（图标右侧） ==========
    Node* starContainer = createLevelStars(currentLevel);
    if (starContainer) {
        starContainer->setPosition(Vec2(50, 0));  // 中间位置
        starContainer->setScale(2.0f);
        skillItem->addChild(starContainer);
    }

    // ========== 显示最终等级文字（最右侧） ==========
    auto levelText = Label::createWithTTF(std::to_string(currentLevel), PATH_FONT_HANDRON, 45);
    if (levelText) {
        levelText->setTextColor(COLOR_SKILL_LEVEL_NUM);
        levelText->setAnchorPoint(Vec2(0, 0.5f));  // 左对齐
        levelText->setPosition(Vec2(400, 0));      // 最右边
        skillItem->addChild(levelText);

    }
    return skillItem;
}

// 创建技能界面
Node* SkillLevel::createSkillsInterface(Node* parent, Sprite* background)
{
    if (!parent || !background) {
        return nullptr;
    }

    // 确保技能数据已初始化
    static bool isInitialized = false;
    if (!isInitialized) {
        initSkillData();
        isInitialized = true;
    }

    // 创建容器
    Node* container = Node::create();

    // 获取背景位置
    Vec2 bgPos = background->getPosition();

    // 计算起始位置（背景上部）
    float startY = bgPos.y + 140;
    float spacing = 90;  // 每个技能项的间距


    // 创建所有技能项
    for (int i = 0; i < SKILL_COUNT; i++) {
        SkillType skillType = static_cast<SkillType>(i);
        Vec2 position = Vec2(bgPos.x, startY - i * spacing);

        Node* skillItem = createSkillItem(skillType, position);
        if (skillItem) {
            container->addChild(skillItem);
        }
    }

    // 将容器添加到父节点
    parent->addChild(container, 100);

    return container;
}

// 增加技能水平
bool SkillLevel::increaseSkillLevel(SkillType skillType, int amount)
{
    if (static_cast<int>(skillType) < 0 || static_cast<int>(skillType) >= SKILL_COUNT || amount <= 0) {
        return false;
    }

    SkillData& skill = s_skillData[static_cast<int>(skillType)];
    int oldLevel = skill.level;

    // 计算新等级，不超过最大等级
    int newLevel = oldLevel + amount;
    if (newLevel > MAX_LEVEL) {
        newLevel = MAX_LEVEL;
    }

    // 如果等级有变化，更新并返回true
    if (newLevel != oldLevel) {
        skill.level = newLevel;
        return true;
    }

    return false;  // 等级没有变化
}

// 减少技能水平
bool SkillLevel::decreaseSkillLevel(SkillType skillType, int amount)
{
    if (static_cast<int>(skillType) < 0 || static_cast<int>(skillType) >= SKILL_COUNT || amount <= 0) {
        return false;
    }

    SkillData& skill = s_skillData[static_cast<int>(skillType)];
    int oldLevel = skill.level;

    // 计算新等级，不低于0
    int newLevel = oldLevel - amount;
    if (newLevel < 0) {
        newLevel = 0;
    }

    // 如果等级有变化，更新并返回true
    if (newLevel != oldLevel) {
        skill.level = newLevel;
        return true;
    }

    return false;  // 等级没有变化
}

// 获取技能等级
int SkillLevel::getSkillLevel(SkillType skillType)
{
    if (static_cast<int>(skillType) >= 0 && static_cast<int>(skillType) < SKILL_COUNT) {
        return s_skillData[static_cast<int>(skillType)].level;
    }
    return 0;  // 无效类型返回0
}