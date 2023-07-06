
#include "Bird.h"
#include "GameManager.h"


ABird::ABird()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Утановка коллизии птицы
	GetCapsuleComponent()->SetCapsuleHalfHeight(55.0f);
	GetCapsuleComponent()->SetCapsuleRadius(55.0f);

	// Камерабум
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//Создание птицы и установка Flipbook 
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

	// Гравитация и сила прыжка
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->JumpZVelocity = 900.f;

	//Флаг включена гравитация или нет
	Gravity = false;

	// Загрузка звуковых файлов
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

	if (JumpSoundWave)	// Проверка наличия звукового файла
	{
		UGameplayStatics::PlaySound2D(this, JumpSoundWave);//Воспроизведение звука
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

	// Сброс вертикальной скорости
	GetCharacterMovement()->Velocity.Z = 0.0f;
	Gravity = false;
}

void ABird::OnBirdCollision(UPrimitiveComponent* OverlappendComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodeIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)//Проврека на возникновение столкновения
	{
		UBoxComponent* CollisionUp = nullptr;
		UBoxComponent* CollisionDown = nullptr;

		if (OtherActor->IsA(ATube::StaticClass()))// столкновение с трубами
		{
			CollisionUp = Cast<ATube>(OtherActor)->GetCollisionBoxUp();
			CollisionDown = Cast<ATube>(OtherActor)->GetCollisionBoxDown();
		}
		else if (OtherActor->IsA(AGameManager::StaticClass()))// столкновение с заемлей или небом
		{
			CollisionUp = Cast<AGameManager>(OtherActor)->GetCollisionBoxSky();
			CollisionDown = Cast<AGameManager>(OtherActor)->GetCollisionBoxGround();
		}

		if (CollisionUp && otherComp == CollisionUp || CollisionDown && otherComp == CollisionDown)//Если одно из столкновений было
		{
			// отрабатывается логика смерти
			GameManager->ShowDeathMenu();
			if (DieSoundWave)	// Проверка наличия звукового файла
			{
				UGameplayStatics::PlaySound2D(this, DieSoundWave);//Воспроизведение звука
			}
		}
	}
}

void ABird::IncrementScore(UPrimitiveComponent* OverlappendComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodeIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ATube::StaticClass()))//столкновение с коллизией счета 
	{
		UBoxComponent* IncrScore = Cast<ATube>(OtherActor)->GetScoreCollision();

		if (IncrScore && otherComp == IncrScore)//Проврека на возникновение столкновения
		{
			GameManager->IncrementScore();
			if (PointSoundWave)	// Проверка наличия звукового файла
			{
				UGameplayStatics::PlaySound2D(this, PointSoundWave);//Воспроизведение звука
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