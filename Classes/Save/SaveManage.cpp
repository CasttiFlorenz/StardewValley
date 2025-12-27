#include "SaveManage.h"

USING_NS_CC;

// 获取单例
SaveManage* SaveManage::getInstance() 
{
    static SaveManage instance;
    return &instance;
}

// ========== 工具函数 ==========

std::string SaveManage::getFilePath(const std::string& filename)
{
    return FileUtils::getInstance()->getWritablePath() + filename;
}

// ========== 保存背包 ==========

// 序列化Item到JSON对象
rapidjson::Value SaveManage::serializeItem(const Item& item, rapidjson::Document::AllocatorType& alloc)
{
    rapidjson::Value itemObj(rapidjson::kObjectType);

    // 直接添加所有成员
    itemObj.AddMember("price", item.getPrice(), alloc);
    itemObj.AddMember("tag", static_cast<int>(item.getTag()), alloc);
    itemObj.AddMember("count", item.getCount(), alloc);
    itemObj.AddMember("scale", item.getScale(), alloc);
    itemObj.AddMember("printPos", item.getPrintPos(), alloc);

    // 获取字符串
    std::string name = item.getName();
    std::string path = item.getPath();

    itemObj.AddMember("name", rapidjson::Value(name.c_str(), alloc).Move(), alloc);
    itemObj.AddMember("path", rapidjson::Value(path.c_str(), alloc).Move(), alloc);

    return itemObj;
}

// 保存背包到文件
bool SaveManage::saveInventory() 
{
    const auto& inventory = PlaceItems::getInstance()->getCurrentInventory();

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    rapidjson::Value itemsArray(rapidjson::kArrayType);
    for (const auto& item : inventory) {
        itemsArray.PushBack(serializeItem(item, alloc), alloc);
    }

    doc.AddMember("inventory", itemsArray, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("save.json"));
}

// 从文件加载背包
bool SaveManage::loadInventory() 
{
    auto placeItems = PlaceItems::getInstance();
    if (!placeItems) return false;

    auto& inventory = const_cast<std::vector<Item>&>(placeItems->getCurrentInventory());

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("save.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("inventory")) {
        return false;
    }

    inventory.clear();

    const rapidjson::Value& itemsArray = doc["inventory"];
    for (rapidjson::SizeType i = 0; i < itemsArray.Size(); i++) {
        Item newItem;
        // 假设有从JSON创建Item的函数
        if (Item::createFromJson(itemsArray[i], newItem)) {
            inventory.push_back(newItem);
        }
    }

    return true;
}

// ========== 保存好感度 ==========

bool SaveManage::saveFriendships() 
{
    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    rapidjson::Value friendshipsObj(rapidjson::kObjectType);
    for (NPCBase* npc : NPCManager::getInstance()->getAllNPCs()) {
        if (npc) {
            std::string name = npc->getNPCName();
            friendshipsObj.AddMember(rapidjson::Value(name.c_str(), alloc).Move(), npc->getFriendship(), alloc);
        }
    }

    doc.AddMember("friendships", friendshipsObj, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("friendships.json"));
}

// 加载好感度
bool SaveManage::loadFriendships() 
{
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("friendships.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("friendships")) {
        return false;
    }

    const auto& npcList = NPCManager::getInstance()->getAllNPCs();
    const rapidjson::Value& friendshipsObj = doc["friendships"];

    for (auto it = friendshipsObj.MemberBegin(); it != friendshipsObj.MemberEnd(); ++it) {
        std::string npcName = it->name.GetString();
        for (NPCBase* npc : npcList) {
            if (npc && npc->getNPCName() == npcName) {
                npc->setNPCFriendship(it->value.GetInt());
                break;
            }
        }
    }

    return true;
}

// ========== 保存时间 ==========

