/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryManager.cpp
 * File Function: InventoryManager类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#include "InventoryManager.h"

USING_NS_CC;

// 静态变量初始化
Node* InventoryManager::s_parent = nullptr;
Sprite* InventoryManager::s_background = nullptr;
std::array<InventoryManager::BackpackInfo, 3> InventoryManager::s_backpacks;
std::array<Sprite*, 4> InventoryManager::s_buttons;
std::array<Vec2, 4> InventoryManager::s_buttonPositions;
Node* InventoryManager::s_container = nullptr;
InventoryManager::ButtonCallback InventoryManager::s_buttonCallback = nullptr;
float InventoryManager::s_backpackWidth = 0;
float InventoryManager::s_backpackHeight = 0;
int InventoryManager::s_currentPressedIndex = 0;


// 重置所有静态变量
void InventoryManager::resetStaticVariables()
{
    // 重置指针类型变量
    s_parent = nullptr;
    s_background = nullptr;
    s_container = nullptr;  // RefPtr会自动管理释放

    // 重置按钮回调
    s_buttonCallback = nullptr;

    // 重置数组
    for (auto& backpack : s_backpacks) {
        backpack.sprite = nullptr;
        backpack.position = Vec2::ZERO;
        backpack.scale = 0.0f;
    }

    for (auto& button : s_buttons) {
        button = nullptr;
    }

    for (auto& pos : s_buttonPositions) {
        pos = Vec2::ZERO;
    }

    // 重置标量变量
    s_backpackWidth = 0.0f;
    s_backpackHeight = 0.0f;
    s_currentPressedIndex = 0;
}

// 创建整个界面
void InventoryManager::createInterface(
    Node* parent,
    std::array<BackpackInfo, 3>& backpacks,
    std::array<Sprite*, 4>& buttons,
    Sprite*& background,
    float& backpackWidth,
    float& backpackHeight,
    float& cellWidth)
{
    if (!parent) return;
    s_parent = parent;
    if (!s_parent) return;

    // 创建背景
    background = createBackground(parent);
    s_background = background;
    if (!s_background) return;

    // 创建背包
    createBackpacks(parent, backpacks);
    s_backpacks = backpacks;

    // 计算尺寸
    if (backpacks[0].sprite) {
        Size contentSize = backpacks[0].sprite->getContentSize();
        backpackWidth = contentSize.width * backpacks[0].scale;
        backpackHeight = contentSize.height * backpacks[0].scale;
        s_backpackWidth = backpackWidth;
        s_backpackHeight = backpackHeight;
        cellWidth = backpackWidth / INVENTORY_COLS;  // 12个格子
    }

    auto cantainer = SkillLevel::createSkillsInterface(s_parent, s_background);
    cantainer->setVisible(false);

    // 创建按钮
    createButtons(parent, background, buttons, backpackWidth);
}

// 创建背景
Sprite* InventoryManager::createBackground(Node* parent)
{
    if (!parent) return nullptr;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create(PATH_INVENTORY_BG);
    if (background) {
        background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 70));
        background->setScaleX(1.5f);
        background->setScaleY(2.0f);
        parent->addChild(background, -1);
    }

    return background;
}

// 创建背包
void InventoryManager::createBackpacks(Node* parent, std::array<BackpackInfo, 3>& backpacks)
{
    if (!parent) return;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 背包位置
    float yPositions[3] = {
        visibleSize.height * 0.75f,
        visibleSize.height * 0.6f,
        visibleSize.height * 0.45f
    };

    for (int i = 0; i < INVENTORY_ROWS; i++) {
        // 创建背包精灵
        auto backpack = Sprite::create(PATH_INVENTORY_SLOT_BG);
        if (backpack) {
            Vec2 position = Vec2(visibleSize.width / 2, yPositions[i]);
            float scale = 1.65f;

            backpack->setPosition(position);
            backpack->setScale(scale);
            parent->addChild(backpack);

            // 保存信息
            backpacks[i].sprite = backpack;
            backpacks[i].position = position;
            backpacks[i].scale = scale;
        }
    }
}

// 设置初始背包界面
void InventoryManager::setInitPress()
{
    if (!s_buttons[0]) return;

    // 判断是否需要切换到背包界面
    bool needSwitchToInventory = (s_currentPressedIndex != 0);

    // 设置按钮状态
    if (s_buttons[0]) {
        s_currentPressedIndex = 0;
        s_buttons[0]->setPosition(Vec2(s_buttonPositions[0].x, s_buttonPositions[0].y - 10));
    }

    for (int i = 1; i < INVENTORT_BUTTON_COUNT; i++) {
        if (s_buttons[i]) {
            s_buttons[i]->setPosition(Vec2(s_buttonPositions[i].x, s_buttonPositions[i].y));
        }
    }

    // 只有在需要时才切换到背包界面
    if (needSwitchToInventory) {
        switchToInventory();
    }

}

