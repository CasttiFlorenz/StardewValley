#include "DialogueLayer.h"

USING_NS_CC;
using namespace ui;

 * 工厂方法：创建 DialogueLayer 实例
 * 分配内存并初始化对象，使用 autorelease 将内存管理移交给 Cocos2d-x 自动释放池
 * @return 初始化成功的 DialogueLayer 指针，若失败则返回 nullptr
 */
DialogueLayer* DialogueLayer::create() {
    DialogueLayer* pRet = new(std::nothrow) DialogueLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease(); // 关键：标记为自动释放，避免内存泄漏
        return pRet;
    }
    // 如果初始化失败，安全删除并置空
    delete pRet;
    pRet = nullptr;
    return nullptr;
}

/**
 * 初始化层状态与 UI 组件
 * 设置触摸吞噬监听，初始化背景 Sprite、名字 Label、内容 Label 及选择按钮容器
 * @return 初始化成功返回 true，否则返回 false
 */
bool DialogueLayer::init() {
    if (!Layer::init()) return false;

    Size winSize = Director::getInstance()->getWinSize();

    // 1. 设置事件监听器 (吞噬触摸)
    auto listener = EventListenerTouchOneByOne::create();
    if (listener == nullptr) return false; // 空指针检查

    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch*, Event*) {
        // 安全检查：如果选择节点不存在或不可见
        if (_choiceNode && _choiceNode->isVisible()) {
            return true; // 正在做选择时，吞噬触摸，不响应点击
        }

        // 普通对话模式：点击屏幕显示下一句
        this->showNextSentence();
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 2. 初始化背景框 (使用 Constant.h 中的常量)
    _bg = Scale9Sprite::create(PATH_DIALOGUE_BG_DEFAULT);
    if (_bg == nullptr) {
        CCLOG("Error: Failed to load dialogue background: %s", PATH_DIALOGUE_BG_DEFAULT.c_str());
        return false;
    }
    _bg->setContentSize(SIZE_DIALOGUE_BG);
    _bg->setPosition(winSize.width / 2, POS_BG_OFFSET.y);
    this->addChild(_bg);

    // 3. 初始化名字标签
    _nameLabel = Label::createWithTTF("", PATH_FONT_PIXEL, FONT_SIZE_NAME);
    if (_nameLabel) {
        _nameLabel->setPosition(POS_NAME_LABEL);
        _nameLabel->setColor(COLOR_NAME_TEXT);
        _bg->addChild(_nameLabel);
    }

    // 4. 初始化内容标签
    _contentLabel = Label::createWithTTF("", PATH_FONT_PIXEL, FONT_SIZE_CONTENT);
    if (_contentLabel) {
        _contentLabel->setPosition(POS_CONTENT_LABEL);
        // 设置文本区域限制，自动换行
        _contentLabel->setDimensions(700, 100);
        _contentLabel->setColor(COLOR_CONTENT_TEXT);
        _bg->addChild(_contentLabel);
    }

    // 5. 初始化选择按钮容器节点
    _choiceNode = Node::create();
    if (_choiceNode) {
        _bg->addChild(_choiceNode);
        _choiceNode->setVisible(false); // 默认隐藏
        createChoiceButtons();          // 创建具体按钮
    }

    return true;
}


/**
 * 构建 YES/NO 选择按钮 UI
 * 创建按钮对象，设置其样式、位置、背景颜色以及点击事件监听器，并将它们添加到 _choiceNode 中
 */
void DialogueLayer::createChoiceButtons() {
    if (_choiceNode == nullptr) return;

    // --- 创建 YES 按钮 ---
    auto btnYes = Button::create();
    if (btnYes) {
        btnYes->setTitleText("YES");
        btnYes->setTitleFontSize(FONT_SIZE_BTN);
        btnYes->setTitleColor(Color3B::WHITE);
        btnYes->ignoreContentAdaptWithSize(false);
        btnYes->setContentSize(SIZE_CHOICE_BTN);
        btnYes->setPosition(POS_BTN_YES);

        // 添加绿色背景层
        auto yesBg = LayerColor::create(COLOR_BTN_BG_YES, SIZE_CHOICE_BTN.width, SIZE_CHOICE_BTN.height);
        if (yesBg) btnYes->addChild(yesBg, -1);

        // 设置点击回调
        btnYes->addClickEventListener([this](Ref* sender) {
            this->handleChoiceClick(sender, true);
            });

        _choiceNode->addChild(btnYes);
    }

    // --- 创建 NO 按钮 ---
    auto btnNo = Button::create();
    if (btnNo) {
        btnNo->setTitleText("NO");
        btnNo->setTitleFontSize(FONT_SIZE_BTN);
        btnNo->setTitleColor(Color3B::WHITE);
        btnNo->ignoreContentAdaptWithSize(false);
        btnNo->setContentSize(SIZE_CHOICE_BTN);
        btnNo->setPosition(POS_BTN_NO);

        // 添加红色背景层
        auto noBg = LayerColor::create(COLOR_BTN_BG_NO, SIZE_CHOICE_BTN.width, SIZE_CHOICE_BTN.height);
        if (noBg) btnNo->addChild(noBg, -1);

        // 设置点击回调
        btnNo->addClickEventListener([this](Ref* sender) {
            this->handleChoiceClick(sender, false);
            });

        _choiceNode->addChild(btnNo);
    }
}

