/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SocialLevel.cpp
 * File Function: SocialLevel类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#include "SocialLevel.h"

USING_NS_CC;

// 初始化静态成员
SocialLevel::CharacterData SocialLevel::s_characterData[CHARACTER_COUNT];
const char* SocialLevel::HEART_FILLED = "/NPC/red heart.png";
const char* SocialLevel::HEART_EMPTY = "/NPC/empty heart.png";
bool SocialLevel::s_isInitialized = false;

// 初始化人物数据
void SocialLevel::initCharacterData()
{
    if (s_isInitialized) return;

    // 初始化3个人物的数据
    s_characterData[(int)CharacterType::Evelyn] = {
        "Evelyn", 0, "/NPC/EvelynPhoto.png", 6.0f
    };

    s_characterData[(int)CharacterType::Haley] = {
        "Haley", 0, "/NPC/HaleyPhoto.png", 5.2f
    };

    s_characterData[(int)CharacterType::Sam] = {
        "Sam", 0, "/NPC/SamPhoto.png", 5.2f
    };

    s_isInitialized = true;
}

// 设置好感度等级
void SocialLevel::setFriendshipLevel(CharacterType character, int level)
{
    int index = (int)character;
    if (index >= 0 && index < CHARACTER_COUNT) {
        if (level < 0) 
            level = 0;
        if (level > MAX_FRIENDSHIP_LEVEL) 
            level = MAX_FRIENDSHIP_LEVEL;
        s_characterData[index].friendshipLevel = level;
    }
}

// 增加好感度
bool SocialLevel::increaseFriendshipLevel(CharacterType character, int amount)
{
    int index = (int)character;
    if (index < 0 || index >= CHARACTER_COUNT || amount <= 0) {
        return false;
    }

    CharacterData& data = s_characterData[index];
    int oldLevel = data.friendshipLevel;

    int newLevel = oldLevel + amount;
    if (newLevel > MAX_FRIENDSHIP_LEVEL) {
        newLevel = MAX_FRIENDSHIP_LEVEL;
    }

    if (newLevel != oldLevel) {
        data.friendshipLevel = newLevel;
        return true;
    }

    return false;
}

// 减少好感度
bool SocialLevel::decreaseFriendshipLevel(CharacterType character, int amount)
{
    int index = (int)character;
    if (index < 0 || index >= CHARACTER_COUNT || amount <= 0) {
        return false;
    }

    CharacterData& data = s_characterData[index];
    int oldLevel = data.friendshipLevel;

    int newLevel = oldLevel - amount;
    if (newLevel < 0) {
        newLevel = 0;
    }

    if (newLevel != oldLevel) {
        data.friendshipLevel = newLevel;
        return true;
    }

    return false;
}

// 获取好感度等级
int SocialLevel::getFriendshipLevel(CharacterType character)
{
    int index = (int)character;
    if (index >= 0 && index < CHARACTER_COUNT) {
        return s_characterData[index].friendshipLevel;
    }
    return 0;
}

