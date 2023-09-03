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
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	cameraBoom->SetUsingAbsoluteRotation(true);
	cameraBoom->bDoCollisionTest = false;
	cameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//Создание птицы и установка Flipbook 
	birdFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BirdFlipbook"));
	birdFlipbook->SetupAttachment(RootComponent);
	animationBird = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/Bird.Bird"));
	birdFlipbook->SetFlipbook(animationBird);

	sideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	sideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	sideViewCameraComponent->OrthoWidth = 2100;//2100
	sideViewCameraComponent->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

	cameraBoom->SetUsingAbsoluteRotation(true);
	sideViewCameraComponent->bUsePawnControlRotation = false;
	sideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// Гравитация и сила прыжка
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->JumpZVelocity = 900.f;
	//Флаг включена гравитация или нет
	gravity = false;
	// Загрузка звуковых файлов
	jumpSoundWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/jump.jump"));
	dieSoundWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/die.die"));
	pointSoundWave = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/point.point"));
}

void ABird::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABird::onBirdCollision);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABird::incrementScore);
	FVector birdPosition = GetActorLocation();
	cameraBoom->SetWorldLocation(FVector(birdPosition.X, birdPosition.Y, birdPosition.Z - 200));
}

void ABird::myJump()
{
	Super::Jump();

	if (gravity == false) {
		gravitationON();
		gameManager->startSpawnTube();
	}
	// Проверка наличия звукового файла
	if (jumpSoundWave) {
		UGameplayStatics::PlaySound2D(this, jumpSoundWave);//Воспроизведение звука
	}
}

void ABird::setGameManager(AGameManager* manager)
{
	gameManager = manager;
}

void ABird::gravitationON()
{
	GetCharacterMovement()->GravityScale = 2.0f;
	gravity = true;
}

void ABird::gravitationOFF()
{
	GetCharacterMovement()->GravityScale = 0.0f;
	// Сброс вертикальной скорости
	GetCharacterMovement()->Velocity.Z = 0.0f;
	gravity = false;
}

void ABird::onBirdCollision(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	//Проврека на возникновение столкновения
	if (otherActor) {
		UBoxComponent* collisionUp = nullptr;
		UBoxComponent* collisionDown = nullptr;
		
		if (otherActor->IsA(ATube::StaticClass())) { // столкновение с трубами
			collisionUp = Cast<ATube>(otherActor)->getCollisionBoxUp();
			collisionDown = Cast<ATube>(otherActor)->getCollisionBoxDown();
		}
		else if (otherActor->IsA(AGameManager::StaticClass())) {// столкновение с заемлей или небом	
			collisionUp = Cast<AGameManager>(otherActor)->getCollisionBoxSky();
			collisionDown = Cast<AGameManager>(otherActor)->getCollisionBoxGround();
		}
		//Если одно из столкновений было
		if (collisionUp && otherComp == collisionUp || collisionDown && otherComp == collisionDown) {
			// отрабатывается логика смерти
			gameManager->showDeathMenu();
			if (dieSoundWave) {// Проверка наличия звукового файла	
				UGameplayStatics::PlaySound2D(this, dieSoundWave);//Воспроизведение звука
			}
		}
	}
}

void ABird::incrementScore(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	//столкновение с коллизией счета 
	if (otherActor && otherActor->IsA(ATube::StaticClass())) {
		UBoxComponent* incrScore = Cast<ATube>(otherActor)->getScoreCollision();
		//Проврека на возникновение столкновения
		if (incrScore && otherComp == incrScore) {
			gameManager->incrementScore();
			// Проверка наличия звукового файла
			if (pointSoundWave) {	
				UGameplayStatics::PlaySound2D(this, pointSoundWave);//Воспроизведение звука
			}
		}
	}
}

void ABird::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABird::myJump);
}

bool ABird::CanJumpInternal_Implementation() const
{
	return true;
}