// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_AbilityIcon.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseGameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLockCharacter.h"


void UZL_HUD_AbilityIcon::Setup(const UBaseGameplayAbility* abilityToBindTo, FGameplayAbilitySpec* InSpec,FGameplayAbilitySpecHandle SpecHandle)
{
	if (abilityToBindTo == nullptr) return;
	//if (Hero) return;

	Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	AbilityToBindTo = abilityToBindTo;
	AbilitySpec = InSpec;
	AbilitySpecHandleToStore = SpecHandle;
	BackgroundIcon->SetBrushFromSoftTexture(AbilityToBindTo->IconImage);
	BackgroundIcon->SetColorAndOpacity(FLinearColor::Gray);
	AbilityIcon->SetBrushFromTexture(AbilityToBindTo->IconImage);
	AbilityName->SetText(FText::FromString(AbilityToBindTo->AbilityName));
	AbilityDescription->SetText(FText::FromString(AbilityToBindTo->AbilityDescription));
	durationOfCooldown =AbilityToBindTo->CooldownDuration.GetValue();
	SetCoolDownTextToTextBlock(AbilityToBindTo->CooldownDuration.GetValue());
	bIsAbilityOnCoolDown = false;
	if (Hero)
	{
		AddToDelegate();
	}
}

void UZL_HUD_AbilityIcon::SetCoolDownTextToTextBlock(float intime)
{
	FString tCooldownText = FString::Printf(TEXT("%.1f"), intime);
	CoolDownTimerText->SetText(FText::FromString(tCooldownText));
}


void UZL_HUD_AbilityIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CoolDownTimerText->SetVisibility(ESlateVisibility::Collapsed);
	AbilityDescription->SetVisibility(ESlateVisibility::Collapsed);
	AbilityName->SetVisibility(ESlateVisibility::Collapsed);
}

void UZL_HUD_AbilityIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bIsAbilityOnCoolDown) return;
	
	if (GetCooldownRemainingForTag(TimeRemaining,TotalDuration))
	{
		SetCoolDownTextToTextBlock(TimeRemaining);
		float Alpha = 1 - FMath::Clamp(TimeRemaining / TotalDuration, 0.0f, 1.0f);
			
		float NewOpacity = FMath::Lerp(0.0f, 1.0f, Alpha);
		
		AbilityIcon->SetOpacity(NewOpacity);
	}
	
	
	
	
	
	
}

void UZL_HUD_AbilityIcon::AbilityActivated(UGameplayAbility* GameplayAbility)
{
	if (GameplayAbility == nullptr) return;
	if (GameplayAbility->GetCurrentAbilitySpecHandle() != AbilitySpecHandleToStore)return ;
	
	const FGameplayTagContainer* Tags = GameplayAbility->GetCooldownTags();
	if (Tags->IsValid() && !alreadyRegstered)
	{
		
		alreadyRegstered = true;
		for (FGameplayTag Tag : *Tags)
		{
			if (Tag.IsValid())
			{
				//AbilityCooldownTags.AddTag(Tag);
				Hero->GetMyAbilitySystemComp()->RegisterGameplayTagEvent(Tag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UZL_HUD_AbilityIcon::OnCooldownTagChanged);
			}
		}
	}
	AbilityActivatedEvent();
}



void UZL_HUD_AbilityIcon::OnCooldownTagChanged(FGameplayTag GameplayTag, int NewCount)
{
	FGameplayAbilitySpec* Spec = Hero->GetMyAbilitySystemComp()->FindAbilitySpecFromHandle(AbilitySpecHandleToStore);
	if (!Spec) return;
	AbilityCooldownTags=Spec->Ability->GetCooldownTags();
	if (!AbilityCooldownTags->HasTag(GameplayTag)) return;

	if (NewCount > 0)
	{
		
		AbilityCooldownStartEvent();
	}
	else
	{
		AbilityCooldownEndEvent();
	
	}
}

void UZL_HUD_AbilityIcon::AddToDelegate()
{
	if (AbilityToBindTo == nullptr) return;
	if (AbilitySpec == nullptr) return;
	if (AbilityToBindTo->TargetStyle == EGASTargetConfirmationStyle::Passive) return;
	if (!Hero) return;
	Hero->GetMyAbilitySystemComp()->AbilityActivatedCallbacks.AddUObject(this,&ThisClass::AbilityActivated);
}

float UZL_HUD_AbilityIcon::GetTimeRemaining()
{
	if (AbilitySpecHandleToStore.IsValid())
	{
		FGameplayAbilitySpec* Spec = Hero->GetMyAbilitySystemComp()->FindAbilitySpecFromHandle(AbilitySpecHandleToStore);
		if (Spec)
		{
			return Spec->Ability->GetCooldownTimeRemaining();
		}
		
	}
	return 0.0f;
}

void UZL_HUD_AbilityIcon::AbilityActivatedEvent()
{
	//TODO Add animation and handle change to UI on ability Activated

	//AbilityIcon->SetVisibility(ESlateVisibility::Collapsed);
}
void UZL_HUD_AbilityIcon::AbilityCooldownStartEvent()
{
	ZLOG("CoolDownStart");
	//GetAbilityCooldownAndRemaining(TotalDuration,TimeRemaining);
	GetCooldownRemainingForTag(TotalDuration,TimeRemaining);
	AbilityStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	ZLOG("StartTime-"+FString::SanitizeFloat(AbilityStartTime,1));
	bIsAbilityOnCoolDown = true;
	SetCoolDownTextToTextBlock(TimeRemaining);
	CoolDownTimerText->SetVisibility(ESlateVisibility::Visible);
	AbilityIcon->SetOpacity(0.0f);
	
	
}

void UZL_HUD_AbilityIcon::AbilityCooldownEndEvent()
{
	bIsAbilityOnCoolDown = false;
	ZLOG("CoolDownEnded");
	AbilityIcon->SetOpacity(1.0f);
	CoolDownTimerText->SetVisibility(ESlateVisibility::Collapsed);
}

void UZL_HUD_AbilityIcon::GetAbilityCooldownAndRemaining(float& Duration, float& timeleft)
{
	if (AbilitySpecHandleToStore.IsValid())
	{
		FGameplayAbilitySpec* Spec = Hero->GetMyAbilitySystemComp()->FindAbilitySpecFromHandle(AbilitySpecHandleToStore);
		if (Spec)
		{
			ZLOG("TimeSet to");
			Spec->Ability->GetCooldownTimeRemainingAndDuration(AbilitySpecHandleToStore,Spec->Ability->GetCurrentActorInfo(),timeleft,Duration);

			FString texttoprint = "Cooldown Remaining: " + FString::SanitizeFloat(timeleft,1)+ "     / Total Duration : " + FString::SanitizeFloat(Duration,1);
			ZLOG(texttoprint);
		}
		
	}
}
bool UZL_HUD_AbilityIcon::GetCooldownRemainingForTag(float & mTimeRemaining, float & CooldownDuration) const 
{
	if (Hero->GetMyAbilitySystemComp() && AbilityCooldownTags->Num() > 0)
	{
		
		mTimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*AbilityCooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = Hero->GetMyAbilitySystemComp()->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			mTimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}


