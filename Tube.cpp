#include "Tube.h"

ATube::ATube()
{
    PrimaryActorTick.bCanEverTick = true;

    tubeFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/pipe_Flipbook.pipe_Flipbook"));
    //�������� ������� �����
    tubeUp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("tubeUp"));
    RootComponent = tubeUp;
    tubeUp->SetFlipbook(tubeFlipbook);
    tubeUp->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    FVector sizeCollisionTube = FVector(50.0f, 50.0f, 500.0f);
    //��������� �������� ������� �����
    collisionBoxUp = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBoxUp"));
    collisionBoxUp->SetupAttachment(RootComponent);
    collisionBoxUp->SetBoxExtent(sizeCollisionTube);
    //�������� ������ �����
    tubeDown = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("tubeDown"));
    tubeDown->SetFlipbook(tubeFlipbook);
    tubeDown->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    tubeDown->SetupAttachment(RootComponent);
    //��������� �������� ������ �����
    collisionBoxDown = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionBoxDown"));
    collisionBoxDown->SetupAttachment(tubeDown);
    collisionBoxDown->SetBoxExtent(sizeCollisionTube);

    float offset = 1400.0f; // ���������� ����� �������
    FVector relativeLocation = FVector(0.0f, 0.0f, offset);
    tubeDown->AddRelativeLocation(relativeLocation);// ������������ ���� �� ���������

    //��������� �������� ��� ���������� �����
    scoreCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreCollision"));
    scoreCollision->SetupAttachment(RootComponent);
    FVector SizeCollisionScore = FVector(10.0f, 10.0f, 170.0f);
    scoreCollision->SetBoxExtent(SizeCollisionScore);
    //������������ �������� ����� ����� ������� � ��������� ���������
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
    //����� ����� � �����
    FVector newTubeLocation = GetActorLocation();
    newTubeLocation.X += deltaTime * speedTube;
    SetActorLocation(newTubeLocation);
}
