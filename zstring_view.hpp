#pragma once
#include <string_view>

class zstring_view: private std::string_view {
    public:
        constexpr zstring_view() noexcept = default;
        constexpr zstring_view(const zstring_view&) noexcept = default;
        constexpr zstring_view(const char* s): std::string_view{s} {}
        constexpr zstring_view(const std::string& s): std::string_view{s.c_str()} {}
        constexpr zstring_view& operator=(const zstring_view&) noexcept = default;

        constexpr const char* c_str() noexcept { return data(); }

        using std::string_view::begin;
        using std::string_view::cbegin;
        using std::string_view::end;
        using std::string_view::cend;
        using std::string_view::rbegin;
        using std::string_view::crbegin;
        using std::string_view::rend;
        using std::string_view::crend;
        using std::string_view::operator[];
        using std::string_view::at;
        using std::string_view::front;
        using std::string_view::back;
        using std::string_view::data;
        using std::string_view::size;
        using std::string_view::length;
        using std::string_view::max_size;
        using std::string_view::empty;
        using std::string_view::remove_prefix;
        //Intentionally leaving out remove_suffix
        using std::string_view::swap;
        using std::string_view::copy;
        using std::string_view::substr;
        using std::string_view::compare;
        //using std::string_view::starts_with; //C++20
        //using std::string_view::ends_with; //C++20
        using std::string_view::find;
        using std::string_view::rfind;
        using std::string_view::find_first_of;
        using std::string_view::find_last_of;
        using std::string_view::find_first_not_of;
        using std::string_view::find_last_not_of;

        std::string_view to_string_view() { return *this; }
};

//TODO: Comparison operators

inline std::ostream& operator<<(std::ostream& out, zstring_view zsv)
{
    return out << zsv.to_string_view();
}

