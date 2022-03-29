// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "kismet/gameplaystatics.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isStunned = false;
	isPaused = false;
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
	
	Player->puzzleStarted.BindUObject(this, &AMonster::StopMovement);
	Player->puzzleEnded.BindUObject(this, &AMonster::StartMovement);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	if (!isStunned && !isPaused) { // if the monster is not stunned or paused due to puzzles
		if (distance >= 100.0f) {// if the monster does not reach the player
			selfPositon += DeltaTime * velocity; // update position
			SetActorLocation(selfPositon);
		}

		else { // if the monster reaches player
			Player->TakeDamage(); // player evaluate health value
		}

			selfPositon = GetActorLocation();//update self position
			playerPosition = Player->GetActorLocation();//update player position

			velocity.Normalize();
			velocity *= speed; // update velcity
		
		
	}


	if (Player->stunning <= 0.0f) {// if player not stuns monster
		isStunned = false;
	}
	else {// if stuns
		isStunned = true;
	}
	velocity = playerPosition - selfPositon; // update direction

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

void AMonster::StartMovement() {
	isPaused = false;
}

void AMonster::StopMovement() {
	isPaused = true;
}