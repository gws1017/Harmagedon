// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Objects/CainRock.h"
#include "Actor/Character/PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

// Sets default values
ACainRock::ACainRock()
{
    // Create and set up a sphere collision component
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    RootComponent = CollisionComponent;

    // Bind the OnHit function
    CollisionComponent->OnComponentHit.AddDynamic(this, &ACainRock::OnHit);

    // Create and set up the projectile movement component
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ACainRock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACainRock::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
    if (player && player->GetCapsuleComponent() == OtherComp)
    {
        //돌던지기 패턴 데미지
        UGameplayStatics::ApplyDamage(player, 321.0f, GetInstigatorController(), this, nullptr);
        Destroy();
    }
}

