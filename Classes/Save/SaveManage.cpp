#include "SaveManage.h"

USING_NS_CC;

// 获取单例
SaveManage* SaveManage::getInstance()
{
    static SaveManage instance;
    return &instance;
}

// 获取文件路径
std::string SaveManage::getFilePath(const std::string& filename)
{
    // 存档目录
    std::string customPath = FileUtils::getInstance()->getDefaultResourceRootPath() + "../../Resources/SaveGame/";

    // 确保目录存在
    if (!FileUtils::getInstance()->isDirectoryExist(customPath)) {
        FileUtils::getInstance()->createDirectory(customPath);
    }

    return customPath + filename;
}

// 序列化 Item
rapidjson::Value SaveManage::serializeItem(const Item& item, rapidjson::Document::AllocatorType& alloc)
{
    rapidjson::Value itemObj(rapidjson::kObjectType);

    itemObj.AddMember("price", item.getPrice(), alloc);
    itemObj.AddMember("tag", static_cast<int>(item.getTag()), alloc);
    itemObj.AddMember("count", item.getCount(), alloc);
    itemObj.AddMember("scale", item.getScale(), alloc);
    itemObj.AddMember("printPos", item.getPrintPos(), alloc);

    std::string name = item.getName();
    std::string path = item.getPath();

    itemObj.AddMember("name", rapidjson::Value(name.c_str(), alloc).Move(), alloc);
    itemObj.AddMember("path", rapidjson::Value(path.c_str(), alloc).Move(), alloc);

    return itemObj;
}

// 保存物品栏
bool SaveManage::saveInventory()
{
    auto placeItems = PlaceItems::getInstance();
    if (!placeItems) return false;

    const auto& inventory = placeItems->getCurrentInventory();

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

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("inventory.json"));
}

// 加载物品栏
bool SaveManage::loadInventory()
{
    auto placeItems = PlaceItems::getInstance();
    if (!placeItems) return false;

    auto& inventory = const_cast<std::vector<Item>&>(placeItems->getCurrentInventory());

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("inventory.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("inventory")) {
        return false;
    }

    inventory.clear();

    // 通知更新
    EventCustom event("INVENTORY_COUNT_CHANGED");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

    std::vector<Item> savedItems;
    const rapidjson::Value& itemsArray = doc["inventory"];
    for (rapidjson::SizeType i = 0; i < itemsArray.Size(); i++) {
        Item newItem;
        if (Item::createFromJson(itemsArray[i], newItem)) {
            savedItems.push_back(newItem);
        }
    }

    auto inventoryScene = InventoryScene::getInstance();
    if (inventoryScene) {
        return inventoryScene->loadItemsFromSaveData(savedItems);
    }
    return false;
}

// 保存好感度
bool SaveManage::saveFriendships()
{
    auto npcManager = NPCManager::getInstance();
    if (!npcManager) return false;

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    rapidjson::Value friendshipsObj(rapidjson::kObjectType);
    for (const NPCBase* npc : npcManager->getAllNPCs()) {
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
    auto npcManager = NPCManager::getInstance();
    if (!npcManager) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("friendships.json"));
    if (jsonStr.empty()) return false;

    rapidjson::Document doc;
    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("friendships")) {
        return false;
    }

    const auto& npcList = npcManager->getAllNPCs();
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

// ========== 时间 天气 金钱 ==========


// 序列化游戏时间
rapidjson::Value SaveManage::serializeGameTime(const GameTime& time, rapidjson::Document::AllocatorType& alloc)
{
    rapidjson::Value timeObj(rapidjson::kObjectType);
    timeObj.AddMember("year", time.getYear(), alloc);
    timeObj.AddMember("season", static_cast<int>(time.getSeason()), alloc);
    timeObj.AddMember("day", time.getDayOfMonth(), alloc);
    timeObj.AddMember("hour", time.getHour(), alloc);
    timeObj.AddMember("minute", time.getMinute(), alloc);
    return timeObj;
}

// 游戏时间反序列化
bool SaveManage::deserializeGameTime(const rapidjson::Value& timeObj, GameTime& time)
{
    if (!timeObj.IsObject()) return false;

    // 检查字段是否存在
    const char* requiredFields[] = { "year", "season", "day", "hour", "minute" };
    for (const char* field : requiredFields) {
        if (!timeObj.HasMember(field)) return false;
    }

    // 使用setter函数
    time.setYear(timeObj["year"].GetInt());
    time.setSeason(static_cast<Season>(timeObj["season"].GetInt()));
    time.setDayOfMonth(timeObj["day"].GetInt());
    time.setHour(timeObj["hour"].GetInt());
    time.setMinute(timeObj["minute"].GetInt());

    return true;
}

