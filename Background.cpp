#include "Background.h"

ABackground::ABackground()
{
	PrimaryActorTick.bCanEverTick = true;

	BackGround = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BackGround"));
	RootComponent = BackGround;
	BackGroundFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/Background_Flipbook.Background_Flipbook"));
}

void ABackground::BeginPlay()
{
	Super::BeginPlay();
	BackGround->SetFlipbook(BackGroundFlipbook);
}