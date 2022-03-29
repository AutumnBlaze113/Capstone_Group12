// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CapCharacter.generated.h"

DECLARE_DELEGATE(FOnPuzzleStart);

DECLARE_DELEGATE(FOnPuzzleEnd);

UCLASS(config=Game)
class ACapCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ACapCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UCharacterMovementComponent* MovementPtr;

	UPROPERTY(BlueprintReadWrite)
		bool isSprinting;

	UPROPERTY(BlueprintReadWrite)
		int32 health;
	UPROPERTY(BlueprintReadWrite)
		int32 maxHealth;
	UPROPERTY(BlueprintReadWrite)
		float immortal;

	UPROPERTY(BlueprintReadWrite)
		float energy;
	UPROPERTY(BlueprintReadWrite)
		float maxEnergy;
	UPROPERTY(BlueprintReadWrite)
		bool sprintEnable;

	UPROPERTY(BlueprintReadWrite)
		int32 shortcut;

	UPROPERTY(BlueprintReadWrite)
		int32 pills;
	UPROPERTY(BlueprintReadWrite)
		int32 stimulants;
	UPROPERTY(BlueprintReadWrite)
		int32 flash;
	UPROPERTY(BlueprintReadWrite)
		int32 vaccine;

	UPROPERTY(BlueprintReadOnly)
		float distance;
	UPROPERTY(BlueprintReadOnly)
		FVector2D direction;
	UPROPERTY(BlueprintReadWrite)
		float stunning;

	UPROPERTY(BlueprintReadOnly)
		int toBePick; // 0 nothing, 1 flash, 2 pill, 3 energy
	UPROPERTY(BlueprintReadOnly)
		float keyPick; // pick time, -1 unavaiable
	UPROPERTY(BlueprintReadOnly)
		float PickTime; // pick time for items

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool inPuzzle; //is the player in a puzzle

	FOnPuzzleStart puzzleStarted;
	FOnPuzzleEnd puzzleEnded;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void StartSprint();
	UFUNCTION(BlueprintCallable)
		void EndSprint();

	UFUNCTION(BlueprintCallable)
		void TakeDamage();

	UFUNCTION(BlueprintCallable)
		void UseShortCut();
	UFUNCTION(BlueprintCallable)
		void UseFlash();
	UFUNCTION(BlueprintCallable)
		void UsePill();
	UFUNCTION(BlueprintCallable)
		void UseStimulants();
	UFUNCTION(BlueprintCallable)
		void UseVaccine();

	UFUNCTION(BlueprintCallable)
		void SetShortCut(int32 name);

	UFUNCTION(BlueprintCallable)
		void SetPuzzle(bool _inPuzzle);
};

