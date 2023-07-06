
#include "Tube.h"

ATube::ATube()
{
    PrimaryActorTick.bCanEverTick = true;

    TubeFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/pipe_Flipbook.pipe_Flipbook"));
    //�������� ������� �����
    TubeUp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("TubeUp"));
    RootComponent = TubeUp;
    TubeUp->SetFlipbook(TubeFlipbook);
    TubeUp->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    FVector SizeCollisionTube = FVector(50.0f, 50.0f, 500.0f);

    //��������� �������� ������� �����
    CollisionBoxUp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxUp"));
    CollisionBoxUp->SetupAttachment(RootComponent);
    CollisionBoxUp->SetBoxExtent(SizeCollisionTube);

    //�������� ������ �����
    TubeDown = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("TubeDown"));
    TubeDown->SetFlipbook(TubeFlipbook);
    TubeDown->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    TubeDown->SetupAttachment(RootComponent);

    //��������� �������� ������ �����
    CollisionBoxDown = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxDown"));
    CollisionBoxDown->SetupAttachment(TubeDown);
    CollisionBoxDown->SetBoxExtent(SizeCollisionTube);

    float Offset = 1400.0f; // ���������� ����� �������
    FVector RelativeLocation = FVector(0.0f, 0.0f, Offset);
    TubeDown->AddRelativeLocation(RelativeLocation);// ������������ ���� �� ���������

    //��������� �������� ��� ���������� �����
    ScoreCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreCollision"));
    ScoreCollision->SetupAttachment(RootComponent);
    FVector SizeCollisionScore = FVector(10.0f, 10.0f, 170.0f);
    ScoreCollision->SetBoxExtent(SizeCollisionScore);
    //������������ �������� ����� ����� ������� � ��������� ���������
    RelativeLocation = FVector(-45.0f, 0.0f, Offset / 2);
    ScoreCollision->AddRelativeLocation(RelativeLocation);

    SpeedTube = 0.0f;
}

void ATube::BeginPlay()
{
    Super::BeginPlay();
    SpeedTube = -300.0f;
}

UBoxComponent* ATube::GetScoreCollision()
{
    return ScoreCollision;
}

UBoxComponent* ATube::GetCollisionBoxUp()
{
    return CollisionBoxUp;
}

UBoxComponent* ATube::GetCollisionBoxDown()
{
    return CollisionBoxDown;
}

void ATube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //����� ����� � �����
    FVector NewTubeLocation = GetActorLocation();
    NewTubeLocation.X += DeltaTime * SpeedTube;
    SetActorLocation(NewTubeLocation);
}
