# traits
A small header only utility library adding traits for function types in C++.

# Usage
Add the CMakeLists.txt to your project via `add_subdirectory` and then include the header in your source files:
(Or add the header directly to your project.)

```cmake
cmake_minimum_required(VERSION 3.16) # or higher

add_subdirectory(traits)
```

Then in your source files, you can include the header like this:
```cpp
#include <traits/functions.hpp>
```

# Examples
```cpp
#include <traits/functions.hpp>

// All of these can be inspected:
auto lambda = [](int x) { return x * 2; };

struct FunctionObject {
    int operator()(double x) const {
        return static_cast<int>(x);
    }

    std::string memberFunction(double x) const& {
        return std::to_string(x);
    }
};

double freeFunction(double x) {
    return x * 2.0;
}

int main() {
    using namespace Traits;

    // Example of using FunctionTraits with a lambda
    auto lambda = [](std::string const& x, std::size_t y) noexcept { return x.size() + y; };
    using LambdaTraits = FunctionTraits<decltype(lambda)>;

    static_assert(LambdaTraits::arity == 2, "Lambda should have arity 1");
    static_assert(std::is_same_v<LambdaTraits::ReturnType, std::size_t>, "Lambda should return int");
    static_assert(std::is_same_v<LambdaTraits::Argument<0>, std::string const&>, "Lambda should take int as argument");
    static_assert(std::is_same_v<LambdaTraits::Argument<1>, std::size_t>, "Lambda should take size_t as argument");
    static_assert(
        std::is_same_v<LambdaTraits::ArgumentDecayed<0>, std::string>, "Lambda should take size_t as argument");
    static_assert(std::is_same_v<LambdaTraits::ArgsTuple, std::tuple<std::string const&, std::size_t>>);
    static_assert(std::is_same_v<LambdaTraits::ArgsTupleDecayed, std::tuple<std::string, std::size_t>>);
    static_assert(LambdaTraits::qualifiers.isConst, "Lambda is not mutable");
    static_assert(!LambdaTraits::qualifiers.isVolatile, "Lambda is not volatile");
    static_assert(LambdaTraits::qualifiers.isNoexcept, "Lambda is noexcept");
    static_assert(!LambdaTraits::qualifiers.isReferenceQualified, "Lambda is not reference qualified");
    static_assert(!LambdaTraits::qualifiers.isRvalueReferenceQualified, "Lambda is not rvalue reference qualified");
    static_assert(
        std::is_same_v<LambdaTraits::StandardFunctionType, std::function<std::size_t(std::string const&, std::size_t)>>,
        "Standard function type should match");
    static_assert(
        std::is_same_v<LambdaTraits::StandardFunctionTypeDecayed, std::function<std::size_t(std::string, std::size_t)>>,
        "Standard function type decayed should match");

    return 0;
}
```

