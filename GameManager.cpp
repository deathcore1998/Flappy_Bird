#include "GameManager.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

	FVector sizeBox3d = FVector(200.0f, 50.0f, 50.0f);
	//Коллизия для неба
	collisionBoxSky = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxSky"));
	collisionBoxSky->SetBoxExtent(sizeBox3d);
	collisionBoxSky->SetupAttachment(RootComponent);
	//Коллизия для земли
	collisionBoxGround = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxGround"));
	collisionBoxGround->SetBoxExtent(sizeBox3d);
	collisionBoxGround->SetupAttachment(collisionBoxSky);

	distanceBetweenPipes = 600.0f;
	score = 0;
	countTube = 5;
	minPipeHeight = 250.f;
	maxPipeHeight = 800.f;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	//Создание виджета StartMenu и размещение на карте
	startMenuWidget = CreateWidget<UUserWidget>(GetWorld(), mainMenu);
	
	if (startMenuWidget) {
		startMenuScoreBlock = Cast<UTextBlock>(startMenuWidget->GetWidgetFromName(TEXT("ScoreTextBlockWidget")));
		startMenuWidget->AddToViewport();
	}
	////Создание виджета GameOver и размещение на карте
	gameOverWidget = CreateWidget<UUserWidget>(GetWorld(), gameOver);
	if (gameOverWidget) {	
		gameOverWidget->AddToViewport();
		gameOverScoreBlock = Cast<UTextBlock>(gameOverWidget->GetWidgetFromName(TEXT("GameOverScoreBlock")));
	}
	gameOverWidget->SetVisibility(ESlateVisibility::Collapsed);//Скрытие виджета GameOver 
	// Поиск птицы и передача ей ссылки на GameManager
	bird = Cast<ABird>(UGameplayStatics::GetActorOfClass(GetWorld(), ABird::StaticClass()));
	bird->setGameManager(this);
	bird->gravitationOFF();
	if (bird) {
		FVector BirdPosition = bird->GetActorLocation();
		float DistanceFromTheBird = 500.f;
		//Установка заднего фона	
		background = GetWorld()->SpawnActor<ABackground>();
		background->SetActorRelativeLocation(FVector(BirdPosition.X, BirdPosition.Y - DistanceFromTheBird, BirdPosition.Z));
		//Установка позиций коллизий неба и земли
		float DistanceFromBoxSky = 700.0f;
		float DistanceFromBoxGround = 1200.0f;
		collisionBoxSky->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z + DistanceFromBoxSky));
		collisionBoxGround->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z - DistanceFromBoxGround));
		restartGame();
	}
}

void AGameManager::destroyTubeFromViewport()
{
	// Удаление труб
	if (!tubeQueue.IsEmpty()){
		ATube* pipe;
		for (int i = 0; i < countTube; i++) {
			tubeQueue.Dequeue(pipe);
			pipe->Destroy();
		}
	}
}

void AGameManager::startSpawnTube()
{
	UWorld* world = GetWorld();
	if (world) {
		//Случайное начальное число
		randomStream.GenerateNewSeed();

		FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		FVector locationTube = bird->GetActorLocation();
		float distanceFromTheBird = 1300;// Дистация создания трубы от птицы
		locationTube.X += distanceFromTheBird;
		// Создание труб на карте
		for (int i = 0; i < countTube; i++) {

			// случайное число в заданном диапазоне 
			locationTube.Z = randomStream.FRandRange(minPipeHeight, maxPipeHeight); 
			// Создание труб на карте и добавление их в очередь
			ATube* pipe = world->SpawnActor<ATube>(ATube::StaticClass(), locationTube, spawnRotation, spawnParams);
			tubeQueue.Enqueue(pipe);
			locationTube.X += distanceBetweenPipes;
		}
	}
}

void AGameManager::endOfQueue()
{
	FVector birdLocation = bird->GetActorLocation();
	ATube* firstTube = nullptr;
	tubeQueue.Peek(firstTube);
	float distanceFromTheBird = 1000;
	// перемещение трубы в конец очереди если она далеко за спиной птицы
	if (firstTube != nullptr && birdLocation.X > firstTube->GetActorLocation().X + distanceFromTheBird) {
		//Случайное начальное число
		randomStream.GenerateNewSeed();

		FVector tempLocation = firstTube->GetActorLocation();
		tempLocation.X += distanceBetweenPipes * countTube;
		tempLocation.Z = randomStream.FRandRange(minPipeHeight, maxPipeHeight);
		tubeQueue.Dequeue(firstTube);
		tubeQueue.Enqueue(firstTube);
		firstTube->SetActorLocation(tempLocation);
	}
}

void AGameManager::restartGame()
{
	destroyTubeFromViewport();
	gameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
	startMenuWidget->SetVisibility(ESlateVisibility::Visible);
	//Обновление счета в 0
	score = 0;
	startMenuScoreBlock->SetText(FText::AsNumber(score));
	//Установка птицы в начальное положение
	bird->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AGameManager::incrementScore()
{
	startMenuScoreBlock->SetText(FText::AsNumber(++score));
}

void AGameManager::showDeathMenu()
{
	//Показ количества набранных очков
	FString scoreText = FString::Printf(TEXT("Score: %d"), score);
	gameOverScoreBlock->SetText(FText::FromString(scoreText));

	startMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	gameOverWidget->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(), true); 
	bird->gravitationOFF();
}

UBoxComponent* AGameManager::getCollisionBoxSky()
{
	return collisionBoxSky;
}

UBoxComponent* AGameManager::getCollisionBoxGround()
{
	return collisionBoxGround;
}

void AGameManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	endOfQueue();
}