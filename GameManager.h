#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bird.h"
#include "Tube.h"
#include "Containers/Queue.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"
#include "Background.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.generated.h"

UCLASS()
class FLAPPY_BIRD_API AGameManager : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void destroyTubeFromViewport();
	friend class ABird;
	void incrementScore();
	void showDeathMenu();
	void startSpawnTube();
	void endOfQueue();

private:
	TQueue<ATube*> tubeQueue;
	FRandomStream randomStream;
	ABackground* background;
	ABird* bird;

	float distanceBetweenPipes;
	int countTube;
	int score;
	float minPipeHeight;
	float maxPipeHeight;

	UBoxComponent* collisionBoxSky;
	UBoxComponent* collisionBoxGround;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> mainMenu;
	UUserWidget* startMenuWidget;
	UTextBlock* startMenuScoreBlock;
	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> gameOver;
	UUserWidget* gameOverWidget;
	UTextBlock* gameOverScoreBlock;

public:
	AGameManager();
	UFUNCTION(BlueprintCallable)
		void restartGame();

	UBoxComponent* getCollisionBoxSky();
	UBoxComponent* getCollisionBoxGround();
	virtual void Tick(float deltaTime) override;
};