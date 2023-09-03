#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Background.generated.h"

UCLASS()
class FLAPPY_BIRD_API ABackground : public AActor
{
	GENERATED_BODY()

public:
	ABackground();

protected:
	virtual void BeginPlay() override;
	class UPaperFlipbookComponent* backGround;
	class UPaperFlipbook* backGroundFlipbook;
};