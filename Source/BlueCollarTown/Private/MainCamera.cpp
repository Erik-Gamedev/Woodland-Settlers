// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

#include "AssetTypeCategories.h"
#include "InputState.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "MainPlayerController.h"
#include "Components/Widget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCamera::AMainCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(SceneComponent);
	SpringArmComponent->TargetArmLength = 2000.0f;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMainCamera::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation();
	TargetZoom = 3000.f;

	const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + - 50, Rotation.Yaw, 0.f);

	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AMainCamera::GetTerrainPosition(FVector& TerrainPosition) const
{
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	FVector TraceOrigin = TerrainPosition;
	TraceOrigin.Z += 10000.f;
	FVector TraceEnd = TerrainPosition;
	TraceEnd.Z -= 10000.f;

	if(UWorld* WorldContext = GetWorld())
	{
		if(WorldContext->LineTraceSingleByChannel(Hit, TraceOrigin, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams))
		{
			TerrainPosition = Hit.ImpactPoint;
		}
	}
}

void AMainCamera::Forward(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}

	TargetLocation = SpringArmComponent->GetForwardVector() * AxisValue * MoveSpeed + TargetLocation;
	GetTerrainPosition(TargetLocation);
}

void AMainCamera::Right(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	TargetLocation = SpringArmComponent->GetRightVector() * AxisValue * MoveSpeed + TargetLocation;
	GetTerrainPosition(TargetLocation);
}

void AMainCamera::Zoom(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	const float Zoom = AxisValue * 100.f;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, MinZoom, MaxZoom);
}
void AMainCamera::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.f, -45, 0.f));
}

void AMainCamera::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.f,45, 0.f));
}

void AMainCamera::EnableRotate()
{
	CanRotate = true;
}

void AMainCamera::DisableRotate()
{
	CanRotate = false;
}

void AMainCamera::RotateHorizontal(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}

	if(CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.f, AxisValue  , 0.f));
	}
}

void AMainCamera::RotateVertical(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	if(CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue, 0.f, 0.f));
	}
}

void AMainCamera::EdgeScroll()
{
	if(UWorld* WorldContext = GetWorld())
	{
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(WorldContext);
		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(WorldContext);
		MousePosition = MousePosition * UWidgetLayoutLibrary::GetViewportScale(WorldContext);
		MousePosition.X = MousePosition.X / ViewportSize.X;
		MousePosition.Y = MousePosition.Y / ViewportSize.Y;

		//Right/Left
		if(MousePosition.X > 0.95f && MousePosition.X < 1.f)
		{
			Right(1);
		}
		else if(MousePosition.X < 0.05f && MousePosition.X > 0.f)
		{
			Right(-1);
		}

		//Forward/Backward
		if(MousePosition.Y > 0.95f && MousePosition.Y < 1.f)
		{
			Forward(-1);
		}
		else if(MousePosition.Y < 0.05f && MousePosition.Y > 0.f)
		{
			Forward(1);
		}
	}	
}

AActor* AMainCamera::GetSelectedObject()
{
	if(UWorld* World = GetWorld())
	{
		FVector WorldLocation, WorldDirection;
		MainPlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		FVector End = WorldDirection * 1000000.f + WorldLocation;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FHitResult Hit;
		if(World->LineTraceSingleByChannel(Hit, WorldLocation, End, ECC_Visibility, Params))
		{
			if(AActor* HitActor = Hit.GetActor())
			{
				return HitActor;
			}
		}
	}

	return nullptr;
}

void AMainCamera::MouseLeftPressed()
{
	if(MainPlayerController)
	{
		MainPlayerController->Handle_Selection(GetSelectedObject());
	}
	
	AActor* SelectedActor = GetSelectedObject();
	MainPlayerController->Handle_Selection(SelectedActor);
	UE_LOG(LogTemp, Warning, TEXT("Selected Actor: %s"), *SelectedActor->GetName());
}

void AMainCamera::MouseLeftReleased()
{
	if(MainPlayerController)
	{
		MainPlayerController->Handle_Selection(GetSelectedObject());
	}

	
}

void AMainCamera::MouseRightPressed()
{
	if (MainPlayerController)
	{
		FString SelectedActorsString;
		for (AActor* Actor : MainPlayerController->Selected)
		{
			if (Actor)
			{
				SelectedActorsString += Actor->GetName() + TEXT(", ");
			}
		}

		// Remove the trailing comma and space
		if (!SelectedActorsString.IsEmpty())
		{
			SelectedActorsString.LeftChopInline(2);
		}

		UE_LOG(LogTemp, Warning, TEXT("Selected Actors: %s"), *SelectedActorsString);
	}
}

void AMainCamera::MouseRightReleased()
{
}

void AMainCamera::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if(TargetRotation.Pitch < (RotatePitchMax * -1))
	{
		NewPitch = (RotatePitchMax * -1);
	}
	else if (TargetRotation.Pitch > (RotatePitchMin * -1))
	{
		NewPitch = (RotatePitchMin * -1);
	}

	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.f);
}
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CameraBounds();
	EdgeScroll();

	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);
	
	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength ,TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent->TargetArmLength = InterpolatedZoom;

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	SpringArmComponent->SetRelativeRotation(InterpolatedRotation);
}

void AMainCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AMainCamera::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AMainCamera::Right);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AMainCamera::Zoom);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &AMainCamera::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &AMainCamera::RotateVertical);

	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed, this, &AMainCamera	::RotateRight);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed, this, &AMainCamera::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Pressed, this, &AMainCamera::EnableRotate);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Released, this, &AMainCamera::DisableRotate);
	
	PlayerInputComponent->BindAction(TEXT("MouseLeftclick"), IE_Pressed, this, &AMainCamera::MouseLeftPressed);
	PlayerInputComponent->BindAction(TEXT("MouseLeftclick"), IE_Released, this, &AMainCamera::MouseLeftReleased);

	PlayerInputComponent->BindAction(TEXT("MouseRightclick"), IE_Pressed, this, &AMainCamera::MouseRightPressed);
	PlayerInputComponent->BindAction(TEXT("MouseRightclick"), IE_Released, this, &AMainCamera::MouseRightReleased);
}


