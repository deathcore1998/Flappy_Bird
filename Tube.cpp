#include "Tube.h"

ATube::ATube()
{
    PrimaryActorTick.bCanEverTick = true;

    tubeFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/pipe_Flipbook.pipe_Flipbook"));
    //Создание верхней трубы
    tubeUp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("tubeUp"));
    RootComponent = tubeUp;
    tubeUp->SetFlipbook(tubeFlipbook);
    tubeUp->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    FVector sizeCollisionTube = FVector(50.0f, 50.0f, 500.0f);
    //Настройка коллизии верхней трубы
    collisionBoxUp = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBoxUp"));
    collisionBoxUp->SetupAttachment(RootComponent);
    collisionBoxUp->SetBoxExtent(sizeCollisionTube);
    //Создание нижней трубы
    tubeDown = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("tubeDown"));
    tubeDown->SetFlipbook(tubeFlipbook);
    tubeDown->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    tubeDown->SetupAttachment(RootComponent);
    //Настройка коллизии нижней трубы
    collisionBoxDown = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBoxDown"));
    collisionBoxDown->SetupAttachment(tubeDown);
    collisionBoxDown->SetBoxExtent(sizeCollisionTube);

    float offset = 1400.0f; // Расстояние между трубами
    FVector relativeLocation = FVector(0.0f, 0.0f, offset);
    tubeDown->AddRelativeLocation(relativeLocation);// Расположение труб на дистанции

    //Настройка коллизии для добавления очков
    scoreCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreCollision"));
    scoreCollision->SetupAttachment(RootComponent);
    FVector SizeCollisionScore = FVector(10.0f, 10.0f, 170.0f);
    scoreCollision->SetBoxExtent(SizeCollisionScore);
    //Расположение коллизии очков между трубами с небольшим смещением
    relativeLocation = FVector(-45.0f, 0.0f, offset / 2);
    scoreCollision->AddRelativeLocation(relativeLocation);

    speedTube = 0.0f;
}

void ATube::BeginPlay()
{
    Super::BeginPlay();
    speedTube = -300.0f;
}

UBoxComponent* ATube::getScoreCollision()
{
    return scoreCollision;
}

UBoxComponent* ATube::getCollisionBoxUp()
{
    return collisionBoxUp;
}

UBoxComponent* ATube::getCollisionBoxDown()
{
    return collisionBoxDown;
}

void ATube::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    //Сдвиг трубы к птице
    FVector newTubeLocation = GetActorLocation();
    newTubeLocation.X += deltaTime * speedTube;
    SetActorLocation(newTubeLocation);
}
