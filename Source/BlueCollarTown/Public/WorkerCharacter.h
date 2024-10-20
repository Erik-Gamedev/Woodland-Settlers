// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Selectable.h"
#include "GameFramework/Character.h"
#include "WorkerCharacter.generated.h"

UCLASS()
class BLUECOLLARTOWN_API AWorkerCharacter : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWorkerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	//Selected Interface

	UFUNCTION()
	virtual void Select() override;
	
	UFUNCTION()
	virtual void Deselect() override;
	
	UFUNCTION()
	virtual void HighLight(const bool Highlight) override;

	UPROPERTY()
	bool Selected;
};
