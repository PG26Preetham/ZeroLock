#include "ZeroEditorModule.h"
#include "LevelEditor.h"
#include "GAS/BaseGameplayAbility.h"

#define LOCTEXT_NAMESPACE "FZeroEditorModuleModule"

void FZeroEditorModuleModule::StartupModule()
{
    //Getting the ref
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    //MEnu Extender - add menus to the bar
    const TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());

    //add menu bar to it
    MenuExtender->AddMenuBarExtension("Help",
        EExtensionHook::Before,
        nullptr,
        FMenuBarExtensionDelegate::CreateRaw(this,&FZeroEditorModuleModule::AddMenu));


	//adding it in
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	CreatePropertyTool();
    
}

void FZeroEditorModuleModule::AddMenu(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(FText::FromString("ZeroLock"),
		FText::FromString("Custom ability menu abilities"),
		FNewMenuDelegate::CreateRaw(this,&FZeroEditorModuleModule::FillMenu));
}

void FZeroEditorModuleModule::FillMenu(FMenuBuilder& MenuBuilder)
{
	// MenuBuilder.AddMenuEntry(FText::FromString("BaseAbilityList"),
	// 	FText::FromString("Base Ability List"),
	// 	FSlateIcon(),
	// 	FUIAction(FExecuteAction::CreateRaw(this,&FZeroEditorModuleModule::PrintAllAbilities)));
}

// void FZeroEditorModuleModule::PrintAllAbilities()
// {
// 	TArray<UClass*> foundClasses;
// 		for (TObjectIterator<UClass> classIterator; classIterator; ++classIterator)
// 		{
// 			UClass* Class = *classIterator;
//
// 			if (Class && Class->IsChildOf(UBaseGameplayAbility::StaticClass()) && Class != UBaseGameplayAbility::StaticClass())
// 			{
// 				foundClasses.Add(Class);
// 			}
// 		}
//     
// 		for (UClass* Class : foundClasses)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Found Class: %s"), *Class->GetName());
// 			// Iterate through properties using TFieldIterator
// 			for (TFieldIterator<FProperty> PropertyIt(Class); PropertyIt; ++PropertyIt)
// 			{
// 				FProperty* Property = *PropertyIt;
//         
// 				FString PropertyName = Property->GetName();
// 				FString PropertyTypeName = Property->GetClass()->GetName();
//         
// 				UE_LOG(LogTemp, Log, TEXT("Property: %s, Type: %s"), *PropertyName, *PropertyTypeName);
//             
// 				// Get more detailed information
// 				//UE_LOG(LogTemp, Log, TEXT("  - CPP Type: %s"), *Property->GetCPPType());
// 				//UE_LOG(LogTemp, Log, TEXT("  - Size: %d bytes"), Property->GetSize());
// 				//UE_LOG(LogTemp, Log, TEXT("  - Offset: %d"), Property->GetOffset_ForInternal());
// 				//UE_LOG(LogTemp, Log, TEXT("  - Array Dim: %d"), Property->ArrayDim);
// 			}
// 		}
//
// }

void FZeroEditorModuleModule::CreatePropertyTool()
{
	TArray<UObject*> Abilities;
	GetObjectsWithOuter(UBaseGameplayAbility::StaticClass(), Abilities, true);
}

void FZeroEditorModuleModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FZeroEditorModuleModule, ZeroEditorModule)