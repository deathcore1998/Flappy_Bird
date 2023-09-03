#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Tube.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundClass.h"
#include "Sound/SoundWave.h"

#include "Bird.generated.h"

class AGameManager;

UCLASS()
class FLAPPY_BIRD_API ABird : public APaperCharacter
{
	GENERATED_BODY()

private:
	class UCameraComponent* sideViewCameraComponent;
	class USpringArmComponent* cameraBoom;
	class UPaperFlipbookComponent* birdFlipbook;
	class UPaperFlipbook* animationBird;

protected:
	UFUNCTION()
		void onBirdCollision(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
		void incrementScore(UPrimitiveComponent* overlappendComp, AActor* OtherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);

	virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;
	bool CanJumpInternal_Implementation() const override;
	void myJump();
	virtual void BeginPlay() override;

private:
	AGameManager* gameManager;
	USoundWave* jumpSoundWave;
	USoundWave* dieSoundWave;
	USoundWave* pointSoundWave;

	bool gravity;
public:
	ABird();
	virtual void Tick(float deltaTime) override;
	void setGameManager(AGameManager* manager);
	void gravitationON();
	void gravitationOFF();
};
