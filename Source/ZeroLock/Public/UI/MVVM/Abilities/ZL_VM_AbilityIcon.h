//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_AbilityIcon.generated.h"

class UBaseGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityLevelChanged, UZL_VM_AbilityIcon*,AbilityIconVM,int32,newLevel);

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_AbilityIcon : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	UTexture2D* IconTexture;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "UI")
	float CooldownPercent = 0.f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "UI")
	FText CooldownText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "UI")
	bool bIsOnCooldown = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	bool bHasStacks =false;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	int32 StackNum =0;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	int32 AbilityLevel = 1;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	FText AbilityDescription;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	FText AbilityName;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	FText AbilityLevel1Description;
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	FText AbilityLevel2Description;
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	FText AbilityLevel3Description;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	float MaxCoolDownTime;


	void SetAbilityDescription(FText mAbilityDescription);
	void SetAbilityLevel1Description(FText mAbilityLevel1Description);
	void SetAbilityLevel2Description(FText mAbilityLevel2Description);
	void SetAbilityLevel3Description(FText mAbilityLevel3Description);
	void SetMaxCoolDownTime(float mMaxCoolDownTime);
	void SetAbilityName(FText mAbilityName);

	FText GetAbilityName() const{return AbilityName;}
	FText GetAbilityDescription()const { return AbilityDescription; }
	FText GetAbilityLevel1Description()const { return AbilityLevel1Description; }
	FText GetAbilityLevel2Description()const { return AbilityLevel2Description; }
	FText GetAbilityLevel3Description()const { return AbilityLevel3Description; }
	float GetMaxCoolDownTime()const { return MaxCoolDownTime; }
	
	void SetIconTexture(UTexture2D* NewIcon) ;
	UTexture2D* GetIconTexture()const  { return IconTexture; }
	void UpdateCooldown(float Remaining, float Duration);

	void SetbHasStacks(bool bstacks);
	bool GetbHasStacks()const { return bHasStacks; }

	

	void SetStackNum(int32 NewStackNum);
	int32 GetStackNum()const { return StackNum; }

	void IncrementAbilityLevel();
	void SetAbilityLevel(int32 NewAbilityLevel);
	int32 GetAbilityLevel()const { return AbilityLevel; }


	UPROPERTY(BlueprintAssignable)
	FAbilityLevelChanged OnAbilityLevelChanged;
};
