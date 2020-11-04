#pragma once

#include "Monarch.g.h"
#include "Peasant.h"
#include "../cascadia/inc/cppwinrt_utils.h"

// 50dba6cd-2222-4b12-8363-5e06f5d0082c
constexpr GUID Monarch_clsid{
    0x50dba6cd,
    0x2222,
    0x4b12,
    { 0x83, 0x63, 0x5e, 0x06, 0xf5, 0xd0, 0x08, 0x2c }
};

enum class GlomToLastWindow : uint64_t
{
    Never = 0,
    LastActive = 1,
    Always = 2
};

namespace winrt::MonarchPeasantSample::implementation
{
    struct Monarch : public MonarchT<Monarch>
    {
        Monarch();
        ~Monarch();

        uint64_t GetPID();

        uint64_t AddPeasant(winrt::MonarchPeasantSample::IPeasant peasant);

        winrt::MonarchPeasantSample::IPeasant GetPeasant(uint64_t peasantID);

        winrt::MonarchPeasantSample::IPeasant GetMostRecentPeasant();

        void SetSelfID(const uint64_t selfID);

        bool ProposeCommandline(array_view<const winrt::hstring> args, winrt::hstring cwd);
        void ToggleWindowingBehavior();

    private:
        uint64_t _nextPeasantID{ 1 };
        uint64_t _thisPeasantID{ 0 };
        uint64_t _mostRecentPeasant{ 0 };
        GlomToLastWindow _windowingBehavior{ GlomToLastWindow::Never };
        std::unordered_map<uint64_t, winrt::MonarchPeasantSample::IPeasant> _peasants;

        void _setMostRecentPeasant(const uint64_t peasantID);

        void _peasantWindowActivated(const winrt::Windows::Foundation::IInspectable& sender,
                                     const winrt::Windows::Foundation::IInspectable& args);
    };
}

namespace winrt::MonarchPeasantSample::factory_implementation
{
    struct Monarch : MonarchT<Monarch, implementation::Monarch>
    {
    };
}
