//Copyright Preetham Mukundan (C) 2026


#include "GAS/BaseGameplayAbility.h"

#include "GameplayTagsManager.h"
#include "KismetTraceUtils.h"
#include "Engine/OverlapResult.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "GAS/ZL_GameplayTags.h"
#include "GAS/ZL_GE_BaseCooldown.h"
#include "ZeroLock/ZeroLockCharacter.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLLock.Abilities"),false));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CooldownGameplayEffectClass = UZL_GE_BaseCooldown::StaticClass();
}

void UBaseGameplayAbility::SetSlot(EGameplayAbilitySlot slot)
{
	Slot = slot;
}

void UBaseGameplayAbility::ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> GEToApply,
                                                       UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* SourceASC)
{
	if (GEToApply == nullptr || TargetASC == nullptr || SourceASC == nullptr)
	{
		ZLOG("CancelledInBaseGameplayAbility::ApplyGameplyEffectToTarget");
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if (HasAuthority(&CurrentActivationInfo))
	{
		//ZLOG("Found Damage");
		FGameplayEffectContextHandle EffectContext =SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
				
	
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GEToApply, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			ZLOG("ApplyingEffect::ApplyGameplyEffectToTarget");
			FActiveGameplayEffectHandle GEHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
		}
	}
}

void UBaseGameplayAbility::ApplyGameplyEffectToSelf(TSubclassOf<UGameplayEffect> GEToApply,
	UAbilitySystemComponent* SourseASC)
{
	
	if (GEToApply == nullptr || SourseASC == nullptr)
	{
		ZLOG("CancelledInBaseGameplayAbility::ApplyGameplyEffectToSelf");
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if (HasAuthority(&CurrentActivationInfo))
	{
		FGameplayEffectContextHandle EffectContext =SourseASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);


		FGameplayEffectSpecHandle SpecHandle = SourseASC->MakeOutgoingSpec(GEToApply, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			ZLOG("BuffDone");
			FActiveGameplayEffectHandle GEHandle = SourseASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

UAbilitySystemComponent* UBaseGameplayAbility::GetOwnerASC()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero )
	{
		return Hero->GetAbilitySystemComponent();
	}
	return nullptr;
}



void UBaseGameplayAbility::SetInputID(EGASAbilityInputID in)
{
	AbilityInputID=in;
}

void UBaseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(  "Ability.Cooldown.Duration" )), GetCoolDownTime());
		FActiveGameplayEffectHandle GEHandle=ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		if (GEHandle.IsValid())
		{
			ZLOG("CustomSetByCaller");
		}
	}
}

float UBaseGameplayAbility::GetCoolDownTime() const
{
	if (!GetCurrentActorInfo()) 
	{
		return 0.0f; 
	}
	float cooldownTime = CooldownDuration.GetValueAtLevel(GetAbilityLevel());
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero)
	{ 
		if (!Hero->GetMyAttributeSet())
		{
			return cooldownTime;
		}
		float CDR = Hero->GetMyAttributeSet()->GetCooldownReduction();
		return (cooldownTime*(1-CDR/100));
	}
	return cooldownTime;
}

const FGameplayTagContainer* UBaseGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