// 保存游戏环境
bool SaveManage::saveGameConditions()
{
    auto timeManager = TimeManager::getInstance();
    if (!timeManager) return false;

    // 获取当前时间
    GameTime gameTime = timeManager->getCurrentTime();

    // 获取金钱
    int money = 0;
    auto moneyManager = Money::getInstance();
    if (moneyManager) {
        money = moneyManager->getMoney();
    }

    // 获取天气
    int weather = 0;  // 存储为int
    auto weatherManager = WeatherManager::getInstance();
    if (weatherManager) {
        weather = static_cast<int>(weatherManager->getCurrentWeather());
    }

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    // 添加时间数据
    doc.AddMember("gameTime", serializeGameTime(gameTime, alloc), alloc);
    // 添加金钱数据
    doc.AddMember("money", money, alloc);
    // 添加天气数据
    doc.AddMember("weather", weather, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("conditions.json"));
}

// 加载游戏环境
bool SaveManage::loadGameConditions()
{
    auto timeManager = TimeManager::getInstance();
    if (!timeManager) return false;

    // 读取时间文件
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("conditions.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError()) {
        return false;
    }

    bool timeLoaded = false;
    bool moneyLoaded = false;
    bool weatherLoaded = false;

    // 解析时间数据
    if (doc.HasMember("gameTime")) {
        GameTime gameTime;
        if (deserializeGameTime(doc["gameTime"], gameTime)) {
            timeManager->setTime(gameTime);
            timeLoaded = true;
        }
    }
    // 解析金钱数据
    if (doc.HasMember("money") && doc["money"].IsInt()) {
        int money = doc["money"].GetInt();
        auto moneyManager = Money::getInstance();  // 假设存在
        if (moneyManager) {
            moneyManager->setMoney(money);
            moneyLoaded = true;
        }
    }
    // 解析天气数据
    if (doc.HasMember("weather") && doc["weather"].IsInt()) {
        int weatherValue = doc["weather"].GetInt();
        auto weatherManager = WeatherManager::getInstance();
        if (weatherManager) {
            weatherManager->setWeather(static_cast<WeatherType>(weatherValue));
            weatherLoaded = true;
        }
    }

    return timeLoaded && moneyLoaded && weatherLoaded;
}

// 保存技能

// 序列化技能数据
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

    // 获取技能数组
    const SkillData* skills = skillLevel->getSkillData();
    const int count = static_cast<int>(SkillType::SKILL_COUNT);
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
    const int count = static_cast<int>(SkillType::SKILL_COUNT);
    if (!skills || count <= 0) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(getFilePath("skills.json"));
    rapidjson::Document doc;

    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.HasMember("skills")) {
        return false;
    }

    // 从JSON数组加载技能数据
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


// 保存畜棚数据
bool SaveManage::saveBarnData()
{
    auto barn = BarnManager::getInstance();
    if (!barn) return false;

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    // 1. 干草
    rapidjson::Value hayArray(rapidjson::kArrayType);
    for (const auto& pos : barn->getHayPositions()) {
        rapidjson::Value p(rapidjson::kObjectType);
        p.AddMember("x", pos.x, alloc);
        p.AddMember("y", pos.y, alloc);
        hayArray.PushBack(p, alloc);
    }
    doc.AddMember("hays", hayArray, alloc);

    // 2. 动物
    rapidjson::Value animalArray(rapidjson::kArrayType);
    for (const int type : barn->getAnimalTypes()) {
        animalArray.PushBack(type, alloc);
    }
    doc.AddMember("animals", animalArray, alloc);

    // 3. 产物
    rapidjson::Value prodArray(rapidjson::kArrayType);
    for (const auto& pair : barn->getProductions()) {
        rapidjson::Value p(rapidjson::kObjectType);
        p.AddMember("nestIndex", pair.first, alloc);
        p.AddMember("itemType", pair.second, alloc);
        prodArray.PushBack(p, alloc);
    }
    doc.AddMember("productions", prodArray, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("barn_data.json"));
}

// 加载畜棚数据
bool SaveManage::loadBarnData()
{
    std::string path = getFilePath("barn_data.json");
    if (!FileUtils::getInstance()->isFileExist(path)) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(path);
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    if (doc.HasParseError()) return false;

    std::vector<Vec2> hays;
    if (doc.HasMember("hays")) {
        for (const auto& v : doc["hays"].GetArray()) {
            hays.emplace_back(v["x"].GetFloat(), v["y"].GetFloat());
        }
    }

    std::vector<int> animals;
    if (doc.HasMember("animals")) {
        for (const auto& v : doc["animals"].GetArray()) {
            animals.push_back(v.GetInt());
        }
    }

    std::vector<std::pair<int, int>> prods;
    if (doc.HasMember("productions")) {
        for (const auto& v : doc["productions"].GetArray()) {
            prods.push_back({ v["nestIndex"].GetInt(), v["itemType"].GetInt() });
        }
    }

    auto barn = BarnManager::getInstance();
    if (barn) {
        barn->restoreData(hays, animals, prods);
    }
    return true;
}

