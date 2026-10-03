#pragma once

#include <cassert>
#include <map>

template <class S, class T>
std::map<T, S> make_inverse_map(const std::map<S, T>& m)
{
    std::map<T, S> m_inv;
    for (const auto& [key, value]: m)
    {
        auto [it, is_new] = m_inv.insert({value, key});
        assert(is_new);
    }
    return m_inv;
}
