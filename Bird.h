// Fill out your copyright notice in the Description page of Project Settings.

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

		class UCameraComponent* SideViewCameraComponent;

	class USpringArmComponent* CameraBoom;

	class UPaperFlipbookComponent* BirdFlipbook;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void MyJump();

protected:
	UFUNCTION()
		void OnBirdCollision(UPrimitiveComponent* OverlappendComp, AActor* OtherActor,
			UPrimitiveComponent* otherComp, int32 OtherBodeIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void IncrementScore(UPrimitiveComponent* OverlappendComp, AActor* OtherActor,
			UPrimitiveComponent* otherComp, int32 OtherBodeIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UPaperFlipbook* AnimationBird;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	bool CanJumpInternal_Implementation() const override;

	AGameManager* GameManager;

	bool Gravity;

	USoundWave* JumpSoundWave;

	USoundWave* DieSoundWave;

	USoundWave* PointSoundWave;

public:

	ABird();

	void SetGameManager(AGameManager* Manager);

	void GravitationON();

	void GravitationOFF();
};
