#pragma once

#include <functional>
#include <tuple>
#include <type_traits>

namespace Traits
{
    template <typename MemberFunctionT>
    struct FunctionTraitsImpl;

    struct FunctionQualifiers
    {
        bool isConst = false;
        bool isVolatile = false;
        bool isRef = false;
        bool isRvalueRef = false;
        bool isNoexcept = false;

        constexpr bool operator==(const FunctionQualifiers& other) const noexcept
        {
            return isConst == other.isConst && isVolatile == other.isVolatile && isRef == other.isRef &&
                isRvalueRef == other.isRvalueRef && isNoexcept == other.isNoexcept;
        }
    };

    /**
     * @brief This class provides information about a function type, such as its return type, argument types, and arity.
     *
     * @tparam ReturnT The return type of the function.
     * @tparam Args The argument types of the function.
     */
    template <typename ReturnT, FunctionQualifiers qualifiersValue = FunctionQualifiers{}, typename... Args>
    struct FunctionTraitsCommon
    {
      public:
        /// The return type of the function.
        using ReturnType = ReturnT;

        /// The arguments of the function as a tuple
        using ArgsTuple = std::tuple<Args...>;

        /// The arguments of the function as a tuple with decayed types. Decayed means that references and qualifiers
        /// (const / volatile) are removed.
        using ArgsTupleDecayed = std::tuple<std::decay_t<Args>...>;

        /// The qualifiers of the function, such as const, volatile, reference, rvalue reference, and noexcept.
        constexpr static FunctionQualifiers qualifiers = qualifiersValue;

      private:
        template <std::size_t N>
        struct ArgImpl
        {
            using type = std::tuple_element_t<N, ArgsTuple>;
        };

      public:
        /// The number of arguments of the function (= arity)
        constexpr static auto arity = sizeof...(Args);

        /// Allows for accessing the N-th argument type of the function.
        template <std::size_t N>
        using Argument = typename ArgImpl<N>::type;

        /// Allows for accessing the N-th argument type of the function with decayed types (removes references and
        /// qualifiers).
        template <std::size_t N>
        using ArgumentDecayed = std::decay_t<typename ArgImpl<N>::type>;

        /// The function type as a std::function
        using StandardFunctionType = std::function<ReturnT(Args...)>;

        /// The function type as a std::function with decayed argument types (removes references and qualifiers).
        using StandardFunctionTypeDecayed = std::function<ReturnT(std::decay_t<Args>...)>;

        struct AsStdFunctionImpl
        {
            using type = std::function<ReturnT(Args...)>;
            using DecayedType = std::function<ReturnT(std::decay_t<Args>...)>;
        };

        using AsStdFunction = typename AsStdFunctionImpl::type;
        using AsStdFunctionDecayed = typename AsStdFunctionImpl::DecayedType;
    };

    /**
     * @brief When the function is a member function, also provide the class type.
     *
     * @tparam ClassT The class type of the member function.
     * @tparam ReturnT The return type of the member function.
     * @tparam Args The argument types of the member function.
     */
    template <typename ClassT, typename ReturnT, FunctionQualifiers qualifiers, typename... Args>
    struct FunctionTraitsClassCommon : public FunctionTraitsCommon<ReturnT, qualifiers, Args...>
    {
        /// The class type of the class owning the member function.
        using ClassType = ClassT;
    };

    /**
     * @brief Specialization for function pointers:
     *
     * @tparam ReturnT
     * @tparam Args
     */
    template <typename ReturnT, typename... Args>
    struct FunctionTraitsImpl<ReturnT (*)(Args...)> : public FunctionTraitsCommon<ReturnT, {}, Args...>
    {};
    template <typename ReturnT, typename... Args>
    struct FunctionTraitsImpl<ReturnT (*)(Args...) noexcept>
        : public FunctionTraitsCommon<ReturnT, {.isNoexcept = true}, Args...>
    {};

#define TRAITS_LIB_CREATE_SPECIALIZATION(QUALIFIER, QUALIFIER_OBJECT) \
    template <typename ReturnT, typename... Args> \
    struct FunctionTraitsImpl<ReturnT(Args...) QUALIFIER> \
        : public FunctionTraitsCommon<ReturnT, QUALIFIER_OBJECT, Args...> \
    {}; \
    template <typename ClassT, typename ReturnT, typename... ArgsT> \
    struct FunctionTraitsImpl<ReturnT (ClassT::*)(ArgsT...) QUALIFIER> \
        : public FunctionTraitsClassCommon<ClassT, ReturnT, QUALIFIER_OBJECT, ArgsT...> \
    {};

    // Specializations for all kinds of qualifiers
    namespace Detail
    {
        constexpr FunctionQualifiers cvQualified{.isConst = true, .isVolatile = true};
        constexpr FunctionQualifiers constRef{.isConst = true, .isRef = true};
        constexpr FunctionQualifiers volatileRef{.isVolatile = true, .isRef = true};
        constexpr FunctionQualifiers cvRef{.isConst = true, .isVolatile = true, .isRef = true};
        constexpr FunctionQualifiers constRvalueRef{.isConst = true, .isRvalueRef = true};
        constexpr FunctionQualifiers volatileRvalueRef{.isVolatile = true, .isRvalueRef = true};
        constexpr FunctionQualifiers cvRvalueRef{.isConst = true, .isVolatile = true, .isRvalueRef = true};

