#include "Settings.h"

bool Settings::LoadJsonFile()
{
    std::ifstream inFile(FILE_NAME);
    if (!inFile.is_open()) {
        logger::error("Failed to open TweenShortcut.json!");
        return false;
    }

    logger::info("Parsing json");

    try {
        std::string json_str((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        settingsJson = nlohmann::json::parse(json_str);
        return true;
    }
    catch (const std::exception& e) {
        logger::critical(FMT_STRING("Failed to load TweenShortcut.json!: {}"), e.what());
        return false;
    }
}

bool Settings::ParsePreLoadSettings()
{
    try {
        logger::info("Parsing pre load settings");

        if (settingsJson == nullptr) {
            logger::critical("No TweenShortcut.json found!");
            return false;
        }

        auto debugLogs = Settings::settingsJson.value("DebugLogs", false);
        logger::info("\tDebug log: {}", debugLogs);
        if (debugLogs) {
            spdlog::default_logger()->set_level(spdlog::level::debug);
            spdlog::default_logger()->flush_on(spdlog::level::debug);
            logger::debug("Debug logging enabled");
        }

        if (settingsJson.contains("Keybinds")) {
            auto& bindings  = settingsJson["Keybinds"];
            keyboardBind    = bindings.value("Keyboard", 46);
            gamepadBind     = bindings.value("Gamepad", 279);

            logger::info("Keybinds: Gamepad={}, Keyboard={}", gamepadBind, keyboardBind);
        }

        if (settingsJson.contains("TextSettings")) {
            auto& veryHard  = settingsJson["TextSettings"];
            hotkeyName      = veryHard.value("Hotkeytext", "");

            logger::info("TextSettings: Hotkeyname={}", hotkeyName);
        }

        if (settingsJson.contains("ActionSettings")) {
            auto& veryHard  = settingsJson["ActionSettings"];
            eventName       = veryHard.value("EventName", "");
            openMenu        = veryHard.value("OpenMenu", true);
            menuName        = veryHard.value("MenuName", "");

            logger::info("ActionSettings: EventName={}, OpenMenu={}, MenuName={}", eventName, openMenu, menuName);
        }

        logger::info("Settings loaded");
        
        return true;
    }
    catch (const std::exception& e) {
        logger::critical(FMT_STRING("Failed to parse TweenShortcut.json!: {}"), e.what());
        return false;
    }
}

bool Settings::ParsePostLoadSettings()
{
    return true;
}

RE::BGSPerk* Settings::FindPerk(json file, std::string perkSectionName)
{
    auto perkSection = file[perkSectionName].get<json>();
    auto formId      = perkSection.value("formid", "");
    auto fileName    = perkSection.value("filename", "");

    if (formId.empty() && fileName.empty()) {
        logger::info("Perk formId or filename were empty");
        return nullptr;
    }

    auto parsedId = ParseFormID(formId);

    if (!parsedId) {
        logger::error("Could not parse perk formId");
        return nullptr;
    }

    logger::info(FMT_STRING("Parsed formId: {}"), parsedId);
    logger::info(FMT_STRING("Looking up form in: {}"), fileName);

    auto form = RE::TESDataHandler::GetSingleton()->LookupForm(parsedId, fileName);

    if (!form) {
        logger::error("Could not find perk");
        return nullptr;
    }

    logger::info("Found perk");

    RE::BGSPerk* foundPerk = skyrim_cast<RE::BGSPerk*>(form);
    return foundPerk;
}

RE::FormID Settings::ParseFormID(const std::string& str)
{
    RE::FormID         result;
    std::istringstream ss{ str };
    ss >> std::hex >> result;
    return result;
}
