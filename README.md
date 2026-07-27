# Simple C++ library for string enums

The library allows defining enums that are convertible from/to string. This is useful in cases when
 a value can be from a set of allowed string values.

## Requirements

* C++20 compiler
* CMake 3.28

## Installation

### CPM

The library can be installed via [CPM](https://github.com/cpm-cmake/CPM.cmake):

```cmake
CPMAddPackage("gh:NTSFka/string-enum")
```

## Example

```c++
#include "ntsfka/string-enum.hpp"
// Or via module
// import ntsfka.string_enum;

enum class my_enum {
    value1,
    value2,
};

// Library requires only the convertor to be defined
template<>
struct ntsfka::string_enum::convertor<my_enum> {
    static auto to_string(const my_enum val) -> std::string_view {
        switch (val) {
            case my_enum::value1:
                return "value1";
            case my_enum::value2:
                return "value2";
        }
    }

    static auto try_from_string(const std::string_view val) -> std::optional<my_enum> {
        if (val == "value1") {
            return my_enum::value1;
        }

        if (val == "value2") {
            return my_enum::value2;
        }

        return std::nullopt;
    }
};

auto main(int argc, char** argv) -> int
{
    using namespace ntsfka::string_enum;

    const auto val = from_string<my_enum>("value1");

    std::cout << to_string(val) << "\n";
    // OR
    std::cout << val << "\n";
    // OR
    std::print("{}", val);
}
```

## Documentation

For any custom enum type an implementation must be defined via `ntsfka::string_enum::convertor`.

The convertor can define one of or both `to_string` (convert enum to string) and `try_from_string` (convert string to
enum) static member functions.

```c++
template<>
struct ntsfka::string_enum::convertor<my_enum> {
    static auto to_string(const my_enum val) -> std::string_view {
        switch (val) {
            case my_enum::value1:
                return "value1";
            case my_enum::value2:
                return "value2";
        }
    }

    static auto try_from_string(const std::string_view val) -> std::optional<my_enum> {
        if (val == "value1") {
            return my_enum::value1;
        }

        if (val == "value2") {
            return my_enum::value2;
        }

        return std::nullopt;
    }
};
```

The convertor can be then used via `to_string` and `try_from_string` or `from_string` functions.

```c++
// Convert from string
const auto val = ntsfka::string_enum::from_string<my_enum>("value1");

// Convert to string
const auto str = ntsfka::string_enum::to_string(val);
```

### Tagging

In some cases the different behavior is needed for conversion, e.g., JSON conversion vs. printing. This can be done
by using tags. The `convertor` implementation can be specialized for a specific tag, and conversion functions can be
called with a tag. By default, the `tag_default` tag is used.

The library provides `tag_print` that is used for printing to `std::format` and `std::ostream`, and by default it uses
the default tag as a fallback.

```c++
constexpr struct tag_json_t {} tag_json;

template<>
struct ntsfka::string_enum::convertor<my_enum, tag_json_t> {
    // ...
}; 

// Convert from string
const auto val = ntsfka::string_enum::from_string<my_enum>("value1", tag_json);

// Convert to string
const auto str = ntsfka::string_enum::to_string(val, tag_json);
```

## License

MIT
