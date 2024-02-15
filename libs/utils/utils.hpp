#pragma once
#include <cstdint>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <map>
#include <numbers>
#include <unordered_map>

#include <logger/logger.hpp>

#if defined(__GNUC__)
    #define Y_UNLIKELY(exp) __builtin_expect(!!(exp), 0)
#else
    #define Y_UNLIKELY(exp) (exp)
    [[noreturn]] __forceinline void __builtin_unreachable() { __assume(false); }
    [[noreturn]] __forceinline void UNIMPLEMENTED(char const *const s) { throw s; }
#endif



using i8  = int8_t;
using ui8 = uint8_t;
using i16  = int16_t;
using ui16 = uint16_t;
using i32  = int32_t;
using ui32 = uint32_t;
using i64  = int64_t;
using ui64 = uint64_t;
using usize = size_t;

constexpr double EPSILON = 0.0000001;

namespace NFunctions {
    constexpr double Gamma(double);
    double Beta(double, double);
}

namespace std {
    template<class T, class U>
    std::ostream& operator<<(std::ostream& out, const std::unordered_map<T, U>& map) {
        out << "{\n";

        for (auto&& [k, v] : map) {
            out << "\t" << k << ": " << v << Endl;
        }

        out << "}";
        return out;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& map) {
        out << "[ ";

        for (auto& v : map) {
            out << v << " ";
        }

        out << "]";
        return out;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& out, const std::optional<T>& op) {
        if (op.has_value()) {
            out << op.value();
        } else {
            out << "std::nullopt";
        }
        return out;
    }

    template<class TupType, size_t... I>
    void _print(std::ostream& out, const TupType& _tup, std::index_sequence<I...>)
    {
        out << "Tupple:";
        (..., (out << "\nIndex " << I << ":\n" << std::get<I>(_tup)));
    }

    template<class... Args>
    std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& _tup)
    {
        _print(out, _tup, std::make_index_sequence<sizeof...(Args)>());

        return out;
    }
}