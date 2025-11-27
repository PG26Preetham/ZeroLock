// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Objectives/ZeroGuardian.h"

#include "Components/SphereComponent.h"

AZeroGuardian::AZeroGuardian()
{
	BeamSphere = CreateDefaultSubobject<USphereComponent>("Beam Overlap Sphere");
	BeamSphere->SetupAttachment(ObjectiveMesh);
	BeamSphere->SetSphereRadius(BeamRange);
	BeamSphere->SetLineThickness(5.0f);
	BeamSphere->ShapeColor = FColor::Red;
	
	PunchSphere = CreateDefaultSubobject<USphereComponent>("Punch Overlap Sphere");
	PunchSphere->SetupAttachment(ObjectiveMesh);
	PunchSphere->SetSphereRadius(PunchRange);
	PunchSphere->SetLineThickness(5.0f);
	PunchSphere->ShapeColor = FColor::Green;
}

void AZeroGuardian::OnConstruction(const FTransform& Transform)
{

	if (IsValid(BeamSphere))
	{
		BeamSphere->SetSphereRadius(BeamRange);
	}

	if (IsValid(PunchSphere))
	{
		PunchSphere->SetSphereRadius(PunchRange);
	}
	
	Super::OnConstruction(Transform);
}
