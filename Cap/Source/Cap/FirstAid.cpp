// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstAid.h"
#include "kismet/gameplaystatics.h"

// Sets default values
AFirstAid::AFirstAid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	coolDown = 0.0f;
}

// Called when the game starts or when spawned
void AFirstAid::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ACapCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));// Call player status
	
}

// Called every frame
void AFirstAid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (coolDown > 0.0f) {
		coolDown -= DeltaTime;
	}

}

void AFirstAid::ProvideFirstAid()
{
	if (Player->health < Player->maxHealth) {
		Player->health = Player->maxHealth;
		Player->energy = Player->maxEnergy;
		coolDown = 30.0f;
	}
}

