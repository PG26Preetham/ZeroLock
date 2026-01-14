//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_AbilitiesContainer.generated.h"

class UZL_VM_AbilityIcon;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_AbilitiesContainer : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Slots")
	UZL_VM_AbilityIcon* Slot_Secondary;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Slots")
	UZL_VM_AbilityIcon* Slot_Ability1;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Slots")
	UZL_VM_AbilityIcon* Slot_Ability2;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Slots")
	UZL_VM_AbilityIcon* Slot_Ultimate;

	UZL_VM_AbilitiesContainer();

	void InitSlots();


	void SetSlot_Secondary(UZL_VM_AbilityIcon* iconSlotVM);

	void SetSlot_Ability1(UZL_VM_AbilityIcon* iconSlotVM);

	void SetSlot_Ability2(UZL_VM_AbilityIcon* iconSlotVM);
	
	void SetSlot_Ultimate(UZL_VM_AbilityIcon* iconSlotVM);


	UZL_VM_AbilityIcon* GetSlot_Secondary()const{return Slot_Secondary;};
	UZL_VM_AbilityIcon* GetSlot_Ability1()const{return  Slot_Ability1;};
	UZL_VM_AbilityIcon* GetSlot_Ability2()const{return Slot_Ability2;};
	UZL_VM_AbilityIcon* GetSlot_Ultimate()const{return  Slot_Ultimate;};
	
};
