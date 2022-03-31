// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CapGameMode.generated.h"

UCLASS(minimalapi)
class ACapGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACapGameMode();

	UPROPERTY(BlueprintReadWrite)
		float currentTime;

	UPROPERTY(BlueprintReadWrite)
		bool isRunning;

	UPROPERTY(BlueprintReadWrite)
		int32 keysLeft;

	UPROPERTY(BlueprintReadWrite)
		int32 totalKey;

	UPROPERTY(BlueprintReadWrite)
		bool power;
	UPROPERTY(BlueprintReadWrite)
		bool passcode;
	UPROPERTY(BlueprintReadWrite)
		bool test;

	UPROPERTY(BlueprintReadWrite)
		float PuzzleCd;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void GetKey();
	
};



