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

protected:
	virtual void BeginPlay() override;

	float speedTube;
	UBoxComponent* collisionBoxUp;
	UBoxComponent* collisionBoxDown;
	UBoxComponent* scoreCollision;
	class UPaperFlipbookComponent* tubeUp;
	class UPaperFlipbookComponent* tubeDown;
	class UPaperFlipbook* tubeFlipbook;

public:
	UBoxComponent* getScoreCollision();
	UBoxComponent* getCollisionBoxUp();
	UBoxComponent* getCollisionBoxDown();
	ATube();
	virtual void Tick(float deltaTime) override;
};
