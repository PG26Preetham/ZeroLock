// Copyright Preetham Mukundan (C) 2026


#include "Apollo/ZL_Apollo_ItaniLoSahn.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "GAS/Tasks/ZL_WaitDelay_Task.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Apollo_ItaniLoSahn::UZL_Apollo_ItaniLoSahn()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UZL_Apollo_ItaniLoSahn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    if (Hero)
    {
        Hero->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        HeightSave= Hero->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        RadiusSave= Hero->GetCapsuleComponent()->GetScaledCapsuleRadius();
    }

    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
    }

    WaitTimeTask = UZL_WaitDelay_Task::WaitDealyWithProgressBar(this, ChargeupTime);
    WaitTimeTask->OnProgress.AddDynamic(this,&UZL_Apollo_ItaniLoSahn::UpdateProgressionTimer);
    WaitTimeTask->OnStarted.AddDynamic(this,&UZL_Apollo_ItaniLoSahn::StartProgressionTimer);
    WaitTimeTask->OnEnd.AddDynamic(this,&UZL_Apollo_ItaniLoSahn::StopProgressionTimer);
    WaitTimeTask->OnFinished.AddDynamic(this, &UZL_Apollo_ItaniLoSahn::ChargedDone);
    WaitTimeTask->ReadyForActivation();
}

void UZL_Apollo_ItaniLoSahn::ChargedDone()
{
    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    if (!Hero) return;
    DashDetectionSphere = NewObject<USphereComponent>(Hero, TEXT("DashDetectionSphere"));
    DashDetectionSphere->RegisterComponent();
    DashDetectionSphere->SetSphereRadius(DamageRadius);
    DashDetectionSphere->AttachToComponent(Hero->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
    DashDetectionSphere->SetHiddenInGame(false);
    DashDetectionSphere->SetVisibility(true);
    DashDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DashDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    DashDetectionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
    UAbilityTask_WaitInputRelease* InputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
    InputReleaseTask->OnRelease.AddDynamic(this, &UZL_Apollo_ItaniLoSahn::ReleaseInputRelease);
    InputReleaseTask->ReadyForActivation();
}

void UZL_Apollo_ItaniLoSahn::ReleaseInputRelease(float TimeHeld)
{
   AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    if (!Hero) return;

    Hero->GetCapsuleComponent()->SetCapsuleHalfHeight(0.1);
    Hero->GetCapsuleComponent()->SetCapsuleRadius(0.1);
    Hero->GetCameraBoom()->bEnableCameraLag = true;
    Hero->GetCameraBoom()->CameraLagSpeed = 1.0f;

    FVector CameraLoc; FRotator CameraRot;
    Hero->GetActorEyesViewPoint(CameraLoc, CameraRot);
    FVector ForwardDir = CameraRot.Vector();

    FVector TraceStart = Hero->GetActorLocation();
    FVector TraceEnd = TraceStart + (ForwardDir * MaxTargetRange);
    
    TArray<AActor*> IgnoreList;
    IgnoreList.Add(Hero);
    
    FHitResult DashHit;
    UKismetSystemLibrary::CapsuleTraceSingle(this, TraceStart, TraceEnd, 0.1, 0.1, UEngineTypes::ConvertToTraceType(ECC_Camera), false, IgnoreList, EDrawDebugTrace::ForDuration, DashHit, true);

    FVector FinalTarget = TraceEnd;

    CurrentTargetData.Clear();
    
    TArray<FHitResult> OutHits;

    FCollisionShape SweepSphere = FCollisionShape::MakeSphere(DamageRadius); 
    
    bool bHitSomething = GetWorld()->SweepMultiByChannel(OutHits, TraceStart, FinalTarget, FQuat::Identity, ECC_Pawn, SweepSphere);
/*
    if (bHitSomething)
    {
        for (const FHitResult& Hit : OutHits)
        {
            AActor* HitActor = Hit.GetActor();
            if (AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(HitActor))
            {
                if (!Hero->IsOnSameTeam(Villan) && !UniqueHitActors.Contains(HitActor))
                {
                    UniqueHitActors.Add(HitActor);

                    FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit();
                    NewData->HitResult = Hit;
                    CurrentTargetData.Add(NewData);
                    Villan->GetAbilitySystemComponent()->AddGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Apollo.ItaniLoSahn",false));
                    Villan->CustomTimeDilation = 0.05f; 
                }
            }
        }
    }
    */
  
    

    UniqueHitActors.Empty();
    DashDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UZL_Apollo_ItaniLoSahn::OnDashSphereOverlap);
    TArray<AActor*> InitialOverlaps;
    DashDetectionSphere->GetOverlappingActors(InitialOverlaps, AZeroLockCharacter::StaticClass());

    for (AActor* OverlappedActor : InitialOverlaps)
    {
        OnDashSphereOverlap(DashDetectionSphere, OverlappedActor, nullptr, 0, false, FHitResult());
    }
    if (IsLocallyControlled())
    {
        GetAbilitySystemComponentFromActorInfo()->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), CurrentTargetData, FGameplayTag(), FPredictionKey());
    }

 
    MoveToTask = UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(this, "MoveToTargetTask", FinalTarget, 0.2f, false, MOVE_None, false, nullptr, ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0);
    
    MoveToTask->OnTimedOutAndDestinationReached.AddDynamic(this, &UZL_Apollo_ItaniLoSahn::OnMoveToDone);
    MoveToTask->ReadyForActivation();
    
    UAbilityTask_WaitDelay* TempWaitTask = UAbilityTask_WaitDelay::WaitDelay(this,0.3);
    TempWaitTask->OnFinish.AddDynamic(this,&UZL_Apollo_ItaniLoSahn::OnMoveToDone);
    TempWaitTask->ReadyForActivation();
    
}

