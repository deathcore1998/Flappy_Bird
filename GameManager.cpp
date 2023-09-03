#include "GameManager.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

	FVector sizeBox3d = FVector(200.0f, 50.0f, 50.0f);
	//�������� ��� ����
	collisionBoxSky = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxSky"));
	collisionBoxSky->SetBoxExtent(sizeBox3d);
	collisionBoxSky->SetupAttachment(RootComponent);
	//�������� ��� �����
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
	//�������� ������� StartMenu � ���������� �� �����
	startMenuWidget = CreateWidget<UUserWidget>(GetWorld(), mainMenu);
	
	if (startMenuWidget) {
		startMenuScoreBlock = Cast<UTextBlock>(startMenuWidget->GetWidgetFromName(TEXT("ScoreTextBlockWidget")));
		startMenuWidget->AddToViewport();
	}
	////�������� ������� GameOver � ���������� �� �����
	gameOverWidget = CreateWidget<UUserWidget>(GetWorld(), gameOver);
	if (gameOverWidget) {	
		gameOverWidget->AddToViewport();
		gameOverScoreBlock = Cast<UTextBlock>(gameOverWidget->GetWidgetFromName(TEXT("GameOverScoreBlock")));
	}
	gameOverWidget->SetVisibility(ESlateVisibility::Collapsed);//������� ������� GameOver 
	// ����� ����� � �������� �� ������ �� GameManager
	bird = Cast<ABird>(UGameplayStatics::GetActorOfClass(GetWorld(), ABird::StaticClass()));
	bird->setGameManager(this);
	bird->gravitationOFF();
	if (bird) {
		FVector BirdPosition = bird->GetActorLocation();
		float DistanceFromTheBird = 500.f;
		//��������� ������� ����	
		background = GetWorld()->SpawnActor<ABackground>();
		background->SetActorRelativeLocation(FVector(BirdPosition.X, BirdPosition.Y - DistanceFromTheBird, BirdPosition.Z));
		//��������� ������� �������� ���� � �����
		float DistanceFromBoxSky = 700.0f;
		float DistanceFromBoxGround = 1200.0f;
		collisionBoxSky->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z + DistanceFromBoxSky));
		collisionBoxGround->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z - DistanceFromBoxGround));
		restartGame();
	}
}

void AGameManager::destroyTubeFromViewport()
{
	// �������� ����
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
		//��������� ��������� �����
		randomStream.GenerateNewSeed();

		FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		FVector locationTube = bird->GetActorLocation();
		float distanceFromTheBird = 1300;// �������� �������� ����� �� �����
		locationTube.X += distanceFromTheBird;
		// �������� ���� �� �����
		for (int i = 0; i < countTube; i++) {

			// ��������� ����� � �������� ��������� 
			locationTube.Z = randomStream.FRandRange(minPipeHeight, maxPipeHeight); 
			// �������� ���� �� ����� � ���������� �� � �������
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
	// ����������� ����� � ����� ������� ���� ��� ������ �� ������ �����
	if (firstTube != nullptr && birdLocation.X > firstTube->GetActorLocation().X + distanceFromTheBird) {
		//��������� ��������� �����
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
	//���������� ����� � 0
	score = 0;
	startMenuScoreBlock->SetText(FText::AsNumber(score));
	//��������� ����� � ��������� ���������
	bird->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AGameManager::incrementScore()
{
	startMenuScoreBlock->SetText(FText::AsNumber(++score));
}

void AGameManager::showDeathMenu()
{
	//����� ���������� ��������� �����
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