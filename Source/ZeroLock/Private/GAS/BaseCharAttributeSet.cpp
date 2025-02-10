// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseCharAttributeSet.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ZeroLock/ZeroLockCharacter.h"

UBaseCharAttributeSet::UBaseCharAttributeSet()
{
	MaximumHealth = 0.0f;
	CurrentHealth = 0.0f;
	HealthRegeneration = 0.0f;
	CurrentSpeed = 0.0f;
	CurrentJump = 0.0f;
	Soul =0;
}
void UBaseCharAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaximumHealthAttribute())
	{
		AdjustAttributeForMaxChange(CurrentHealth, MaximumHealth, NewValue, GetCurrentHealthAttribute());
	}

}

void UBaseCharAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
 
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		FString TheFloatStr = "Dam=" + FString::SanitizeFloat(LocalDamageDone);
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, *TheFloatStr);

		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{

			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() - LocalDamageDone;
			//FString TheFloatStr = FString::SanitizeFloat(NewHealth);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, *TheFloatStr);
			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));
			// TheFloatStr = FString::SanitizeFloat(GetCurrentHealth());
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *TheFloatStr);
			if (GetCurrentHealth() <= 0)
			{
				//Handle Death
			}
		}
	}
	

	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Store a local copy of the amount of Healing done and clear the Healing attribute.
		const float LocalHealingDone = GetHealing();

		SetHealing(0.f);

		if (LocalHealingDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() + LocalHealingDone;

			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));
		}
	}

	else if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaximumHealth()));


	}

	else if (Data.EvaluatedData.Attribute == GetHealthRegenerationAttribute())
	{
		SetHealthRegeneration(FMath::Clamp(GetHealthRegeneration(), 0.0f, GetMaximumHealth()));
	}
}

void UBaseCharAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);


	AZeroLockCharacter* TargetChar = Cast<AZeroLockCharacter>(GetOwningActor());
	if (!TargetChar)
	{
		return;
	}
	if (Attribute == GetCurrentSpeedAttribute())
	{
	
		
		TargetChar->GetCharacterMovement()->MaxWalkSpeed = NewValue;
	
	}

	else if (Attribute == GetCurrentJumpAttribute())
	{
	
		TargetChar->GetCharacterMovement()->JumpZVelocity = NewValue;
	
	}
	/*else if (Attribute == GetMaximumHealthAttribute())
	{
		float HP = GetCurrentHealth();
		float Diff = NewValue - OldValue;
		FString TheFloatStr = "Diff="+FString::SanitizeFloat(Diff);
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, *TheFloatStr);
		TheFloatStr = "HP=" + FString::SanitizeFloat(HP);
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, *TheFloatStr);
		 TheFloatStr = "Diff+HP=" + FString::SanitizeFloat(HP + Diff);
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Orange, *TheFloatStr);
		
	}*/
}

void UBaseCharAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCharAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCharAttributeSet, MaximumHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCharAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCharAttributeSet, CurrentSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCharAttributeSet, CurrentJump, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseCharAttributeSet,Soul, COND_None, REPNOTIFY_Always);
}

void UBaseCharAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, const float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();

	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		// Change current value to maintain the Current Value / Maximum Value percentage.
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		
		 float NewDelta= (CurrentMaxValue > 0.f) ? FMath::Clamp(CurrentValue,0.0,NewMaxValue) : NewMaxValue;
		if (NewMaxValue > CurrentMaxValue)
		{
			NewDelta = (CurrentMaxValue > 0.f) ? CurrentValue + (NewMaxValue - CurrentMaxValue) : NewMaxValue;
		}
		

		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Override, NewDelta);
	}
}

void UBaseCharAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData & OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCharAttributeSet, CurrentHealth, OldValue);
}
void UBaseCharAttributeSet::OnRep_CurrentSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCharAttributeSet, CurrentSpeed, OldValue);
}
void UBaseCharAttributeSet::OnRep_CurrentJump(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCharAttributeSet, CurrentJump, OldValue);
}

void UBaseCharAttributeSet::OnRep_MaximumHealth(const FGameplayAttributeData & OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCharAttributeSet, MaximumHealth, OldValue);
}

void UBaseCharAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData & OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCharAttributeSet, HealthRegeneration, OldValue);
}
void UBaseCharAttributeSet::OnRep_Souls(const FGameplayAttributeData & OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseCharAttributeSet, Soul, OldValue);
}