// 创建好感度爱心显示（单行显示10个爱心）
Node* SocialLevel::createFriendshipHearts(int level)
{
    Node* heartContainer = Node::create();

    // 参数设置
    const float SPACING = 35.0f;      // 爱心间距
    const int TOTAL_HEARTS = 10;      // 总共10个爱心

    // 计算总宽度和起始位置
    float totalWidth = (TOTAL_HEARTS - 1) * SPACING;
    float startX = -totalWidth / 2 ;

    // 单行显示所有爱心
    for (int i = 0; i < TOTAL_HEARTS; i++) {
        bool isFilled = (i < level / 150);
        const char* heartPath = isFilled ? HEART_FILLED : HEART_EMPTY;

        auto heartSprite = Sprite::create(heartPath);
        if (heartSprite) {
            float currentX = startX + i * SPACING;
            heartSprite->setPosition(Vec2(currentX, 0));
            heartSprite->setScale(0.3f);
            heartContainer->addChild(heartSprite);
        }
    }

    return heartContainer;
}
// 创建单个社交人物项
Node* SocialLevel::createCharacterItem(CharacterType characterType, const Vec2& position)
{
    int index = (int)characterType;
    if (index < 0 || index >= CHARACTER_COUNT) {
        return nullptr;
    }

    // 确保数据已初始化
    if (!s_isInitialized) {
        initCharacterData();
    }

    CharacterData& character = s_characterData[index];
    int currentLevel = character.friendshipLevel;

    // 创建容器
    Node* characterItem = Node::create();
    characterItem->setPosition(position);

    // ========== 人物头像（最左侧） ==========
    auto characterIcon = Sprite::create(character.iconPath);
    if (characterIcon) {
        characterIcon->setScale(character.scale);
        characterIcon->setAnchorPoint(Vec2(0, 0.5f));  // 左对齐
        characterIcon->setPosition(Vec2(-350, 0));     // 最左边
        characterItem->addChild(characterIcon);
    }

    // ========== 人物名字（头像右侧） ==========
    auto nameLabel = Label::createWithTTF(character.name, "fonts/Louis George Cafe Bold.ttf", 28);
    if (nameLabel) {
        nameLabel->setTextColor(Color4B::BLACK);
        nameLabel->setAnchorPoint(Vec2(0, 0.5f));  // 左对齐
        nameLabel->setPosition(Vec2(-260, 0));     // 头像右侧
        characterItem->addChild(nameLabel);
    }

    // ========== 名字和爱心之间的竖线 ==========
    DrawNode* verticalLine = DrawNode::create();
    if (verticalLine) {
        // 竖线参数
        Vec2 startPoint(-150, -75);  // 竖线起点
        Vec2 endPoint(-150, 75);     // 竖线终点
        float lineWidth = 2.0f;     // 线宽
        Color4F lineColor(0.55f, 0.27f, 0.07f, 1.0f);

        verticalLine->drawSegment(startPoint, endPoint, lineWidth, lineColor);
        characterItem->addChild(verticalLine);
    }

    // ========== 好感度爱心显示 ==========
    Node* heartContainer = createFriendshipHearts(currentLevel);
    if (heartContainer) {
        heartContainer->setPosition(Vec2(100, 0));  // 中间位置
        heartContainer->setScale(1.2f);            // 适当缩放
        characterItem->addChild(heartContainer);
    }

    return characterItem;
}

// 创建社交界面
Node* SocialLevel::createSocialInterface(Node* parent, Sprite* background)
{
    if (!parent || !background) {
        return nullptr;
    }

    // 确保数据已初始化
    if (!s_isInitialized) {
        initCharacterData();
        s_isInitialized = true;
    }

    // 创建容器
    Node* container = Node::create();

    // 获取背景位置
    Vec2 bgPos = background->getPosition();

    // 计算起始位置（背景上部）
    float startY = bgPos.y + 120;
    float spacing = 120;  // 每个人物项的间距

    // 创建3个人物项
    for (int i = 0; i < CHARACTER_COUNT; i++) {
        CharacterType characterType = static_cast<CharacterType>(i);
        Vec2 position = Vec2(bgPos.x, startY - i * spacing);

        Node* characterItem = createCharacterItem(characterType, position);
        if (characterItem) {
            container->addChild(characterItem);

            // ========== 人物之间的横线（最后一个不画） ==========
            if (i < CHARACTER_COUNT - 1) {
                DrawNode* horizontalLine = DrawNode::create();
                if (horizontalLine) {
                    // 横线参数
                    float lineLength = 920.0f;  // 横线长度
                    float lineY = startY - (i + 0.5f) * spacing;  // 横线Y坐标（在两个人之间）
                    float lineWidth = 2.0f;      // 线宽
                    Color4F lineColor(0.55f, 0.27f, 0.07f, 1.0f);

                    Vec2 startPoint(bgPos.x - lineLength / 2, lineY);
                    Vec2 endPoint(bgPos.x + lineLength / 2, lineY);

                    horizontalLine->drawSegment(startPoint, endPoint, lineWidth, lineColor);
                    container->addChild(horizontalLine);
                }
            }
        }
    }

    // 将容器添加到父节点
    parent->addChild(container, 100);

    return container;
}