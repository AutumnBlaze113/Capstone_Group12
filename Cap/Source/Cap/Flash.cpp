// Fill out your copyright notice in the Description page of Project Settings.


#include "Flash.h"
#include "kismet/gameplaystatics.h"

// Sets default values
AFlash::AFlash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlash::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ACapCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));// Call player status
}

// Called every frame
void AFlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