/**
 * 统一处理选择按钮点击事件
 * 禁用按钮防止连点，通过 retain/release 延长对象生命周期以防止回调执行期间被销毁，并触发逻辑回调
 * @param sender 点击的按钮对象引用 (Ref*)
 * @param isYes  用户的选择结果 (true 代表 YES, false 代表 NO)
 */

void DialogueLayer::handleChoiceClick(cocos2d::Ref* sender, bool isYes) {
    // 类型转换规范：使用 static_cast 进行显式转换
    auto btn = static_cast<ui::Button*>(sender);
    if (btn) {
        btn->setEnabled(false); // 防止连点
    }

    // 内存保护：在回调执行期间持有对象引用，防止被意外析构
    this->retain();

    CCLOG("Choice made: %s", isYes ? "YES" : "NO");
    onChoiceMade(isYes);

    // 安全移除：检查父节点是否存在
    if (this->getParent() != nullptr) {
        this->removeFromParent();
    }

    // 对应上面的 retain，释放引用
    this->release();
}


/**
 * 执行选择后的回调逻辑
 * 在执行前检查回调函数是否有效，将结果传递给外部调用者
 * @param choice 用户做出的选择结果
 */
void DialogueLayer::onChoiceMade(bool choice) {
    if (_choiceCallback) {
        _choiceCallback(choice); // 执行外部传入的回调
    }
    else {
        CCLOG("Warning: DialogueLayer::_choiceCallback is null!");
    }
}

/**
 * 显示下一句对话内容
 * 从队列中取出下一条文本更新显示；若对话队列为空，则从父节点移除当前层
 */
void DialogueLayer::showNextSentence() {
    // 如果队列为空，说明对话结束
    if (_dialogueQueue.empty()) {
        this->removeFromParent(); // 自我销毁
        return;
    }

    // 取出下一句
    std::string text = _dialogueQueue.front();
    _dialogueQueue.pop();

    // 更新文本显示（前置检查空指针）
    if (_contentLabel) {
        _contentLabel->setString(text);
    }
}


/**
 * 根据 NPC 名字动态更新背景纹理
 * 拼接资源路径，加载新纹理并更新 SpriteFrame；若加载失败则打印警告
 * @param name NPC 名字，用于解析图片路径
 */
void DialogueLayer::updateBackground(const std::string& name) {
    if (_bg == nullptr) return;

    // 1. 拼接图片路径 (使用 Constant.h 常量)
    std::string path = PATH_DIALOGUE_PREFIX + name + ".png";

    // 2. 获取纹理缓存
    auto textureCache = Director::getInstance()->getTextureCache();
    if (textureCache == nullptr) return;

    // 3. 加载新纹理
    auto texture = textureCache->addImage(path);

    // 空指针安全检查：确保纹理加载成功
    if (texture) {
        // 创建 SpriteFrame
        auto frame = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));

        if (frame) {
            _bg->setSpriteFrame(frame);
            // 更新尺寸 (常用于根据角色名切换不同样式的对话框)
            _bg->setContentSize(SIZE_DIALOGUE_BG_EXPANDED);
        }
    }
    else {
        CCLOG("Warning: Failed to load texture for NPC: %s at path: %s", name.c_str(), path.c_str());
    }
}


/**
 * 公共接口：显示普通对话文本序列
 * 设置 NPC 名字，更新对应背景，将内容列表填充进队列，并显示第一句
 * @param name        说话的 NPC 名字
 * @param contentList 包含对话内容的字符串列表
 */
void DialogueLayer::showText(const std::string& name, const std::vector<std::string>& contentList) {
    if (_nameLabel) _nameLabel->setString(name);
    if (_choiceNode) _choiceNode->setVisible(false);

    updateBackground(name);

    // 清空旧队列
    while (!_dialogueQueue.empty()) {
        _dialogueQueue.pop();
    }

    // 填入新内容
    for (const auto& str : contentList) {
        _dialogueQueue.push(str);
    }

    showNextSentence();
}


/**
 * 公共接口：显示无名字的选择对话框 (重载)
 * 调用完整版 showChoice，传入空名字
 * @param content  显示的提问或文本内容
 * @param callback 做出选择后的回调函数
 */
void DialogueLayer::showChoice(const std::string& content, ChoiceCallback callback) {
    // 重用完整版的 showChoice，传入空名字即可
    showChoice("", content, callback);
}


/**
 * 公共接口：显示带 NPC 名字的选择对话框
 * 清空旧对话队列，设置提示文本，更新背景，保存回调函数并显示选择按钮
 * @param name     NPC 名字
 * @param content  显示的提问或文本内容
 * @param callback 做出选择后的回调函数
 */

void DialogueLayer::showChoice(const std::string& name, const std::string& content, ChoiceCallback callback) {
    // 清空队列，防止残留
    while (!_dialogueQueue.empty()) _dialogueQueue.pop();

    if (_nameLabel) _nameLabel->setString(name);
    if (_contentLabel) _contentLabel->setString(content);

    // 如果有名字，尝试更新背景；否则保持默认
    if (!name.empty()) {
        updateBackground(name);
    }

    _choiceCallback = callback;

    if (_choiceNode) {
        _choiceNode->setVisible(true);
    }

}
