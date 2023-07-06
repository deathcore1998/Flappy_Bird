#include "GameManager.h"


AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

	FVector SizeBox3d = FVector(200.0f, 50.0f, 50.0f);

	//Коллизия для неба
	CollisionBoxSky = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxSky"));
	CollisionBoxSky->SetBoxExtent(SizeBox3d);
	CollisionBoxSky->SetupAttachment(RootComponent);

	//Коллизия для земли
	CollisionBoxGround = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxGround"));
	CollisionBoxGround->SetBoxExtent(SizeBox3d);
	CollisionBoxGround->SetupAttachment(CollisionBoxSky);

	DistanceBetweenPipes = 600.0f;

	score = 0;
}


void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	//Создание виджета StartMenu и размещение на карте
	StartMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenu);
	StartMenuScoreBlock = Cast<UTextBlock>(StartMenuWidget->GetWidgetFromName(TEXT("ScoreTextBlockWidget")));
	if (StartMenuWidget)
	{
		StartMenuWidget->AddToViewport();
	}

	////Создание виджета GameOver и размещение на карте
	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOver);
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport();
	}
	GameOverScoreBlock = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("GameOverScoreBlock")));
	GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);//Скрытие виджета GameOver 

	// Поиск птицы и передача ей ссылки на GameManager
	Bird = Cast<ABird>(UGameplayStatics::GetActorOfClass(GetWorld(), ABird::StaticClass()));
	Bird->SetGameManager(this);
	Bird->GravitationOFF();

	if (Bird)
	{
		FVector BirdPosition = Bird->GetActorLocation();
		float DistanceFromTheBird = 500.f;
		//Установка заднего фона	
		Background = GetWorld()->SpawnActor<ABackground>();
		Background->SetActorRelativeLocation(FVector(BirdPosition.X, BirdPosition.Y - DistanceFromTheBird, BirdPosition.Z));
		//Установка позиций коллизий неба и земли
		float DistanceFromBoxSky = 700.0f;
		float DistanceFromBoxGround = 1200.0f;
		CollisionBoxSky->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z + DistanceFromBoxSky));
		CollisionBoxGround->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z - DistanceFromBoxGround));

		RestartGame();
	}
}

void AGameManager::DestroyTubeFromViewport()
{
	if (!TubeQueue.IsEmpty())// очистка труб
	{
		ATube* Pipe;
		for (int i = 0; i < countTube; i++)
		{
			TubeQueue.Dequeue(Pipe);
			Pipe->Destroy();
		}
	}
}

void AGameManager::StartSpawnTube()
{
	UWorld* World = GetWorld();

	if (World)
	{
		//Случайное начальное число
		RandomStream.GenerateNewSeed();

		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector LocationTube = Bird->GetActorLocation();
		float DistanceFromTheBird = 1300;// Дистация создания трубы от птицы
		LocationTube.X += DistanceFromTheBird;

		for (int i = 0; i < countTube; i++)// Создание труб на карте
		{
			float MinPipeHeight = 250.f;
			float MaxPipeHeight = 800.f;

			// случайное число в заданном диапазоне 
			LocationTube.Z = RandomStream.FRandRange(MinPipeHeight, MaxPipeHeight); 
			// Создание труб на карте и добавление их в очередь
			ATube* Pipe = World->SpawnActor<ATube>(ATube::StaticClass(), LocationTube, SpawnRotation, SpawnParams);
			TubeQueue.Enqueue(Pipe);
			LocationTube.X += DistanceBetweenPipes;
		}
	}
}

void AGameManager::EndOfQueue()
{
	FVector BirdLocation = Bird->GetActorLocation();

	ATube* FirstTube = nullptr;
	TubeQueue.Peek(FirstTube);
	float DistanceFromTheBird = 1000;
	// перемещение трубы в конец очереди если она далеко за спиной птицы
	if (FirstTube != nullptr && BirdLocation.X > FirstTube->GetActorLocation().X + DistanceFromTheBird)
	{
		//Случайное начальное число
		RandomStream.GenerateNewSeed();
		float MinPipeHeight = 250.f;
		float MaxPipeHeight = 800.f;

		FVector TempLocation = FirstTube->GetActorLocation();
		TempLocation.X += DistanceBetweenPipes * countTube;
		TempLocation.Z = RandomStream.FRandRange(MinPipeHeight, MaxPipeHeight);
		TubeQueue.Dequeue(FirstTube);
		TubeQueue.Enqueue(FirstTube);

		FirstTube->SetActorLocation(TempLocation);
	}
}

void AGameManager::RestartGame()
{
	DestroyTubeFromViewport();
	GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
	StartMenuWidget->SetVisibility(ESlateVisibility::Visible);
	//Обновление счета в 0
	score = 0;
	StartMenuScoreBlock->SetText(FText::AsNumber(score));
	//Установка птицы в начальное положение
	Bird->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AGameManager::IncrementScore()
{
	StartMenuScoreBlock->SetText(FText::AsNumber(++score));
}

void AGameManager::ShowDeathMenu()
{
	//Показ количества набранных очков
	FString ScoreText = FString::Printf(TEXT("Score: %d"), score);
	GameOverScoreBlock->SetText(FText::FromString(ScoreText));

	StartMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	GameOverWidget->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(), true); 
	Bird->GravitationOFF();
}

UBoxComponent* AGameManager::GetCollisionBoxSky()
{
	return CollisionBoxSky;
}

UBoxComponent* AGameManager::GetCollisionBoxGround()
{
	return CollisionBoxGround;
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EndOfQueue();
}