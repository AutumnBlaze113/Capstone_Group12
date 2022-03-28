// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CapCharacter.h"
#include "Monster.generated.h"

UCLASS()
class CAP_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere)
		bool isPaused;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartMovement();
	void StopMovement();


private:
	ACapCharacter* Player;

	FVector selfPositon; 
	FVector playerPosition;

	FVector velocity;
	float speed;

	float distance;

	bool isStunned;

};
