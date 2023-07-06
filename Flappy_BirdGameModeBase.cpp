
#include "Flappy_BirdGameModeBase.h"

AFlappy_BirdGameModeBase::AFlappy_BirdGameModeBase()
{

	DefaultPawnClass = ABird::StaticClass();
	PlayerControllerClass = AControllerForMouse::StaticClass();
}