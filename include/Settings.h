#pragma once

class Settings
{
public:
	using json = nlohmann::json;

	Settings() = delete;
    static bool         LoadJsonFile();
    static bool         ParsePreLoadSettings();
    static bool         ParsePostLoadSettings();
    static RE::BGSPerk* FindPerk(json file, std::string perkSectionName);
    static RE::FormID    ParseFormID(const std::string& str);

    inline static bool  debug_logging{};
    inline static int   gamepadBind{};
    inline static int   keyboardBind{};

    inline static std::string hotkeyName{};
    inline static std::string eventName{};
    inline static std::string menuName{};
    inline static bool        openMenu{};

	static inline json settingsJson;

	static inline constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\TweenShortcut.json";
};
