// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACapCharacter

ACapCharacter::ACapCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	isSprinting = false;
	health = 3;
	maxHealth = 3;
	immortal = 0.0f;

	distance = 0.0f;
	stunning = 0.0f;

	energy = 10.0f;
	maxEnergy = 10.0f;
	sprintEnable = true;

	shortcut = 1;
	pills = 1;
	stimulants = 1;
	flash = 1;
	vaccine = 0;

	toBePick = 0;
	keyPick = 0; 
	PickTime = 0;

	tool = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACapCharacter::BeginPlay()
{
	Super::BeginPlay();
	MovementPtr = Cast<UCharacterMovementComponent>(GetCharacterMovement());

}

void ACapCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACapCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACapCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACapCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACapCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACapCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACapCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACapCharacter::OnResetVR);
}

void ACapCharacter::Tick(float DeltaTime)
{
	if (isSprinting) {
		energy -= (2.0f * DeltaTime);
		if (energy <= 0.0f) { 
			energy = 0; 
			sprintEnable = false;
			EndSprint();
		}
	}
	else if (energy < maxEnergy){
		energy += DeltaTime;
		if (energy >= maxEnergy) {
			sprintEnable = true;
			energy = maxEnergy;
		}
	}

	if (immortal > 0.0f) {
		immortal -= DeltaTime;
	}

	if (stunning > 0.0f) {
		stunning -= DeltaTime;
	}

	
	
}

void ACapCharacter::StartSprint()
{
	if (sprintEnable) {
		isSprinting = true;
		MovementPtr->MaxWalkSpeed = 1800.0f;
	}
	
	
}

void ACapCharacter::EndSprint()
{	
	isSprinting = false;
	MovementPtr->MaxWalkSpeed = 600.0f;
}

void ACapCharacter::TakeDamage()
{
	if (immortal <= 0.0f) {
		health--;
		immortal = 3.0f;
	}
}

void ACapCharacter::UseShortCut()
{
	switch (shortcut) {
	case 1:
		UseFlash();
		break;
	
	case 2:
		UsePill();
		break;

	case 3:
		UseStimulants();
		break;

	case 4:
		UseVaccine();
		break;


	default:
		;
	}
}

void ACapCharacter::UseFlash()
{
	if (distance <= 1000 && flash > 0 && stunning <= 0) {
		flash--;
		stunning = 10.0f;
	}
}

void ACapCharacter::UsePill()
{
	if (health < maxHealth && pills > 0 && health > 0) {
		pills--;
		health++;
	}
}

void ACapCharacter::UseStimulants()
{
	stimulants--;
	energy = maxEnergy;
}

void ACapCharacter::UseVaccine()
{
	if (immortal < 2.0f && vaccine > 0) {
		vaccine--;
		immortal = 6.0f;
	}
}

void ACapCharacter::SetShortCut(int32 name)
{
	shortcut = name;
}


void ACapCharacter::OnResetVR()
{
	// If Cap is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Cap.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACapCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACapCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACapCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACapCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACapCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACapCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACapCharacter::SetPuzzle(bool _inPuzzle) {
	inPuzzle = _inPuzzle;
	if (inPuzzle) {
		puzzleStarted.ExecuteIfBound();
	}
	else {
		puzzleEnded.ExecuteIfBound();
	}
}