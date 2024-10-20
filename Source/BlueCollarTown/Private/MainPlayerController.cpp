// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Selectable.h"
#include "Net/UnrealNetwork.h"

AMainPlayerController::AMainPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

void AMainPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMainPlayerController, Selected, COND_OwnerOnly);
}

void AMainPlayerController::Handle_Selection(AActor* ActorToSelect)
{
    if (ActorToSelect)
    {
        if (!Selected.Contains(ActorToSelect))
        {
            Selected.Add(ActorToSelect);
            OnRep_Selected();
        }
    }
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

bool AMainPlayerController::ActorSelected(AActor* ActorToCheck) const
{
	if(ActorToCheck && Selected.Contains(ActorToCheck))
	{
		return true;
	}

	return false;
}

void AMainPlayerController::Server_Select_Implementation(AActor* ActorToSelect)
{
	Server_ClearSelected();

	if(ActorToSelect)
	{
		if(ISelectable* Selectable = Cast<ISelectable>(ActorToSelect))
		{
			Selectable->Select();
			Selected.Add(ActorToSelect);
			OnRep_Selected();
		}
	}
}

void AMainPlayerController::Server_Deselect_Implementation(AActor* ActorToDeselect)
{
	if(ActorToDeselect)
	{
		if(ISelectable* Selectable = Cast<ISelectable>(ActorToDeselect))
		{
			Selectable->Deselect();
			Selected.Remove(ActorToDeselect);
			OnRep_Selected();
		}
	}
}

void AMainPlayerController::Server_ClearSelected_Implementation()
{
	for (int i = 0; i < Selected.Num(); ++i)
	{
		if(Selected[i])
		{
			if(ISelectable* Selectable = Cast<ISelectable>(Selected[i]))
			{
				Selectable->Deselect();
			}
		}
	}

	Selected.Empty();
	OnRep_Selected();
}

void AMainPlayerController::OnRep_Selected()
{
	OnSelectedUpdated.Broadcast();
}
