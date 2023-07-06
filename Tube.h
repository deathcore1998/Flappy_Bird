// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Tube.generated.h"

UCLASS()
class FLAPPY_BIRD_API ATube : public AActor
{
	GENERATED_BODY()

public:

	ATube();

protected:

	virtual void BeginPlay() override;

	float SpeedTube;

	UBoxComponent* CollisionBoxUp;

	UBoxComponent* CollisionBoxDown;

	UBoxComponent* ScoreCollision;

	class UPaperFlipbookComponent* TubeUp;

	class UPaperFlipbookComponent* TubeDown;

	class UPaperFlipbook* TubeFlipbook;

public:

	UBoxComponent* GetScoreCollision();

	UBoxComponent* GetCollisionBoxUp();

	UBoxComponent* GetCollisionBoxDown();

	virtual void Tick(float DeltaTime) override;
};
