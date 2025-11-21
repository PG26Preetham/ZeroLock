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
	MenuBarBuilder.AddPullDownMenu(FText::FromString("ZeroLock_Ability"),
		FText::FromString("Custom ability menu abilities"),
		FNewMenuDelegate::CreateRaw(this,&FZeroEditorModuleModule::FillMenu));
}

void FZeroEditorModuleModule::FillMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FText::FromString("BaseAbilityList"),
		FText::FromString("Base Ability List"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			//UE_LOG(LogZeroEditor, Log, TEXT("FZeroEditorModuleModule::FillMenu"));
		}))
		);
}

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