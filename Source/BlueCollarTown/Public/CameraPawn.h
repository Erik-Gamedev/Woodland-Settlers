
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "CameraPawn.generated.h"

class UInputAction;
class UInputMappingContext;


UCLASS()
class BLUECOLLARTOWN_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:

	ACameraPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	void MoveCameraIn2D(const FInputActionValue& Value);

	//EnhancedInput

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveCameraAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* CameraContext;
public:	
	
};
