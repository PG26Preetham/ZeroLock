// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Base_GA_TargetActors.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
//#include "GAS/Tasks/GAST_WaitTargetData.h"
#include "ZeroLock/ZeroLockCharacter.h"
// Helper Macros
#if 1
float MacroDurationX = 2.f;
#define ZLOG(x) GEngine->AddOnScreenDebugMessage(-1, MacroDurationX ? MacroDurationX : -1.f, FColor::Yellow, x);
#define ZPOINT(x, c) DrawDebugPoint(GetWorld(), x, 10, c, !MacroDurationX, MacroDurationX);
#define ZLINE(x1, x2, c) DrawDebugLine(GetWorld(), x1, x2, c, !MacroDurationX, MacroDurationX);
#define ZCAPSULE(x, c) DrawDebugCapsule(GetWorld(), x, CapHH(), CapR(), FQuat::Identity, c, !MacroDurationX, MacroDurationX);
#else
#define ZLOG(x)
#define ZPOINT(x, c)
#define ZLINE(x1, x2, c)
#define ZCAPSULE(x, c)
#endif

UBase_GA_TargetActors::UBase_GA_TargetActors()
{
}

void UBase_GA_TargetActors::TargetConfirmed(const FGameplayAbilityTargetDataHandle& Data)
{
	ZLOG("Confirmed");
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UBase_GA_TargetActors::TargetCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	ZLOG("Cancelled");
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void UBase_GA_TargetActors::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	
	FVector StartLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(StartLocation);

	AGameplayAbilityTargetActor_Trace* TargetActor = GetWorld()->SpawnActor<AGameplayAbilityTargetActor_Trace>(Targetclass,SpawnTransform);

	TargetActor->OwningAbility = this;
	TargetActor->PrimaryPC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
	TargetActor->SetOwner(GetAvatarActorFromActorInfo());

	// ✅ Set StartLocation
	TargetActor->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	TargetActor->StartLocation.LiteralTransform = SpawnTransform;

	FGameplayTargetDataFilter* Filter = new FGameplayTargetDataFilter();
	Filter->RequiredActorClass = AZeroLockCharacter::StaticClass();
	Filter->SelfFilter = ETargetDataFilterSelf::TDFS_NoSelf;

	TSharedPtr<FGameplayTargetDataFilter> TargetFilter(Filter);
	//Filter->SelfActor = GetAvatarActorFromActorInfo();

	FGameplayTargetDataFilterHandle FilterHandle(TargetFilter);
	
	TargetActor->Filter = FilterHandle;
	// ✅ Start targeting
	TargetActor->StartTargeting(this);

	TargetActor->ReticleClass = ReticleClassX;

	// ✅ Begin wait task
	UAbilityTask_WaitTargetData* Task = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this,FName(TEXT("Targeting")),	EGameplayTargetingConfirmation::UserConfirmed,TargetActor);

	Task->ValidData.AddDynamic(this, &UBase_GA_TargetActors::TargetConfirmed);
	Task->Cancelled.AddDynamic(this, &UBase_GA_TargetActors::TargetCancelled);

	Task->ReadyForActivation();

}

void UBase_GA_TargetActors::WaitFinish()
{
	ZLOG("Wait over");
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero)
	{
		Hero->GetAbilitySystemComponent()->LocalInputConfirm();
		
	}
}