        constexpr FunctionQualifiers constNoexcept{.isConst = true, .isNoexcept = true};
        constexpr FunctionQualifiers volatileNoexcept{.isVolatile = true, .isNoexcept = true};
        constexpr FunctionQualifiers cvNoexcept{.isConst = true, .isVolatile = true, .isNoexcept = true};
        constexpr FunctionQualifiers refNoexcept{.isRef = true, .isNoexcept = true};
        constexpr FunctionQualifiers rvalueRefNoexcept{.isRvalueRef = true, .isNoexcept = true};
        constexpr FunctionQualifiers constRefNoexcept{.isConst = true, .isRef = true, .isNoexcept = true};
        constexpr FunctionQualifiers volatileRefNoexcept{.isVolatile = true, .isRef = true, .isNoexcept = true};
        constexpr FunctionQualifiers cvRefNoexcept{
            .isConst = true,
            .isVolatile = true,
            .isRef = true,
            .isNoexcept = true};
        constexpr FunctionQualifiers constRvalueRefNoexcept{.isConst = true, .isRvalueRef = true, .isNoexcept = true};
        constexpr FunctionQualifiers volatileRvalueRefNoexcept{
            .isVolatile = true,
            .isRvalueRef = true,
            .isNoexcept = true};
        constexpr FunctionQualifiers cvRvalueRefNoexcept{
            .isConst = true,
            .isVolatile = true,
            .isRvalueRef = true,
            .isNoexcept = true};
    }

    TRAITS_LIB_CREATE_SPECIALIZATION(, {})
    TRAITS_LIB_CREATE_SPECIALIZATION(const, {.isConst = true})
    TRAITS_LIB_CREATE_SPECIALIZATION(volatile, {.isVolatile = true})
    TRAITS_LIB_CREATE_SPECIALIZATION(const volatile, Detail::cvQualified)

    TRAITS_LIB_CREATE_SPECIALIZATION(&, {.isRef = true})
    TRAITS_LIB_CREATE_SPECIALIZATION(const&, Detail::constRef)
    TRAITS_LIB_CREATE_SPECIALIZATION(volatile&, Detail::volatileRef)
    TRAITS_LIB_CREATE_SPECIALIZATION(const volatile&, Detail::cvRef)
    TRAITS_LIB_CREATE_SPECIALIZATION(&&, {.isRvalueRef = true})
    TRAITS_LIB_CREATE_SPECIALIZATION(const&&, Detail::constRvalueRef)
    TRAITS_LIB_CREATE_SPECIALIZATION(volatile&&, Detail::volatileRvalueRef)
    TRAITS_LIB_CREATE_SPECIALIZATION(const volatile&&, Detail::cvRvalueRef)

    TRAITS_LIB_CREATE_SPECIALIZATION(noexcept, {.isNoexcept = true})
    TRAITS_LIB_CREATE_SPECIALIZATION(const noexcept, Detail::constNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(volatile noexcept, Detail::volatileNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(const volatile noexcept, Detail::cvNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(& noexcept, Detail::refNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(&& noexcept, Detail::rvalueRefNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(const& noexcept, Detail::constRefNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(volatile& noexcept, Detail::volatileRefNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(const volatile& noexcept, Detail::cvRefNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(const&& noexcept, Detail::constRvalueRefNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(volatile&& noexcept, Detail::volatileRvalueRefNoexcept)
    TRAITS_LIB_CREATE_SPECIALIZATION(const volatile&& noexcept, Detail::cvRvalueRefNoexcept)

    namespace Detail
    {
        /**
         * @brief This concept is true when the function is not a member function and not a regular function.
         *
         * @tparam FunctionT The function type to check.
         */
        template <typename FunctionT>
        concept IsFunctionObject =
            (!std::is_function_v<typename std::remove_pointer_t<FunctionT>> &&
             !std::is_member_function_pointer_v<FunctionT>);

        /**
         * @brief This concept is true when the function is a regular function.
         *
         * @tparam FunctionT
         */
        template <typename FunctionT>
        concept IsRegularFunction = std::is_function_v<FunctionT> && !std::is_member_function_pointer_v<FunctionT>;

        /**
         * @brief This concept is true when the function is a member function pointer.
         *
         * @tparam FunctionT
         */
        template <typename FunctionT>
        concept IsMemberFunctionPointer = std::is_member_function_pointer_v<FunctionT>;
    } // namespace Detail

    template <typename FunctionT>
    struct FunctionTraits
    {};

    /**
     * @brief When the function is a function object (an object with function call operator), inspect the call operator
     * instead and not the function object itself. This is also the specialization that makes lambda functions work.
     *
     * @tparam FunctionT
     */
    template <typename FunctionT>
    requires Detail::IsFunctionObject<FunctionT>
    struct FunctionTraits<FunctionT> : public FunctionTraitsImpl<decltype(&std::decay_t<FunctionT>::operator())>
    {};

    /**
     * @brief This specialization is for regular functions.
     *
     * @tparam FunctionT
     */
    template <typename FunctionT>
    requires Detail::IsRegularFunction<FunctionT>
    struct FunctionTraits<FunctionT> : public FunctionTraitsImpl<FunctionT>
    {};

    /**
     * @brief This specialization is for function pointer types.
     *
     * @tparam FunctionT
     */
    template <typename FunctionT>
    requires std::is_pointer_v<FunctionT> && std::is_function_v<std::remove_pointer_t<FunctionT>>
    struct FunctionTraits<FunctionT> : public FunctionTraitsImpl<FunctionT>
    {};

    /**
     * @brief This specialization is for member function pointers.
     *
     * @tparam FunctionT
     */
    template <typename FunctionT>
    requires Detail::IsMemberFunctionPointer<FunctionT>
    struct FunctionTraits<FunctionT> : public FunctionTraitsImpl<FunctionT>
    {};
}