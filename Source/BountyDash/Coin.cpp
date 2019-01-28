// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin.h"
#include "BountyDash.h"
#include "BountyDashChararcter.h"
#include "BountyDashGameMode.h"
#include "Obstacle.h"

ACoin::ACoin()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OverlapAllDynamic");
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddLocalRotation(FRotator(5.0f, 0.0f, 0.0f));
}

void ACoin::MyOnActorOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass())&&!BeingPulled)
	{
		USphereComponent* thisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));
		USphereComponent* otherSphere = Cast<USphereComponent>(OtherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere)
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius()));
		}
	}

	if (OtherActor->GetClass()->IsChildOf(ABountyDashChararcter::StaticClass()))
	{
		ABountyDashChararcter*myChar = Cast<ABountyDashChararcter>(OtherActor);
		myChar->ScoreUp();
		GetWorld()->DestroyActor(this);
	}
}