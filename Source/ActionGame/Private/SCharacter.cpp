// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGame/Public/SCharacter.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i< GetMesh()->GetNumMaterials(); ++i)
	{
		UMaterialInstanceDynamic* NewDynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(i), this);
		GetMesh()->SetMaterial(i, NewDynamicMaterial);
		MatInsts.Add(NewDynamicMaterial);
	}
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ASCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction(TEXT("SecondaryAttack"), IE_Pressed, this, &ASCharacter::SecondaryAttack);

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &ASCharacter::Dash);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ASCharacter::SprintStop);
	
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, TEXT("PrimaryAttack"));
}

void ASCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, TEXT("SecondaryAttack"));
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ASCharacter::Dash()
{
	ActionComponent->StartActionByName(this, TEXT("Dash"));
}

void ASCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, TEXT("Sprint"));
}

void ASCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, TEXT("Sprint"));
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		DisableInput(Cast<APlayerController>(GetController()));

		SetLifeSpan(5.0f); 
	}
	else if (Delta < 0.0f)
	{
		for(const auto MatInst : MatInsts)
		{
			MatInst->SetScalarParameterValue(TEXT("TimeToHit"), GetWorld()->TimeSeconds);
		}
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}
