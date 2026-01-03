// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeroSelector/ZL_HeroSelectionScreen.h"

#include "CommonTileView.h"
#include "CharacterSelector/ZL_CharacterSelectionSubsystem.h"
#include "CharacterSelector/ZL_CharacterSelectionVM.h"
#include "CharacterSelector/ZL_Character_Data_Asset.h"

#include "UI/ItemShop/ZL_ItemShop_Base.h"
#include "ZeroLock/ZeroLock.h"

#include "MVVMSubsystem.h"
#include "View/MVVMView.h"

UZL_HeroSelectionScreen::UZL_HeroSelectionScreen()
{
}

void UZL_HeroSelectionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (UZL_CharacterSelectionSubsystem* Subsystem = GetSelectionSubsystem())
	{
		SelectionVM = Subsystem->SelectionVM;
		ZLOG("Set IN Widget");

		

			if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
			{

				if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
				{					

					if (bool bSuccess = WidgetView->SetViewModel(FName("Zl_CharacterSelectionVM"), Subsystem->SelectionVM))
					{
						UE_LOG(LogTemp, Log, TEXT("MVVM: Successfully linked Zl_CharacterSelectionVM!"));
					
						
						if (HeroSelectionList && Subsystem->SelectionVM)
						{
							HeroSelectionList->SetListItems(Subsystem->SelectionVM->GetCharacterList());
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("MVVM: Failed to link Viewmodel. Check the name in the BP!"));
					}
				}
			}
		}
		
	


	if (HeroSelectionList)
	{
		HeroSelectionList->OnItemIsHoveredChanged().AddUObject(this, &UZL_HeroSelectionScreen::HandleOnHoveredChanged);

		HeroSelectionList->OnItemClicked().AddUObject(this, &UZL_HeroSelectionScreen::HandleOnSelectionChanged);
		
	}
}

void UZL_HeroSelectionScreen::HandleOnHoveredChanged(UObject* Item, bool bIsHovered)
{
	if (bIsHovered && SelectionVM)
	{
		if (UZL_Character_Data_Asset* Data = Cast<UZL_Character_Data_Asset>(Item))
		{
			SelectionVM->SetCurrentCharacter(Data);
		}
	}
}

void UZL_HeroSelectionScreen::HandleOnSelectionChanged(UObject* Item)
{
}

FReply UZL_HeroSelectionScreen::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());


	FVector2D WidgetSize = InGeometry.GetLocalSize();


	FVector2D RelativeToCenter = LocalMousePos - (WidgetSize * 0.5f);

	if (SelectionVM)
	{
		SelectionVM->SetLocalMousePos(RelativeToCenter);
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UZL_HeroSelectionScreen::VM_PopulateList(TArray<UZL_Character_Data_Asset*> DataArray)
{
	
	HeroSelectionList->ClearListItems();
	for (UZL_Character_Data_Asset* Data : DataArray)
	{
		ZLOG_COLOR_TIME("VM_PopulateList",FColor::Red,2.0f);
		HeroSelectionList->AddItem(Data);
	}
}

UZL_CharacterSelectionSubsystem* UZL_HeroSelectionScreen::GetSelectionSubsystem() const
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UZL_CharacterSelectionSubsystem>();
	}
	return nullptr;
}
