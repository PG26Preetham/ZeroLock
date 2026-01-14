//Copyright Preetham Mukundan (C) 2026


#include "Drifter/ZL_Drifter_BloodScent.h"

#include "AbilitySystemComponent.h"
#include "Zero_BasePlayerState.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"
#include "Gamemode/Zero_BaseGameState.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Drifter_BloodScent::UZL_Drifter_BloodScent()
{
}

void UZL_Drifter_BloodScent::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthority(&ActivationInfo)) return;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	DetectionSphere = NewObject<USphereComponent>(Avatar);
	DetectionSphere->SetSphereRadius(DetectionRadius);
	DetectionSphere->SetupAttachment(Avatar->GetRootComponent());
    
	// Set collision to only overlap Pawns
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    
	DetectionSphere->RegisterComponent();

	DetectionSphere->SetHiddenInGame(false);
	DetectionSphere->SetVisibility(true);
	// 2. Bind Overlaps
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDetectionOverlapBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnDetectionOverlapEnd);
	GetWorld()->GetTimerManager().SetTimer(LogicTimerHandle, this, &ThisClass::UpdateServerLogic, 0.2f, true);

	UAbilityTask_WaitGameplayEvent* DeathTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Kill"));
	DeathTask->EventReceived.AddDynamic(this, &ThisClass::OnTargetDied);
	DeathTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* AssistTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Assist"));
	AssistTask->EventReceived.AddDynamic(this, &ThisClass::OnTargetDied);
	AssistTask->ReadyForActivation();
}

void UZL_Drifter_BloodScent::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetTimerManager().ClearTimer(LogicTimerHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UZL_Drifter_BloodScent::HasIsolatedTarget(bool HasISoLatedTargetNear)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !IsolatedTargetEffectClass) return;

	if (HasISoLatedTargetNear)
	{
		if (!BloodScentEffectHandle.IsValid())
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(IsolatedTargetEffectClass, GetAbilityLevel(), EffectContext);
            
			if (SpecHandle.IsValid())
			{
				BloodScentEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	else
	{
		if (BloodScentEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(BloodScentEffectHandle);
			BloodScentEffectHandle.Invalidate();
		}
	}
}

void UZL_Drifter_BloodScent::UpdateServerLogic()
{
	FGameplayTag IsolatedTag = FGameplayTag::RequestGameplayTag("Zerolock.Drifter.BloodScent");
    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    AZero_BasePlayerState* MyPS = Hero ? Cast<AZero_BasePlayerState>(Hero->GetPlayerState()) : nullptr;
    if (!MyPS) return;
	int numberOfIsolatedTarget = 0;
    for (int32 i = OverlappingEnemies.Num() - 1; i >= 0; --i)
    {
        AZeroLockCharacter* CurrentEnemy =OverlappingEnemies[i];
        if (!IsValid(CurrentEnemy)) { OverlappingEnemies.RemoveAt(i); continue; }


        AZero_BasePlayerState* EnemyPS = Cast<AZero_BasePlayerState>(CurrentEnemy->GetPlayerState());
        if (!EnemyPS || EnemyPS->TeamID == MyPS->TeamID) continue;

        bool bIsIsolated = true;
        
        AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>();
        if (GS)
        {
            const TArray<AZero_BasePlayerState*>& EnemyTeamArray = (EnemyPS->TeamID == ETeamID::TeamRed) ? GS->TeamRedArray : GS->TeamBlueArray;
            
            for (AZero_BasePlayerState* TeammatePS : EnemyTeamArray)
            {
                if (!TeammatePS || TeammatePS == EnemyPS) continue;
                AActor* TeammatePawn = TeammatePS->GetPawn();
                if (!TeammatePawn) continue;

                if (FVector::Dist(CurrentEnemy->GetActorLocation(), TeammatePawn->GetActorLocation()) < IsolationRadius)
                {
                    bIsIsolated = false;
                    break;
                }
            }
        }
    	
        UAbilitySystemComponent* EnemyASC = CurrentEnemy->GetAbilitySystemComponent();
        if (EnemyASC)
        {
            if (bIsIsolated)
            {
            	numberOfIsolatedTarget++;
                if (!EnemyASC->HasMatchingGameplayTag(IsolatedTag))
                {
                    FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
                    FGameplayEffectSpecHandle Spec = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(IsolatedMarkEffectClass, 1.f, Context);
                    EnemyASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
                }
            }
            else
            {
                EnemyASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(IsolatedTag));
            	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(IsolatedTag));
            	EnemyASC->RemoveActiveEffects(Query);
            	EnemyASC->ExecuteGameplayCue(IsolatedTag, FGameplayCueParameters());
            }
        }
    }
	if (numberOfIsolatedTarget > 0)
	{
		if (!isHavingBuff)
		{
			HasIsolatedTarget(true);
		}
	}
	else
	{
		HasIsolatedTarget(false);
	}
}

