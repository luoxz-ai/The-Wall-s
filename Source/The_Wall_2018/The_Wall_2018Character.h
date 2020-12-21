// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "The_Wall_2018Character.generated.h"

UCLASS(Blueprintable)
class AThe_Wall_2018Character : public ACharacter
{
	GENERATED_BODY()

public:
	AThe_Wall_2018Character();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultMyChar")
	TArray < TSubclassOf<AProjectile> > AmmosForGun;              // Character
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MyGun;              // Character
	
	UFUNCTION(BlueprintCallable, Category = "Ragnar")
	void StartFire();              // Character
	UFUNCTION(BlueprintCallable, Category = "Ragnar")
	void StopFire();              // Character
	UFUNCTION(BlueprintCallable, Category = "Ragnar")
	void GetGun();              // Character
	UFUNCTION(BlueprintCallable, Category = "Ragnar")
	void ChangeAmoo();              // Character
	
	UPROPERTY(BlueprintReadWrite)
	bool PlayAnimGunEquipt;              // Character

	UPROPERTY(BlueprintReadWrite)
	bool OnFire;              // Character

	UPROPERTY(BlueprintReadWrite)
	bool PlayAnimChangeAmmo;              // Character

	UFUNCTION()
	void SetMyGunVisibility();              // Character

	UPROPERTY()
	AProjectile* SpawnedProjectile;              // Character

	
	


	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsMyGunVisible;              // Character
	
	//void ChangeAmoo();              // Character
	//void GetGun();              // Character

	void Fire();              // Character

	UPROPERTY()
	bool GunEquipt;              // Character

	UPROPERTY()
	USceneComponent* SpawnProjectilePoint;              // Character

	UPROPERTY()
	TSubclassOf<AProjectile> CurrentAmmo;              // Character

	UPROPERTY()
	int32 CurrentAmmoIndex;              // Character

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
};

