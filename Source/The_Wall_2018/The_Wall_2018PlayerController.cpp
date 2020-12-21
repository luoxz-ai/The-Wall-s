// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "The_Wall_2018PlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "The_Wall_2018Character.h"

AThe_Wall_2018PlayerController::AThe_Wall_2018PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AThe_Wall_2018PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AThe_Wall_2018PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AThe_Wall_2018PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AThe_Wall_2018PlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AThe_Wall_2018PlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AThe_Wall_2018PlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AThe_Wall_2018PlayerController::OnResetVR);

	InputComponent->BindAction("GetGun", IE_Pressed, this, &AThe_Wall_2018PlayerController::GetGun);
	InputComponent->BindAction("ChangeAmoo", IE_Pressed, this, &AThe_Wall_2018PlayerController::ChangeAmoo);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AThe_Wall_2018PlayerController::StartFireController);
	InputComponent->BindAction("Fire", IE_Released, this, &AThe_Wall_2018PlayerController::StopFireController);
}

void AThe_Wall_2018PlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AThe_Wall_2018PlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AThe_Wall_2018Character* MyPawn = Cast<AThe_Wall_2018Character>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
			
		}
	}
}

void AThe_Wall_2018PlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
		
	}
}

void AThe_Wall_2018PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AThe_Wall_2018PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AThe_Wall_2018PlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AThe_Wall_2018PlayerController::GetGun()
{
	if (AThe_Wall_2018Character* MyPawn = Cast<AThe_Wall_2018Character>(GetPawn()))
	{
		{
			MyPawn->GetGun();
		}
	}
}

void AThe_Wall_2018PlayerController::ChangeAmoo()
{
	if (AThe_Wall_2018Character* MyPawn = Cast<AThe_Wall_2018Character>(GetPawn()))
	{
		{
			MyPawn->ChangeAmoo();
		}
	}
}

void AThe_Wall_2018PlayerController::StartFireController()
{
	if (AThe_Wall_2018Character* MyPawn = Cast<AThe_Wall_2018Character>(GetPawn()))
	{
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Send Start Fire"));
			MyPawn->StartFire();
		}
	}
}

void AThe_Wall_2018PlayerController::StopFireController()
{
	if (AThe_Wall_2018Character* MyPawn = Cast<AThe_Wall_2018Character>(GetPawn()))
	{
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Send Stop Fire"));
			MyPawn->StopFire();
		}
	}
}