bool UBaseGameplayAbility::ConeTraceMulti(const UObject* WorldContextObject, const FVector Start,
	const FRotator Direction, float ConeHeight, float ConeHalfAngle, ETraceTypeQuery TraceChannel, bool bTraceComplex,
	const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits,TArray<AZeroLockCharacter*>& OutVillans,
	bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	OutHits.Reset();
	OutVillans.Reset();
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ConeTraceMulti), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
 
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}
 
	TArray<FHitResult> TempHitResults;
	const FVector End = Start + (Direction.Vector() * ConeHeight);
	const double ConeHalfAngleRad = FMath::DegreesToRadians(ConeHalfAngle);
	// r = h * tan(theta / 2)
	const double ConeBaseRadius = ConeHeight * tan(ConeHalfAngleRad);
	const FCollisionShape SphereSweep = FCollisionShape::MakeSphere(ConeBaseRadius);
 
	// Perform a sweep encompassing an imaginary cone.
	World->SweepMultiByChannel(TempHitResults, Start, End, Direction.Quaternion(), CollisionChannel, SphereSweep, Params);
 
	// Filter for hits that would be inside the cone.
	for (FHitResult& HitResult : TempHitResults)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - Start).GetSafeNormal();
		const double Dot = FVector::DotProduct(Direction.Vector(), HitDirection);
		// theta = arccos((A • B) / (|A|*|B|)). |A|*|B| = 1 because A and B are unit vectors.
		const double DeltaAngle = FMath::Acos(Dot);
 
		// Hit is outside the angle of the cone.
		if (DeltaAngle > ConeHalfAngleRad)
		{
			continue;
		}
 
		const double Distance = (HitResult.ImpactPoint - Start).Length();
		// Hypotenuse = adjacent / cos(theta)
		const double LengthAtAngle = ConeHeight / cos(DeltaAngle);
 
		// Hit is beyond the cone. This can happen because we sweep with spheres, which results in a cap at the end of the sweep.
		if (Distance > LengthAtAngle)
		{
			continue;
		}
		if (AZeroLockCharacter* villan = Cast<AZeroLockCharacter>(HitResult.GetActor()))
		{
			OutVillans.AddUnique(villan);
		}
		OutHits.Add(HitResult);
	}
 
#if ENABLE_DRAW_DEBUG
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		// Cone trace.
		const double ConeSlantHeight = FMath::Sqrt((ConeBaseRadius * ConeBaseRadius) + (ConeHeight * ConeHeight)); // s = sqrt(r^2 + h^2)
		DrawDebugCone(World, Start, Direction.Vector(), ConeSlantHeight, ConeHalfAngleRad, ConeHalfAngleRad, 32, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
 
		// Uncomment to see the trace we're actually performing.
		// DrawDebugSweptSphere(World, Start, End, ConeBaseRadius, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
 
		// Successful hits.
		for (const FHitResult& Hit : OutHits)
		{
			DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, true, Hit, TraceHitColor, TraceHitColor, DrawTime);
		}
 
		// Uncomment to see hits from the sphere sweep that were filtered out.
		// for (const FHitResult& Hit : TempHitResults)
		// {
		//     if (!OutHits.ContainsByPredicate([Hit](const FHitResult& Other)
		//     {
		//         return (Hit.GetActor() == Other.GetActor()) &&
		//                (Hit.ImpactPoint == Other.ImpactPoint) &&
		//                (Hit.ImpactNormal == Other.ImpactNormal);
		//     }))
		//     {
		//         DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, false, Hit, FColor::Red, FColor::Red, DrawTime);
		//     }
		// }
	}
#endif // ENABLE_DRAW_DEBUG
 
	return (OutHits.Num() > 0);
}

bool UBaseGameplayAbility::GetConeOverlap(UWorld* World, TArray<FOverlapResult>& OutResults, const FVector& Origin,
                                          const FVector& Direction, float Radius, float AngleDegrees, ECollisionChannel Channel)
{
	if (!World) return false;

	DrawDebugSphere(GetWorld(), Origin, Radius, 20, FColor::Yellow, false,10);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(GetOwningActorFromActorInfo());
	
	TArray<FOverlapResult> SphereResults;
	bool bHit = World->OverlapMultiByChannel(SphereResults, Origin, FQuat::Identity, Channel, Sphere, Params);

	if (!bHit) return false;


	float HalfAngleRadians = FMath::DegreesToRadians(AngleDegrees * 0.5f);
	float CosThreshold = FMath::Cos(HalfAngleRadians);
	FVector NormalizedDir = Direction.GetSafeNormal();

	for (const FOverlapResult& Result : SphereResults)
	{
		AActor* OverlappedActor = Result.GetActor();
		if (!OverlappedActor) continue;

		FVector TargetLocation = OverlappedActor->GetActorLocation();
		FVector DirToTarget = (TargetLocation - Origin).GetSafeNormal();

		
		float Dot = FVector::DotProduct(NormalizedDir, DirToTarget);

		if (Dot >= CosThreshold)
		{
			OutResults.Add(Result);
		}
	}

	return OutResults.Num() > 0;
}




