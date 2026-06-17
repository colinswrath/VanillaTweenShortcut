
namespace Events
{
    class OnMenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
    public:
        static OnMenuOpenCloseEventHandler* GetSingleton()
        {
            static OnMenuOpenCloseEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
        static void              Register();
    };

    class SKSEModCallbackEventHandler : public RE::BSTEventSink<SKSE::ModCallbackEvent>
    {
    public:
        static SKSEModCallbackEventHandler* GetSingleton()
        {
            static SKSEModCallbackEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource) override;
        static void              Register();
    };

    static void OpenQuestsJournal(bool a_scrollSpeedIncrease)
    {
        using func_t = decltype(&OpenQuestsJournal);
        REL::Relocation<func_t> func{ RELOCATION_ID(52428, 53327) };
        return func(a_scrollSpeedIncrease);
    }

    inline static void Register()
    {
        OnMenuOpenCloseEventHandler::Register();
        SKSEModCallbackEventHandler::Register();
    }
}