rapidjson::Value SaveManage::serializeGameTime(const GameTime& time, rapidjson::Document::AllocatorType& alloc) 
{
    rapidjson::Value timeObj(rapidjson::kObjectType);
    timeObj.AddMember("year", time.year, alloc);
    timeObj.AddMember("season", static_cast<int>(time.season), alloc);
    timeObj.AddMember("day", time.dayOfMonth, alloc);
    timeObj.AddMember("hour", time.hour, alloc);
    timeObj.AddMember("minute", time.minute, alloc);
    return timeObj;
}

// 游戏时间反序列化
bool SaveManage::deserializeGameTime(const rapidjson::Value& timeObj, GameTime& time)
{
    time.year = timeObj["year"].GetInt();
    time.season = static_cast<Season>(timeObj["season"].GetInt());
    time.dayOfMonth = timeObj["day"].GetInt();
    time.hour = timeObj["hour"].GetInt();
    time.minute = timeObj["minute"].GetInt();
    return true;
}

// 保存游戏时间
bool SaveManage::saveGameTime() 
{
    auto timeManager = TimeManager::getInstance();
    if (!timeManager) return false;

    GameTime gameTime = timeManager->getCurrentTime();

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    doc.AddMember("gameTime", serializeGameTime(gameTime, alloc), alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("gametime.json"));
}

// 加载游戏时间
bool SaveManage::loadGameTime() 
{
    auto timeManager = TimeManager::getInstance();
    if (!timeManager) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("gametime.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("gameTime")) {
        return false;
    }

    GameTime gameTime;
    if (deserializeGameTime(doc["gameTime"], gameTime)) {
        timeManager->setTime(gameTime);
        return true;
    }

    return false;
}

// ========== 保存技能 ==========

rapidjson::Value SaveManage::serializeSkill(const SkillData& skill, rapidjson::Document::AllocatorType& alloc)
{
    rapidjson::Value skillObj(rapidjson::kObjectType);
    skillObj.AddMember("name", rapidjson::Value(skill.name.c_str(), alloc).Move(), alloc);
    skillObj.AddMember("level", skill.level, alloc);
    skillObj.AddMember("scale", skill.scale, alloc);
    skillObj.AddMember("iconPath", rapidjson::Value(skill.iconPath.c_str(), alloc).Move(), alloc);
    return skillObj;
}

// 保存技能数据
bool SaveManage::saveSkills()
{
    auto skillLevel = SkillLevel::getInstance();
    if (!skillLevel) return false;

    const SkillData* skills = skillLevel->getSkillData();
    int count = 4;
    if (!skills || count <= 0) return false;

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    rapidjson::Value skillsArray(rapidjson::kArrayType);
    for (int i = 0; i < count; i++) {
        skillsArray.PushBack(serializeSkill(skills[i], alloc), alloc);
    }

    doc.AddMember("skills", skillsArray, alloc);
    doc.AddMember("count", count, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("skills.json"));
}

// 加载技能数据
bool SaveManage::loadSkills()
{
    auto skillLevel = SkillLevel::getInstance();
    if (!skillLevel) return false;

    SkillData* skills = const_cast<SkillData*>(skillLevel->getSkillData());
    int count = 4;
    if (!skills || count <= 0) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("skills.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("skills")) {
        return false;
    }

    const rapidjson::Value& skillsArray = doc["skills"];
    int loadCount = std::min(count, static_cast<int>(skillsArray.Size()));

    for (int i = 0; i < loadCount; i++) {
        const rapidjson::Value& skillObj = skillsArray[i];
        skills[i].name = skillObj["name"].GetString();
        skills[i].level = skillObj["level"].GetInt();
        skills[i].scale = skillObj["scale"].GetFloat();
        skills[i].iconPath = skillObj["iconPath"].GetString();
    }

    return true;
}

// ========== 保存所有数据 ==========

// 保存所有数据
bool SaveManage::saveAllData()
{
    return saveInventory() &&
        saveFriendships() &&
        saveGameTime() &&
        saveSkills();
}

// 加载所有数据
bool SaveManage::loadAllData()
{
    return loadInventory() &&
        loadFriendships() &&
        loadGameTime() &&
        loadSkills();
}