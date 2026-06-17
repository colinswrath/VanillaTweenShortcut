#include "Events.h"
#include "Settings.h"

namespace Events
{
    void OnMenuOpenCloseEventHandler::Register()
    {
        if (const auto ui = RE::UI::GetSingleton()) {
            ui->AddEventSink<RE::MenuOpenCloseEvent>(OnMenuOpenCloseEventHandler::GetSingleton());
        }
    }

    void SKSEModCallbackEventHandler::Register()
    {
        SKSE::GetModCallbackEventSource()->AddEventSink(SKSEModCallbackEventHandler::GetSingleton());
    }

    RE::BSEventNotifyControl OnMenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (a_event->opening && a_event->menuName == RE::TweenMenu::MENU_NAME) {
            auto ui = RE::UI::GetSingleton();
            auto menu = ui->GetMenu(RE::TweenMenu::MENU_NAME);

            RE::GFxValue arg;
            //bGamepad,shortCutText,shortCutKeyNum,shortCutGamepadeKeyNum,eventName
            std::array<RE::GFxValue, 4> hotkeyInit;

            auto devManager = RE::BSInputDeviceManager::GetSingleton();

            hotkeyInit[0] = devManager->IsGamepadEnabled();
            hotkeyInit[1] = Settings::hotkeyName.c_str();
            hotkeyInit[2] = Settings::keyboardBind;
            hotkeyInit[3] = Settings::gamepadBind;

            menu->uiMovie->Invoke("_root.TweenMenu_mc.SetShortcut", nullptr, hotkeyInit.data(), hotkeyInit.size());
        }
    }

    RE::BSEventNotifyControl SKSEModCallbackEventHandler::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource)
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }
        logger::info("Recieved {}", a_event->eventName.c_str());
        if (a_event->eventName == "TweenMenu_HotkeyPress") {
            auto ui = RE::UI::GetSingleton();

            //Credit: JPSteel for the quest journal fix. (Opening must be done via a different call)
            auto uiMessageQueue = RE::UIMessageQueue::GetSingleton();
            if (auto menu = ui->GetMenu(RE::TweenMenu::MENU_NAME)) {
                menu->uiMovie->Invoke("_root.TweenMenu_mc.CloseMenu", nullptr, nullptr, 0);
            }

            if (uiMessageQueue && Settings::openMenu) {

                if (auto menuToOpen = a_event->strArg.c_str()) {

                    if (Settings::menuName == RE::JournalMenu::MENU_NAME) {
                        OpenQuestsJournal(false);
                    }
                    else if (menuToOpen)
                    {
                        uiMessageQueue->AddMessage(Settings::menuName, RE::UI_MESSAGE_TYPE::kShow, nullptr);
                    }
                }
            }

            SKSE::GetTaskInterface()->AddTask([this]() {
                const SKSE::ModCallbackEvent modEvent{ Settings::eventName.c_str(), {}, 0.0f, nullptr };
                SKSE::GetModCallbackEventSource()->SendEvent(&modEvent);
            });
        }
    }
}
