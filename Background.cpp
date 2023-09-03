#include "Background.h"

ABackground::ABackground()
{
	PrimaryActorTick.bCanEverTick = true;

	backGround = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("backGround"));
	RootComponent = backGround;
	backGroundFlipbook = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/FlipBook/Background_Flipbook.Background_Flipbook"));
}

void ABackground::BeginPlay()
{
	Super::BeginPlay();
	backGround->SetFlipbook(backGroundFlipbook);
}