void UZL_Apollo_ItaniLoSahn::OnMoveToDone()
{
    if (MoveToTask && MoveToTask->IsActive())
    {
        MoveToTask->OnTimedOutAndDestinationReached.RemoveDynamic(this, &UZL_Apollo_ItaniLoSahn::OnMoveToDone);
        MoveToTask->EndTask();
    }
    UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 1.0f);
    WaitDelayTask->OnFinish.AddDynamic(this, &UZL_Apollo_ItaniLoSahn::OnExecutecallback);
    WaitDelayTask->ReadyForActivation();
}

void UZL_Apollo_ItaniLoSahn::OnTimeFinish()
{
}

void UZL_Apollo_ItaniLoSahn::OnExecutecallback()
{
    if (!HasAuthority(&CurrentActivationInfo)) return;

    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    if (!Hero) return;
    
    for (int32 i = 0; i < CurrentTargetData.Num(); i++)
    {
        TArray<TWeakObjectPtr<AActor>> TargetActors = CurrentTargetData.Get(i)->GetActors();
        for (auto& ActorPtr : TargetActors)
        {
            if (AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(ActorPtr.Get()))
            {
                float DamageAmp = Damage.GetValueAtLevel(GetAbilityLevel());
                
                if (Villan->GetMyAttributeSet())
                {
                    float HealthPercent = Villan->GetMyAttributeSet()->GetCurrentHealth() / Villan->GetMyAttributeSet()->GetMaximumHealth();
                    if (HealthPercent < 0.5f) DamageAmp *= 2.0f;
                }

                Villan->CustomTimeDilation = 1.0f;
                Villan->GetAbilitySystemComponent()->RemoveGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Apollo.ItaniLoSahn",false));
                   
                
                if (UBaseCharAbilitySystemComponent* HeroASC = Hero->GetMyAbilitySystemComp())
                {
                    HeroASC->ApplySpiritDamage(Villan->GetMyAbilitySystemComp(), DamageAmp);
                }
            }
        }
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UZL_Apollo_ItaniLoSahn::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    if (Hero)
    {
        Hero->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
        Hero->GetCameraBoom()->bEnableCameraLag = false;
        Hero->GetCameraBoom()->CameraLagSpeed = 10.0f;
        Hero->GetCapsuleComponent()->SetCapsuleHalfHeight(HeightSave);
        Hero->GetCapsuleComponent()->SetCapsuleRadius(RadiusSave);
    }
    if (DashDetectionSphere)
    {
        DashDetectionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &UZL_Apollo_ItaniLoSahn::OnDashSphereOverlap);
        DashDetectionSphere->DestroyComponent();
        DashDetectionSphere = nullptr;
    }

    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
    }

    CurrentTargetData.Clear();
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UZL_Apollo_ItaniLoSahn::OnDashSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(OtherActor);

    if (Hero && Villan && !UniqueHitActors.Contains(Villan))
    {
        if (!Hero->IsOnSameTeam(Villan))
        {
            UniqueHitActors.Add(Villan);

            // Create Target Data for the final execution
            FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit();

            NewData->HitResult.HitObjectHandle = FActorInstanceHandle(Villan);
            CurrentTargetData.Add(NewData);

            Villan->GetAbilitySystemComponent()->AddGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Apollo.ItaniLoSahn"));
            Villan->CustomTimeDilation = 0.05f;
        }
    }
    if (IsLocallyControlled())
    {
        GetAbilitySystemComponentFromActorInfo()->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), CurrentTargetData, FGameplayTag(), FPredictionKey());
    }
}