// 创建按钮
void InventoryManager::createButtons(
    Node* parent,
    Sprite* background,
    std::array<Sprite*, 4>& buttons,
    float backpackWidth)
{
    if (!parent || !background) return;

    // 计算按钮位置
    float buttonY = background->getPositionY() * 1.52f;
    float buttonSpacing = backpackWidth / INVENTORT_BUTTON_COUNT;
    float startX = background->getPositionX() - backpackWidth / 2 + buttonSpacing / 2;

    // 创建按钮
    for (int i = 0; i < INVENTORT_BUTTON_COUNT; i++) {
        float buttonX = startX + i * buttonSpacing;

        auto button = Sprite::create(BUTTON_IMAGES[i]);
        if (button) {
            button->setPosition(Vec2(buttonX, buttonY));
            button->setScale(1.7f);

            int tag = BUTTON_TAG_BASE + i;
            button->setTag(tag);

            // 保存位置和指针
            s_buttonPositions[i] = button->getPosition();
            s_buttons[i] = button;
            buttons[i] = button;

            // 设置触摸监听
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            std::function<bool(Touch*, Event*)> callback =
                [](Touch* touch, Event* event) -> bool {
                return InventoryManager::onButtonTouch(touch, event);
                };
            listener->onTouchBegan = callback;

            Director::getInstance()->getEventDispatcher()
                ->addEventListenerWithSceneGraphPriority(listener, button);

            parent->addChild(button, 1);
        }
    }
}


// 设置回调
void InventoryManager::setButtonCallback(ButtonCallback callback)
{
    s_buttonCallback = callback;
} 

// 按钮触摸回调
bool InventoryManager::onButtonTouch(Touch* touch, Event* event)
{
    if (!touch || !event) return false;

    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if (!target) return false;

    // 使用世界坐标系检测
    Vec2 touchLocation = touch->getLocation();  // 屏幕坐标
    Rect buttonRect = target->getBoundingBox(); // 世界坐标矩形

    // 直接比较
    if (buttonRect.containsPoint(touchLocation)) {
        int tag = target->getTag();
        int index = tag - BUTTON_TAG_BASE;

        if (index >= 0 && index < INVENTORT_BUTTON_COUNT) {
            // 处理按钮动画
            if (s_currentPressedIndex != -1 && s_currentPressedIndex != index) {
                if (s_buttons[s_currentPressedIndex]) {
                    s_buttons[s_currentPressedIndex]->setPosition(s_buttonPositions[s_currentPressedIndex]);
                }
            }

            // 更新当前按钮
            s_currentPressedIndex = index;
            target->setPosition(Vec2(s_buttonPositions[index].x, s_buttonPositions[index].y - 10));

            // 切换界面
            switch (index) {
                case 0: switchToInventory(); break;
                case 1: switchToSkills(); break;
                case 2: switchToSocial(); break;
                case 3: switchToExit(); break;
            }

            // 调用回调通知外部
            if (s_buttonCallback) {
                s_buttonCallback(index);
            }

            return true;
        }
    }
    return false;
}


// 切换到背包界面
void InventoryManager::switchToInventory()
{
    // 移除其他界面
    removeContainer();

    // 显示所有背包
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        if (s_backpacks[i].sprite) {
            s_backpacks[i].sprite->setVisible(true);
        }
    }
}

// 切换到技能界面
void InventoryManager::switchToSkills()
{
    // 隐藏所有背包
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        if (s_backpacks[i].sprite) {
            s_backpacks[i].sprite->setVisible(false);
        }
    }

    // 移除之前的界面
    removeContainer();

    // 创建技能界面
    if (s_parent && s_background) {
        s_container = SkillLevel::createSkillsInterface(s_parent, s_background);
    }
}

// 切换到社交界面
void InventoryManager::switchToSocial()
{
    // 隐藏所有背包
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        if (s_backpacks[i].sprite) {
            s_backpacks[i].sprite->setVisible(false);
        }
    }

    // 移除界面
    removeContainer();

    if (s_parent && s_background) {
        s_container = SocialLevel::createSocialInterface(s_parent, s_background);
    }
}

// 切换到退出界面
void InventoryManager::switchToExit()
{
    // 隐藏所有背包
    for (int i = 0; i < INVENTORY_ROWS; i++) {
        if (s_backpacks[i].sprite) {
            s_backpacks[i].sprite->setVisible(false);
        }
    }

    // 移除界面
    removeContainer();

    // 退出逻辑
    if (s_parent && s_background) {
        s_container = ExitConfirm::create(s_parent, s_background);
    }
}

// 清除旧界面
void InventoryManager::removeContainer()
{
    if (!s_container) return;  

    if (s_container->getParent()) {
        s_container->removeFromParent();
    }

    s_container = nullptr;  
}


// 获取当前按钮索引
int InventoryManager::getCurrentButtonIndex()
{
    return s_currentPressedIndex;

}
