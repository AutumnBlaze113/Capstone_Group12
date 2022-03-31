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

	totalKey = 5;
	keysLeft = 5;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	power = false;
	passcode = false;
	test = false;

	PuzzleCd = 0.0f;
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
	if (PuzzleCd > 0.0f) {
		PuzzleCd -= DeltaTime;
	}
	Super::Tick(DeltaTime);
}

void ACapGameMode::GetKey()
{
	keysLeft--;
	if (keysLeft <= 0) { isRunning = false; }
}