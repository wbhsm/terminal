// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

namespace winrt
{
    // If we don't use winrt, nobody will include the ConversionTraits for winrt stuff.
    // If nobody includes it, these forward declarations will suffice.
    namespace Windows::Foundation
    {
        template<typename T>
        struct IReference;
    }
}

namespace til
{
#pragma region coalesce_value
    // Method Description:
    // - Base case provided to handle the last argument to coalesce_value<T...>()
    template<typename T>
    T coalesce_value(const T& base)
    {
        return base;
    }

    // Method Description:
    // - Base case provided to throw an assertion if you call coalesce_value(opt, opt, opt)
    template<typename T>
    T coalesce_value(const std::optional<T>& base)
    {
        static_assert(false, "coalesce_value must be passed a base non-optional value to be used if all optionals are empty");
        return T{};
    }

    // Method Description:
    // - Base case provided to throw an assertion if you call coalesce_value(opt, opt, opt)
    template<typename T>
    T coalesce_value(const winrt::Windows::Foundation::IReference<T>& base)
    {
        static_assert(false, "coalesce_value must be passed a base non-optional value to be used if all optionals are empty");
        return T{};
    }

    // Method Description:
    // - Returns the value from the first populated optional, or a base value if none were populated.
    template<typename T, typename... Ts>
    T coalesce_value(const std::optional<T>& t1, Ts&&... t2)
    {
        // Initially, I wanted to check "has_value" and short-circuit out so that we didn't
        // evaluate value_or for every single optional, but has_value/value emits exception handling
        // code that value_or doesn't. Less exception handling is cheaper than calling value_or a
        // few more times.
        return t1.value_or(coalesce_value(std::forward<Ts>(t2)...));
    }

    // Method Description:
    // - Returns the value from the first populated IReference, or a base value if none were populated.
    template<typename T, typename... Ts>
    T coalesce_value(const winrt::Windows::Foundation::IReference<T>& t1, Ts&&... t2)
    {
        return t1 ? t1.Value() : coalesce_value(std::forward<Ts>(t2)...);
    }
#pragma endregion

#pragma region coalesce
    // Method Description:
    // - Base case provided to handle the last argument to coalesce_value<T...>()
    template<typename T>
    std::optional<T> coalesce(const std::optional<T>& base)
    {
        return base;
    }

    // Method Description:
    // - Base case provided to handle the last argument to coalesce_value<T...>()
    template<typename T>
    winrt::Windows::Foundation::IReference<T> coalesce(const winrt::Windows::Foundation::IReference<T>& base)
    {
        return base;
    }

    // Method Description:
    // - Base case provided to handle the last argument to coalesce_value<T...>(..., nullopt)
    template<typename T>
    std::optional<T> coalesce(const std::nullopt_t& base)
    {
        return base;
    }

    // Method Description:
    // - Returns the value from the first populated optional, or the last one (if none of the previous had a value)
    template<typename T, typename... Ts>
    std::optional<T> coalesce(const std::optional<T>& t1, Ts&&... t2)
    {
        return t1.has_value() ? t1 : coalesce(std::forward<Ts>(t2)...);
    }

    // Method Description:
    // - Returns the value from the first populated IReference, or the last one (if none of the previous had a value)
    template<typename T, typename... Ts>
    winrt::Windows::Foundation::IReference<T> coalesce(const winrt::Windows::Foundation::IReference<T>& t1, Ts&&... t2)
    {
        return t1 ? t1 : coalesce(std::forward<Ts>(t2)...);
    }
#pragma endregion
}
