// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "The_Wall_2018Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"

AThe_Wall_2018Character::AThe_Wall_2018Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PlayAnimGunEquipt = false;               // Character
	OnFire = false;              // Character
	GunEquipt = false;              // Character
	PlayAnimChangeAmmo = false;              // Character
	IsMyGunVisible = false;              // Character

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SpawnProjectilePoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnProjectilePoint->SetupAttachment(RootComponent);

	MyGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyGun"));              // Character
	MyGun->SetupAttachment(GetMesh());              // Character
}

void AThe_Wall_2018Character::SetMyGunVisibility()
{
	IsMyGunVisible = !IsMyGunVisible;
	MyGun->SetVisibility(IsMyGunVisible);

}

// Called when the game starts or when spawned
void AThe_Wall_2018Character::BeginPlay()
{
	Super::BeginPlay();

	MyGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocket"));
	MyGun->SetVisibility(false);
	if (AmmosForGun.Num() > 0)
	{
		CurrentAmmoIndex = 0;
		CurrentAmmo = AmmosForGun[CurrentAmmoIndex];
	}

	SpawnProjectilePoint->AttachToComponent(MyGun, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Muzzle"));
}


void AThe_Wall_2018Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void AThe_Wall_2018Character::ChangeAmoo()
{
	if (GunEquipt && !PlayAnimChangeAmmo && !OnFire)
	{
		PlayAnimChangeAmmo = !PlayAnimChangeAmmo;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Play Anim Change Ammo"));

		if (AmmosForGun.Num() > 0)
		{
			if (AmmosForGun.Num() - 1 == CurrentAmmoIndex)
			{
				CurrentAmmoIndex = 0;
				CurrentAmmo = AmmosForGun[CurrentAmmoIndex];
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, TEXT("Ammo: " + FString::FromInt(CurrentAmmoIndex)));
			}
			else
			{
				++CurrentAmmoIndex;
				CurrentAmmo = AmmosForGun[CurrentAmmoIndex];
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, TEXT("Ammo: " + FString::FromInt(CurrentAmmoIndex)));
			}
		}
	}
}

void AThe_Wall_2018Character::GetGun()
{
	//Reset playing equip animation
	if (!PlayAnimGunEquipt && !OnFire)
	{
		PlayAnimGunEquipt = true;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Play Anim Equipt"));
	}

	//UnEquipt Gun
	if (GunEquipt && !OnFire)
	{
		GunEquipt = !GunEquipt;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("UnEquipt"));
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AThe_Wall_2018Character::SetMyGunVisibility, 1, false);
	}
	//Equip Gun
	else if (!GunEquipt && !OnFire)
	{
		GunEquipt = !GunEquipt;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Equipt"));
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AThe_Wall_2018Character::SetMyGunVisibility, 1, false);
	}
}

void AThe_Wall_2018Character::Fire()
{
	if (OnFire)
	{
		FTransform SpawnTransform = SpawnProjectilePoint->GetComponentTransform();
		auto SpawnPrt = GetWorld()->SpawnActor(CurrentAmmo, &SpawnTransform);             // TODO Add SpawnRotation
		
		//SpawnedProjectile = (AProjectile*)GetWorld()->SpawnActor(CurrentAmmo, SpawnTransform);

		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AThe_Wall_2018Character::Fire, 1.35, false);
	}
}

void AThe_Wall_2018Character::StartFire()
{
	if (GunEquipt)
	{
		OnFire = true;
		Fire();
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Start Fire"));
	}
}


void AThe_Wall_2018Character::StopFire()
{
	if (GunEquipt && OnFire)
	{
		OnFire = false;
		Fire();
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Stop Fire"));
	}
}