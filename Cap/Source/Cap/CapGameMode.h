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

	UPROPERTY(BlueprintReadOnly)
		float currentTime;

	UPROPERTY(BlueprintReadWrite)
		bool isRunning;

	UPROPERTY(BlueprintReadWrite)
		int32 keysLeft;

	UPROPERTY(BlueprintReadWrite)
		int32 totalKey;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void GetKey();
};



