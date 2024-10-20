// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectedUpdatedDelegate);

/**
 * 
 */
UCLASS()
class BLUECOLLARTOWN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

	UFUNCTION()
	void Handle_Selection(AActor* ActorToSelect);
	
	UPROPERTY(ReplicatedUsing = OnRep_Selected)
	TArray<AActor*> Selected;
protected:
	virtual  void BeginPlay() override;

	UFUNCTION()
	bool ActorSelected(AActor* ActorToCheck) const;

	UFUNCTION(Server, Reliable)
	void Server_Select(AActor* ActorToSelect);

	UFUNCTION(Server, Reliable)
	void Server_Deselect(AActor* ActorToDeselect);

	UFUNCTION(Server, Reliable)
	void Server_ClearSelected();

	UFUNCTION()
	void OnRep_Selected();

	

	UPROPERTY()
	FSelectedUpdatedDelegate OnSelectedUpdated;
};
 