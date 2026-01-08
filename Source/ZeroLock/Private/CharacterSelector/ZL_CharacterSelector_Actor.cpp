//Copyright Preetham Mukundan (C) 2026


#include "CharacterSelector/ZL_CharacterSelector_Actor.h"

#include "CharacterSelector/ZL_CharacterSelectionSubsystem.h"
#include"CharacterSelector/ZL_CharacterSelectionVM.h"
#include "CharacterSelector/ZL_Character_Data_Asset.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZeroLock/ZeroLock.h"
#include "ZeroLock/ZeroLockCharacter.h"

// Sets default values
AZL_CharacterSelector_Actor::AZL_CharacterSelector_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
	RootComponent = PreviewMesh;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	// Create capture component
	CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComp"));
	CaptureComp->SetupAttachment(SpringArm);



}

void AZL_CharacterSelector_Actor::OnMousePosChanged(FVector2D NewMousePos)
{
	float MaxYaw = 20.0f;
	float MaxPitch = 15.0f;
	
	float AlphaX = (NewMousePos.X ) ;
	AlphaX = FMath::Clamp(AlphaX, -1, 1);
	float AlphaY = (NewMousePos.Y - 0.5f) * 2.0f;
	float finalX = -90 +(AlphaX*MaxYaw);
	FString logd= ""+FString::SanitizeFloat(AlphaX*MaxYaw);
	//ZLOG(logd);
	//GEngine->AddOnScreenDebugMessage(11, 2.f ? 2.f : -1.f, FColor::Yellow, logd);
	TargetSpringArmRot = FRotator(0, finalX , 0.0f);
}

// Called when the game starts or when spawned
void AZL_CharacterSelector_Actor::BeginPlay()
{
	Super::BeginPlay();
	CaptureComp->ShowOnlyComponent(PreviewMesh);


	if (UGameInstance* GI = GetGameInstance())
	{
		if (auto* Subsystem = GI->GetSubsystem<UZL_CharacterSelectionSubsystem>())
		{
			BoundVM = Subsystem->SelectionVM;

			if (BoundVM)
			{
				BoundVM->OnCharacterChanged.AddDynamic(this,&AZL_CharacterSelector_Actor::OnViewModelFieldChanged);
				OnViewModelFieldChanged(BoundVM->CurrentCharacter);

				BoundVM->OnMousePosChanged.AddDynamic(this,&AZL_CharacterSelector_Actor::OnMousePosChanged);
			}
		}
	}
}

// Called every frame
void AZL_CharacterSelector_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (SpringArm)
	{
		FRotator CurrentRot = SpringArm->GetRelativeRotation();
		
		//FRotator SmoothedRot = FMath::RInterpTo(CurrentRot, TargetSpringArmRot, DeltaTime, 8.0f);
		FRotator SmoothedRot = UKismetMathLibrary::REase(CurrentRot,TargetSpringArmRot,0.1,true,EEasingFunc::CircularIn);
        
		SpringArm->SetRelativeRotation(SmoothedRot);
		//GEngine->AddOnScreenDebugMessage(10, 2.f ? 2.f : -1.f, FColor::Yellow, SmoothedRot.ToString());
	}
}

void AZL_CharacterSelector_Actor::OnViewModelFieldChanged(UZL_Character_Data_Asset* NewHero)
{
	if (NewHero)
	{
		SetPreview(NewHero->DisplaySeletalMesh,NewHero->DisplayAnimation);
		if (!BoundVM) return;
		BoundVM->SetCharacterName(FText::FromString(NewHero->CharacterName));
			// 2. Access the Class Default Object (CDO)
		BoundVM->SetAbility1(nullptr);
		BoundVM->SetAbility2(nullptr);
		BoundVM->SetAbility3(nullptr);
		BoundVM->SetAbility4(nullptr);
		BoundVM->SetColorSelected(NewHero->IconColor);
		if (AZeroLockCharacter* DefaultChar =  NewHero->CharacterClass->GetDefaultObject<AZeroLockCharacter>())
		{
			if (DefaultChar->SecondryFireAbility)
			{
				if (UBaseGameplayAbility* secon = DefaultChar->SecondryFireAbility->GetDefaultObject<UBaseGameplayAbility>())
				{
				
					BoundVM->SetAbility1(secon->IconImage);

				}
			}
			if (DefaultChar->Ability_1)
			{
				if (UBaseGameplayAbility* secon = DefaultChar->Ability_1->GetDefaultObject<UBaseGameplayAbility>())
				{
			
					BoundVM->SetAbility2(secon->IconImage);
			
				}
			}
			if (DefaultChar->Ability_2)
			{
				if (UBaseGameplayAbility* secon = DefaultChar->Ability_2->GetDefaultObject<UBaseGameplayAbility>())
				{
				
					BoundVM->SetAbility3(secon->IconImage);
			
				}
			}
			if (DefaultChar->UltimateAbility)
			{
				if (UBaseGameplayAbility* secon = DefaultChar->UltimateAbility->GetDefaultObject<UBaseGameplayAbility>())
				{
				
					BoundVM->SetAbility4(secon->IconImage);
				
				}
			}
			
			
		}
	}
}



void AZL_CharacterSelector_Actor::SetPreview(USkeletalMesh* NewMesh, class UAnimSequence* NewAnim)
{
	if (NewMesh)
	{
		PreviewMesh->SetSkeletalMesh(NewMesh);
	}

	if (NewAnim)
	{
		PreviewMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		PreviewMesh->SetAnimation(NewAnim);
		PreviewMesh->Play(false); // loop
	}
}

