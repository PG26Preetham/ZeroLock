// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ZeroLockGameMode.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ZEROLOCK_ZeroLockGameMode_generated_h
#error "ZeroLockGameMode.generated.h already included, missing '#pragma once' in ZeroLockGameMode.h"
#endif
#define ZEROLOCK_ZeroLockGameMode_generated_h

#define FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAZeroLockGameMode(); \
	friend struct Z_Construct_UClass_AZeroLockGameMode_Statics; \
public: \
	DECLARE_CLASS(AZeroLockGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/ZeroLock"), ZEROLOCK_API) \
	DECLARE_SERIALIZER(AZeroLockGameMode)


#define FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AZeroLockGameMode(AZeroLockGameMode&&); \
	AZeroLockGameMode(const AZeroLockGameMode&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ZEROLOCK_API, AZeroLockGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AZeroLockGameMode); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AZeroLockGameMode) \
	ZEROLOCK_API virtual ~AZeroLockGameMode();


#define FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_9_PROLOG
#define FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_12_INCLASS_NO_PURE_DECLS \
	FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ZEROLOCK_API UClass* StaticClass<class AZeroLockGameMode>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
