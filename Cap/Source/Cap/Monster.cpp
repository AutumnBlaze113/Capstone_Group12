// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "kismet/gameplaystatics.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isStunned = false;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ACapCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));// Call player status
	selfPositon = GetActorLocation();
	playerPosition = Player->GetActorLocation();
	speed = 600.0f;
	speed = 0.0f;
	velocity = playerPosition - selfPositon;
	distance = velocity.Size();
	velocity.Normalize();
	velocity *= speed;
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	if (!isStunned) {
		if (distance >= 100.0f) {
			selfPositon += DeltaTime * velocity;
			SetActorLocation(selfPositon);
		}

		selfPositon = GetActorLocation();
		playerPosition = Player->GetActorLocation();

		velocity.Normalize();
		velocity *= speed;
	}

	if (Player->stunning <= 0.0f) {
		isStunned = false;
	}
	else {
		isStunned = true;
	}
	velocity = playerPosition - selfPositon;

	distance = velocity.Size();

	Player->distance = distance;
	Player->direction.X = -velocity.X;
	Player->direction.Y = -velocity.Y;
	
	
	

	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

