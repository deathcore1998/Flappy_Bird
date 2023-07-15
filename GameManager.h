
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

public:
	AGameManager();

protected:

	virtual void BeginPlay() override;

private:

	void EndOfQueue();

	TQueue<ATube*> TubeQueue;

	float DistanceBetweenPipes;

	int countTube = 5;

	FRandomStream RandomStream;

	ABackground* Background;

	ABird* Bird;

	UBoxComponent* CollisionBoxSky;

	UBoxComponent* CollisionBoxGround;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> MainMenu;

	UUserWidget* StartMenuWidget;

	UTextBlock* StartMenuScoreBlock;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> GameOver;

	UUserWidget* GameOverWidget;

	UTextBlock* GameOverScoreBlock;

	int score;

	void DestroyTubeFromViewport();

	friend class ABird;
	void IncrementScore();

	void ShowDeathMenu();

	void StartSpawnTube();

public:

	UFUNCTION(BlueprintCallable)
		void RestartGame();

	UBoxComponent* GetCollisionBoxSky();

	UBoxComponent* GetCollisionBoxGround();

	virtual void Tick(float DeltaTime) override;
};
