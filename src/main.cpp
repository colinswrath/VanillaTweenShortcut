#include "Settings.h"
#include "Events.h"

#include <stddef.h>

void InitListener(SKSE::MessagingInterface::Message* a_msg) noexcept
{
    switch (a_msg->type)
    {
    case SKSE::MessagingInterface::kDataLoaded:
        Events::Register();
        break;
    }
}


SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    Init(skse);

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::init();
    logger::info("{} {} is loading...", name, version);
    SKSE::AllocTrampoline(28);
    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener(InitListener)) {
        return false;
    }

    if (!Settings::LoadJsonFile())
    {
        return false;
    }
    Settings::ParsePreLoadSettings();

    logger::info("{} has finished loading.", name);

    return true;
}