void UZL_Drifter_BloodScent::OnTargetDied(const FGameplayEventData Payload)
{
	ZLOG("DeathTrigger");
	const AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Payload.Target);
	const UAbilitySystemComponent* VictimASC = Villan->GetAbilitySystemComponent();
	if (VictimASC && VictimASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Zerolock.Drifter.BloodScent")))
	{
		if (OnKillStackEffectClass)
		{
			FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(OnKillStackEffectClass, 1.f, Context);
			for (int i = 0 ;i<KillStackReward;i++)
			{
				GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
			UE_LOG(LogTemp, Log, TEXT("Drifter: Isolated enemy killed. Stack applied!"));
		}
	}
}

void UZL_Drifter_BloodScent::OnTargetAssist(const FGameplayEventData Payload)
{
	ZLOG("DeathTrigger");
	const AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Payload.Target);
	const UAbilitySystemComponent* VictimASC = Villan->GetAbilitySystemComponent();
	if (VictimASC && VictimASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Zerolock.Drifter.BloodScent")))
	{
		if (OnKillStackEffectClass)
		{
			FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(OnKillStackEffectClass, 1.f, Context);
			for (int i = 0 ;i<AssistStackReward;i++)
			{
				GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
			UE_LOG(LogTemp, Log, TEXT("Drifter: Isolated enemy killed. Stack applied!"));
		}
	}
}

void UZL_Drifter_BloodScent::GetEnemyPawns(TArray<AZeroLockCharacter*>& OutEnemies)
{

	AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>();
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	AZero_BasePlayerState* PS = Hero ? Cast<AZero_BasePlayerState>(Hero->GetPlayerState()) : nullptr;

	if (!GS || !PS) return;

	const TArray<AZero_BasePlayerState*>& EnemyArray = (PS->TeamID == ETeamID::TeamRed) ? GS->TeamBlueArray : GS->TeamRedArray;

	for (AZero_BasePlayerState* EnemyPS : EnemyArray)
	{
		if (EnemyPS && EnemyPS->GetPawn())
		{
			if (AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(EnemyPS->GetPawn()))
			{
				OutEnemies.AddUnique(Villan);
			}
			
		}
	}
}

void UZL_Drifter_BloodScent::OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZeroLockCharacter* Enemy = Cast<AZeroLockCharacter>(OtherActor);
	if (Enemy && Enemy != GetAvatarActorFromActorInfo())
	{
		OverlappingEnemies.Add(Enemy);
	}
}

void UZL_Drifter_BloodScent::OnDetectionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AZeroLockCharacter* Villan =Cast<AZeroLockCharacter>(OtherActor))
	{
			UAbilitySystemComponent* EnemyASC = Villan->GetAbilitySystemComponent();
			if (EnemyASC)
			{
				HasIsolatedTarget(false);
				FGameplayTag IsolatedTag = FGameplayTag::RequestGameplayTag("Zerolock.Drifter.BloodScent");
				EnemyASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(IsolatedTag));
				FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(IsolatedTag));
				EnemyASC->RemoveActiveEffects(Query);
				EnemyASC->ExecuteGameplayCue(IsolatedTag, FGameplayCueParameters());
			}
		OverlappingEnemies.Remove(Villan);
		
	}
}
