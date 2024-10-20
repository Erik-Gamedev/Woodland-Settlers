// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerCharacter.h"

// Sets default values
AWorkerCharacter::AWorkerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorkerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorkerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWorkerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWorkerCharacter::Select()
{
	Selected = true;
	HighLight(Selected);
}

void AWorkerCharacter::Deselect()
{
	Selected = false;
	HighLight(Selected);
}

void AWorkerCharacter::HighLight(const bool Highlight)
{
	TArray<UPrimitiveComponent*> Components;
	GetComponents<UPrimitiveComponent>(Components);
	for (UPrimitiveComponent* VisualComp : Components)
	{
		if(UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComp))
		{
			Prim->SetRenderCustomDepth(Highlight);
		}
	}
}

