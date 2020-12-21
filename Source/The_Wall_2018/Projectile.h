// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.generated.h"

UCLASS()
class THE_WALL_2018_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetup")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetup")
	UMaterialInstance* ProjectileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetup")
	float ProjectileSpeed;

	UFUNCTION()
	void OnHit(AActor* OtherActor);

	UFUNCTION()
	void SetSpeed(float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
