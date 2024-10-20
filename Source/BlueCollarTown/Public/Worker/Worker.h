
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Worker.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;

UCLASS()
class BLUECOLLARTOWN_API AWorker : public ACharacter
{
	GENERATED_BODY()

public:
	AWorker();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* WorkerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;


private:
	
};
