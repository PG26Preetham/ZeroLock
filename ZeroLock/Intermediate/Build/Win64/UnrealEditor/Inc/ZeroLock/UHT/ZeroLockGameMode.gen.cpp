// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ZeroLock/ZeroLockGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeZeroLockGameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_ZeroLock();
ZEROLOCK_API UClass* Z_Construct_UClass_AZeroLockGameMode();
ZEROLOCK_API UClass* Z_Construct_UClass_AZeroLockGameMode_NoRegister();
// End Cross Module References

// Begin Class AZeroLockGameMode
void AZeroLockGameMode::StaticRegisterNativesAZeroLockGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AZeroLockGameMode);
UClass* Z_Construct_UClass_AZeroLockGameMode_NoRegister()
{
	return AZeroLockGameMode::StaticClass();
}
struct Z_Construct_UClass_AZeroLockGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "ZeroLockGameMode.h" },
		{ "ModuleRelativePath", "ZeroLockGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AZeroLockGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AZeroLockGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_ZeroLock,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AZeroLockGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AZeroLockGameMode_Statics::ClassParams = {
	&AZeroLockGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AZeroLockGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AZeroLockGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AZeroLockGameMode()
{
	if (!Z_Registration_Info_UClass_AZeroLockGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AZeroLockGameMode.OuterSingleton, Z_Construct_UClass_AZeroLockGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AZeroLockGameMode.OuterSingleton;
}
template<> ZEROLOCK_API UClass* StaticClass<AZeroLockGameMode>()
{
	return AZeroLockGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AZeroLockGameMode);
AZeroLockGameMode::~AZeroLockGameMode() {}
// End Class AZeroLockGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AZeroLockGameMode, AZeroLockGameMode::StaticClass, TEXT("AZeroLockGameMode"), &Z_Registration_Info_UClass_AZeroLockGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AZeroLockGameMode), 2556350059U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_119686204(TEXT("/Script/ZeroLock"),
	Z_CompiledInDeferFile_FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_ZeroLock_Source_ZeroLock_ZeroLockGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