// 保存农场数据
bool SaveManage::saveFarmData()
{
    auto farmItemMgr = FarmItemManager::getInstance();
    auto cultivationMgr = CultivationManager::getInstance();

    if (!farmItemMgr || !cultivationMgr) return false;

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    // 1. 环境物品
    rapidjson::Value itemsArr(rapidjson::kArrayType);
    auto items = farmItemMgr->getItems();
    for (const auto& item : items) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("type", static_cast<int>(item.type), alloc);
        obj.AddMember("x", item.x, alloc);
        obj.AddMember("y", item.y, alloc);
        itemsArr.PushBack(obj, alloc);
    }
    doc.AddMember("environmentItems", itemsArr, alloc);

    // 2. 耕地与作物
    rapidjson::Value soilsArr(rapidjson::kArrayType);
    auto soils = cultivationMgr->getSoilsData();
    for (const auto& s : soils) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("x", s.x, alloc);
        obj.AddMember("y", s.y, alloc);
        obj.AddMember("status", s.status, alloc);
        obj.AddMember("cropType", s.cropType, alloc);
        obj.AddMember("cropStage", s.cropStage, alloc);
        obj.AddMember("cropStatus", s.cropStatus, alloc);
        soilsArr.PushBack(obj, alloc);
    }
    doc.AddMember("soils", soilsArr, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("farm_data.json"));
}

// 加载农场数据
bool SaveManage::loadFarmData()
{
    std::string path = getFilePath("farm_data.json");
    if (!FileUtils::getInstance()->isFileExist(path)) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(path);
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError()) return false;

    // 1. 环境物品
    if (doc.HasMember("environmentItems")) {
        std::vector<FarmItemManager::ItemData> items;
        const auto& arr = doc["environmentItems"];
        for (const auto& v : arr.GetArray()) {
            FarmItemManager::ItemData d;
            d.type = static_cast<EnvironmentItemType>(v["type"].GetInt());
            d.x = v["x"].GetFloat();
            d.y = v["y"].GetFloat();
            items.push_back(d);
        }

        auto farmItemMgr = FarmItemManager::getInstance();
        if (farmItemMgr) {
            farmItemMgr->restoreData(items);
        }
    }

    // 2. 耕地与作物
    if (doc.HasMember("soils")) {
        std::vector<CultivationManager::SoilSaveData> soils;
        const auto& arr = doc["soils"];
        for (const auto& v : arr.GetArray()) {
            CultivationManager::SoilSaveData s;
            s.x = v["x"].GetFloat();
            s.y = v["y"].GetFloat();
            s.status = v["status"].GetInt();
            s.cropType = v["cropType"].GetInt();
            s.cropStage = v["cropStage"].GetInt();

            if (v.HasMember("cropStatus")) {
                s.cropStatus = v["cropStatus"].GetInt();
            }
            else {
                s.cropStatus = static_cast<int>(CropStatus::GROWING);
            }

            soils.push_back(s);
        }

        auto cultivationMgr = CultivationManager::getInstance();
        if (cultivationMgr) {
            cultivationMgr->restoreData(soils);
        }
    }

    return true;
}

// 保存矿洞数据
bool SaveManage::saveMinesData()
{
    auto minesMgr = MinesItemManager::getInstance();
    if (!minesMgr) return false;

    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    rapidjson::Value itemsArr(rapidjson::kArrayType);
    auto items = minesMgr->getItems();
    for (const auto& item : items) {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("type", static_cast<int>(item.type), alloc);
        obj.AddMember("x", item.x, alloc);
        obj.AddMember("y", item.y, alloc);
        itemsArr.PushBack(obj, alloc);
    }
    doc.AddMember("minesItems", itemsArr, alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), getFilePath("mines_data.json"));
}

// 加载矿洞数据
bool SaveManage::loadMinesData()
{
    std::string path = getFilePath("mines_data.json");
    if (!FileUtils::getInstance()->isFileExist(path)) return false;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(path);
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError()) return false;

    if (doc.HasMember("minesItems")) {
        std::vector<MinesItemManager::MineItemData> items;
        const auto& arr = doc["minesItems"];
        for (const auto& v : arr.GetArray()) {
            MinesItemManager::MineItemData d;
            d.type = static_cast<EnvironmentItemType>(v["type"].GetInt());
            d.x = v["x"].GetFloat();
            d.y = v["y"].GetFloat();
            items.push_back(d);
        }

        auto minesMgr = MinesItemManager::getInstance();
        if (minesMgr) {
            minesMgr->restoreData(items);
        }
    }
    return true;
}

// 保存所有数据
bool SaveManage::saveAllData()
{
    const bool result1 = saveInventory();
    const bool result2 = saveFriendships();
    const bool result3 = saveSkills();
    const bool result4 = saveBarnData();
    const bool result5 = saveFarmData();
    const bool result6 = saveMinesData();
    const bool result7 = saveGameConditions();

    return result1 && result2 && result3 && result4 && result5 && result6 && result7;
}

// 加载所有数据
bool SaveManage::loadAllData()
{
    const bool result1 = loadInventory();
    const bool result2 = loadFriendships();
    const bool result3 = loadSkills();
    const bool result4 = loadBarnData();
    const bool result5 = loadFarmData();
    const bool result6 = loadMinesData();
    const bool result7 = loadGameConditions();

    return result1 && result2 && result3 && result4 && result5 && result6 && result7;
}