//Copyright Preetham Mukundan (C) 2026


#include "Weapon/Zero_BaseProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"


AZero_BaseProjectile::AZero_BaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionComp=CreateDefaultSubobject<USphereComponent>("Collision");
	SetRootComponent(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");


	CollisionComp->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::CollisionOverLap);
	CollisionComp->OnComponentHit.AddDynamic(this,&ThisClass::CollisionHit);
}

// Called when the game starts or when spawned
void AZero_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::CollisionOverLap);
	CollisionComp->OnComponentHit.AddDynamic(this,&ThisClass::CollisionHit);
}

// Called every frame
void AZero_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZero_BaseProjectile::CollisionOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapEventCallBack(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex,bFromSweep,SweepResult);
}

void AZero_BaseProjectile::HitEventCallBack(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OwnerCharacter)
	{
		if (AZeroLockCharacter* Char = Cast<AZeroLockCharacter>(GetOwner()))
		{
			OwnerCharacter = Char;
		}
		else
		{
			Destroy();
			return;
		}
	}

	if (AZeroLockCharacter* enemy = Cast<AZeroLockCharacter>(OtherActor))
	{
		OwnerCharacter->GetMyAbilitySystemComp()->ApplyWeaponDamage(enemy->GetMyAbilitySystemComp(),1);
	}
	Destroy();
}

void AZero_BaseProjectile::OverlapEventCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AZero_BaseProjectile::CollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitEventCallBack(HitComponent,OtherActor,OtherComp,NormalImpulse,Hit);
}

