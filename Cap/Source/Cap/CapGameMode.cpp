// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapGameMode.h"
#include "CapCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACapGameMode::ACapGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	currentTime = 300.0f;
	isRunning = true;

	totalKey = 10;
	keysLeft = 10;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ACapGameMode::Tick(float DeltaTime)
{
	
	if (isRunning) {
		currentTime -= DeltaTime;

	}
	if (currentTime <= 0.0f)
	{
		currentTime = 0.0f;
		isRunning = false;
	}
	Super::Tick(DeltaTime);
}

void ACapGameMode::GetKey()
{
	keysLeft--;
	if (keysLeft <= 0) { isRunning = false; }
}