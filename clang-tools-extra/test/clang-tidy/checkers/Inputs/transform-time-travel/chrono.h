#ifndef LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_TRANSFORM_TIME_TRAVEL_CHRONO_H
#define LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_TRANSFORM_TIME_TRAVEL_CHRONO_H

// Mimic <chrono>

template <long Num, long Den = 1> struct ratio {
    static constexpr long num = Num;
    static constexpr long den = Den;
};

template <class T> T declval() noexcept;

template <typename... Ts>
struct common_type {};
template <typename T>
struct common_type<T> : common_type<T, T> {};
template <typename T1, typename T2>
struct common_type<T1, T2> { using type = decltype(false? declval<T1>() : declval<T2>()); };
// FIX: Does not ensure types decay before finding a common_type
template <typename T1, typename T2, typename... Ts>
struct common_type<T1, T2, Ts...> : common_type<typename common_type<T1, T2>::type, Ts...> {};
template <typename... Ts>
using common_type_t = typename common_type<Ts...>::type;

namespace chrono {
    template <typename Rep, typename Period = ratio<1>>
    struct duration {
        // Constructors
        constexpr duration() = default;
        duration(const duration&) = default;
        template <typename Rep2>
        constexpr explicit duration(const Rep2& r);
        template <typename Rep2, typename Period2>
        constexpr duration(const duration<Rep2, Period2>& d);

        // Unary Operators
        constexpr common_type_t<duration> operator+() const;
        constexpr common_type_t<duration> operator-() const;

        // Assignment Operators
        duration& operator+=(const duration& d);
        duration& operator-=(const duration& d);
        duration& operator*=(const Rep& rhs);
        duration& operator/=(const Rep& rhs);
        duration& operator%=(const Rep& rhs);
        duration& operator%=(const duration& rhs);

        // Increment/Decrement
        duration& operator++();
        duration operator++(int);
        duration& operator--();
        duration operator--(int);

        // Member Function
        constexpr Rep count() const;
    };

    // Arithmetic Operators
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>
        constexpr operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>
        constexpr operator-(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period, typename Rep2>
    duration<common_type_t<Rep1, Rep2>, Period>
        constexpr operator*(const duration<Rep1,Period>& d, const Rep2& s);
    template <typename Rep1, typename Rep2, typename Period>
    duration<common_type_t<Rep1, Rep2>, Period>
        constexpr operator*(const Rep1& s, const duration<Rep2, Period>& d);
    template <typename Rep1, typename Period, typename Rep2>
    duration<common_type_t<Rep1, Rep2>, Period>
        constexpr operator/(const duration<Rep1,Period>& d, const Rep2& s);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    common_type_t<Rep1, Rep2>
        constexpr operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period, typename Rep2>
    duration<common_type_t<Rep1, Rep2>, Period>
        constexpr operator%(const duration<Rep1, Period>& d, const Rep2& s);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    common_type_t<duration<Rep1, Period1>, duration<Rep2, Period2>>
    constexpr operator%(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

    // Comparison Operators
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    constexpr bool operator==(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    constexpr bool operator<(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    constexpr bool operator<=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    constexpr bool operator>(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    constexpr bool operator>=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

    // FIX: Missing several non-member functions

    // Convenience type aliases
    using seconds = duration<long>;

    namespace literals {
        // Convenience typed literals
        constexpr chrono::seconds operator "" _s(unsigned long long secs);
    }
}

constexpr long gcd(long a, long b) { return (b == 0) ? a : gcd(b, a % b); }
constexpr long lcm(long a, long b) { return a * b / gcd(a, b); }

// Common type specialization
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
struct common_type<chrono::duration<Rep1, Period1>, chrono::duration<Rep2, Period2>> {
    using type = chrono::duration<common_type_t<Rep1, Rep2>,
                                  ratio<gcd(Period1::num, Period2::num), lcm(Period1::den, Period2::den)>>;
};

#endif // LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_TRANSFORM_TIME_TRAVEL_CHRONO_H
