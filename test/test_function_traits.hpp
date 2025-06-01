#pragma once

#include <traits/functions.hpp>

#include <gtest/gtest.h>

#include <type_traits>

namespace Traits::Tests
{
    void nonMember();

    struct FunctionObjectTrivial
    {
        void operator()()
        {}
    };

#define CREATE_MEMBER_FUNCTION_VARIANT(Qualifier, Name) \
    struct FunctionObjectQualified##Name \
    { \
        int operator()(int) Qualifier \
        { \
            return 0; \
        } \
    }; \
    struct MemberFunctionStruct##Name \
    { \
        int memberFunction(int) Qualifier \
        { \
            return 0; \
        } \
    };

    CREATE_MEMBER_FUNCTION_VARIANT(, None)
    CREATE_MEMBER_FUNCTION_VARIANT(const, Const)
    CREATE_MEMBER_FUNCTION_VARIANT(volatile, Volatile)
    CREATE_MEMBER_FUNCTION_VARIANT(const volatile, ConstVolatile)
    CREATE_MEMBER_FUNCTION_VARIANT(&, Ref)
    CREATE_MEMBER_FUNCTION_VARIANT(const&, ConstRef)
    CREATE_MEMBER_FUNCTION_VARIANT(volatile&, VolatileRef)
    CREATE_MEMBER_FUNCTION_VARIANT(const volatile&, ConstVolatileRef)
    CREATE_MEMBER_FUNCTION_VARIANT(&&, RvalueRef)
    CREATE_MEMBER_FUNCTION_VARIANT(const&&, ConstRvalueRef)
    CREATE_MEMBER_FUNCTION_VARIANT(volatile&&, VolatileRvalueRef)
    CREATE_MEMBER_FUNCTION_VARIANT(const volatile&&, ConstVolatileRvalueRef)
    CREATE_MEMBER_FUNCTION_VARIANT(noexcept, Noexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(const noexcept, ConstNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(volatile noexcept, VolatileNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(const volatile noexcept, ConstVolatileNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(& noexcept, RefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(const& noexcept, ConstRefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(volatile& noexcept, VolatileRefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(const volatile& noexcept, ConstVolatileRefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(&& noexcept, RvalueRefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(const&& noexcept, ConstRvalueRefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(volatile&& noexcept, VolatileRvalueRefNoexcept)
    CREATE_MEMBER_FUNCTION_VARIANT(const volatile&& noexcept, ConstVolatileRvalueRefNoexcept)

    struct TraitsTests : public ::testing::Test
    {
        void memberFunction()
        {}
    };

    TEST_F(TraitsTests, CanGetReturnTypeFromRegularFunctionType)
    {
        using Traits = FunctionTraits<decltype(nonMember)>;
        EXPECT_TRUE((std::is_same_v<Traits::ReturnType, void>));
    }

    TEST_F(TraitsTests, CanGetReturnTypeFromRegularFunctionPointerType)
    {
        using Traits = FunctionTraits<decltype(&nonMember)>;
        EXPECT_TRUE((std::is_same_v<Traits::ReturnType, void>));
    }

    TEST_F(TraitsTests, CanGetReturnTypeFromMemberFunctionType)
    {
        using Traits = FunctionTraits<decltype(&TraitsTests::memberFunction)>;
        EXPECT_TRUE((std::is_same_v<Traits::ReturnType, void>));
    }

    TEST_F(TraitsTests, CanGetReturnTypeFromFunctionObjectType)
    {
        using Traits = FunctionTraits<FunctionObjectTrivial>;
        EXPECT_TRUE((std::is_same_v<Traits::ReturnType, void>));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedNone)
    {
        EXPECT_EQ(Traits::FunctionTraits<FunctionObjectQualifiedNone>::qualifiers, (Traits::FunctionQualifiers{}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConst)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConst>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedVolatile)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedVolatile>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstVolatile)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstVolatile>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedRef>::qualifiers,
            (Traits::FunctionQualifiers{.isRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstRef>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedVolatileRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedVolatileRef>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstVolatileRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstVolatileRef>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedRvalueRef>::qualifiers,
            (Traits::FunctionQualifiers{.isRvalueRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstRvalueRef>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRvalueRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedVolatileRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedVolatileRvalueRef>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRvalueRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstVolatileRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstVolatileRvalueRef>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRvalueRef = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedVolatileNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedVolatileNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstVolatileNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstVolatileNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedVolatileRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedVolatileRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstVolatileRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstVolatileRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedRvalueRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstRvalueRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedVolatileRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedVolatileRvalueRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, FunctionObjectQualifiedConstVolatileRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<FunctionObjectQualifiedConstVolatileRvalueRefNoexcept>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersNone)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructNone::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConst)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConst::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersVolatile)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructVolatile::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstVolatile)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstVolatile::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersVolatileRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructVolatileRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstVolatileRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstVolatileRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructRvalueRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isRvalueRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstRvalueRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRvalueRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersVolatileRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructVolatileRvalueRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRvalueRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstVolatileRvalueRef)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstVolatileRvalueRef::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRvalueRef = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersVolatileNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructVolatileNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstVolatileNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstVolatileNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersVolatileRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructVolatileRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstVolatileRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstVolatileRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructRvalueRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<decltype(&MemberFunctionStructConstRvalueRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersVolatileRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<
                decltype(&MemberFunctionStructVolatileRvalueRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isVolatile = true, .isRvalueRef = true, .isNoexcept = true}));
    }

    TEST_F(TraitsTests, MemberFunctionQualifiersConstVolatileRvalueRefNoexcept)
    {
        EXPECT_EQ(
            Traits::FunctionTraits<
                decltype(&MemberFunctionStructConstVolatileRvalueRefNoexcept::memberFunction)>::qualifiers,
            (Traits::FunctionQualifiers{.isConst = true, .isVolatile = true, .isRvalueRef = true, .isNoexcept = true}));
    }
}