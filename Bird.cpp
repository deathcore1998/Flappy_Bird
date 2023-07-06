
#include "Bird.h"
#include "GameManager.h"


ABird::ABird()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// �������� �������� �����
	GetCapsuleComponent()->SetCapsuleHalfHeight(55.0f);
	GetCapsuleComponent()->SetCapsuleRadius(55.0f);

	// ���������
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//�������� ����� � ��������� Flipbook 
	BirdFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BirdFlipbook"));
	BirdFlipbook->SetupAttachment(RootComponent);
	AnimationBird = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/Bird.Bird"));
	BirdFlipbook->SetFlipbook(AnimationBird);

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2100;//2100
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// ���������� � ���� ������
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->JumpZVelocity = 900.f;

	//���� �������� ���������� ��� ���
	Gravity = false;

	// �������� �������� ������
	JumpSoundWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/jump.jump"));
	DieSoundWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/die.die"));
	PointSoundWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/point.point"));
}


void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABird::OnBirdCollision);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABird::IncrementScore);
	FVector BirdPosition = GetActorLocation();
	CameraBoom->SetWorldLocation(FVector(BirdPosition.X, BirdPosition.Y, BirdPosition.Z - 200));
}

void ABird::MyJump()
{
	Super::Jump();

	if (Gravity == false)
	{
		GravitationON();
		GameManager->StartSpawnTube();
	}

	if (JumpSoundWave)	// �������� ������� ��������� �����
	{
		UGameplayStatics::PlaySound2D(this, JumpSoundWave);//��������������� �����
	}
}

void ABird::SetGameManager(AGameManager* Manager)
{
	GameManager = Manager;
}

void ABird::GravitationON()
{
	GetCharacterMovement()->GravityScale = 2.0f;
	Gravity = true;
}

void ABird::GravitationOFF()
{
	GetCharacterMovement()->GravityScale = 0.0f;

	// ����� ������������ ��������
	GetCharacterMovement()->Velocity.Z = 0.0f;
	Gravity = false;
}

void ABird::OnBirdCollision(UPrimitiveComponent* OverlappendComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodeIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)//�������� �� ������������� ������������
	{
		UBoxComponent* CollisionUp = nullptr;
		UBoxComponent* CollisionDown = nullptr;

		if (OtherActor->IsA(ATube::StaticClass()))// ������������ � �������
		{
			CollisionUp = Cast<ATube>(OtherActor)->GetCollisionBoxUp();
			CollisionDown = Cast<ATube>(OtherActor)->GetCollisionBoxDown();
		}
		else if (OtherActor->IsA(AGameManager::StaticClass()))// ������������ � ������� ��� �����
		{
			CollisionUp = Cast<AGameManager>(OtherActor)->GetCollisionBoxSky();
			CollisionDown = Cast<AGameManager>(OtherActor)->GetCollisionBoxGround();
		}

		if (CollisionUp && otherComp == CollisionUp || CollisionDown && otherComp == CollisionDown)//���� ���� �� ������������ ����
		{
			// �������������� ������ ������
			GameManager->ShowDeathMenu();
			if (DieSoundWave)	// �������� ������� ��������� �����
			{
				UGameplayStatics::PlaySound2D(this, DieSoundWave);//��������������� �����
			}
		}
	}
}

void ABird::IncrementScore(UPrimitiveComponent* OverlappendComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodeIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ATube::StaticClass()))//������������ � ��������� ����� 
	{
		UBoxComponent* IncrScore = Cast<ATube>(OtherActor)->GetScoreCollision();

		if (IncrScore && otherComp == IncrScore)//�������� �� ������������� ������������
		{
			GameManager->IncrementScore();
			if (PointSoundWave)	// �������� ������� ��������� �����
			{
				UGameplayStatics::PlaySound2D(this, PointSoundWave);//��������������� �����
			}
		}
	}
}

void ABird::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABird::MyJump);
}

bool ABird::CanJumpInternal_Implementation() const
{
	return true;
}