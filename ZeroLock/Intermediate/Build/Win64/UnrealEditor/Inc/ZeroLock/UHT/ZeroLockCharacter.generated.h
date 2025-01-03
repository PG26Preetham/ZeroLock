// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ZeroLockCharacter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ZEROLOCK_ZeroLockCharacter_generated_h
#error "ZeroLockCharacter.generated.h already included, missing '#pragma once' in ZeroLockCharacter.h"
#endif
#define ZEROLOCK_ZeroLockCharacter_generated_h

#define FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAZeroLockCharacter(); \
	friend struct Z_Construct_UClass_AZeroLockCharacter_Statics; \
public: \
	DECLARE_CLASS(AZeroLockCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/ZeroLock"), NO_API) \
	DECLARE_SERIALIZER(AZeroLockCharacter)


#define FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AZeroLockCharacter(AZeroLockCharacter&&); \
	AZeroLockCharacter(const AZeroLockCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AZeroLockCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AZeroLockCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AZeroLockCharacter) \
	NO_API virtual ~AZeroLockCharacter();


#define FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h_18_PROLOG
#define FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h_21_INCLASS_NO_PURE_DECLS \
	FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ZEROLOCK_API UClass* StaticClass<class AZeroLockCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_ZeroLock_Source_ZeroLock_ZeroLockCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
