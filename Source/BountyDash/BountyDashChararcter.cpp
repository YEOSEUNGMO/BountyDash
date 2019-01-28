// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashChararcter.h"
#include "BountyDash.h"
#include "BountyDashGameMode.h"
#include "Obstacle.h"
#include "Coin.h"
#include "Floor.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "Engine/TargetPoint.h"

// Sets default values
ABountyDashChararcter::ABountyDashChararcter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	ConstructorHelpers::FObjectFinder<UAnimBlueprint>myAnimBP(TEXT("/Game/Barrel_Hopper/Chararcter/BH_Character_AnimBP.BH_Character_AnimBP"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>myMesh(TEXT("/Game/Barrel_Hopper/Chararcter/Mesh/SK_Mannequin.SK_Mannequin"));
	if (myMesh.Succeeded() && myAnimBP.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnimBP.Object->GeneratedClass);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check(CameraBoom);

	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	check(FollowCamera);

	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->AddRelativeRotation(FQuat(FRotator(-10.0f, 0.0f, 0.0f)));

	CharSpeed = 10.0f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABountyDashChararcter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABountyDashChararcter::MyOnComponentEndOverlap);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	hitObstacleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	hitObstacleSound->bAutoActivate = false;
	hitObstacleSound->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue>mySoundcue(TEXT("/Game/Sounds/GruntCue.GruntCue"));

	if (mySoundcue.Succeeded())
	{
		hitObstacleSound->SetSound(mySoundcue.Object);
	}

	dingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Ding"));
	dingSound->bAutoActivate = false;
	dingSound->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue> myCue(TEXT("/Game/Sounds/DinCue.DinCue"));

	if (myCue.Succeeded())
	{
		dingSound->SetSound(myCue.Object);
	}
}

// Called when the game starts or when spawned
void ABountyDashChararcter::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		TargetArray.Add(*TargetIter);
	}

	auto SortPred = [](const AActor& A, const AActor& B)->bool
	{
		return(A.GetActorLocation().Y < B.GetActorLocation().Y);
	};
	TargetArray.Sort(SortPred);

	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);

	for (TActorIterator<AFloor> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		KillPoint = TargetIter->GetKillPoint();
	}
}

// Called every frame
void ABountyDashChararcter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TargetArray.Num() > 0)
	{
		FVector targetLoc = TargetArray[CurrentLocation]->GetActorLocation();
		targetLoc.Z = GetActorLocation().Z;
		targetLoc.X = GetActorLocation().X;

		if (targetLoc != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), targetLoc, CharSpeed*DeltaTime));
		}

	}

	if (bBeingPushed)
	{
		float moveSpeed = GetCustomGameMode<ABountyDashGameMode>(GetWorld())->GetInvGameSpeed();
		AddActorLocalOffset(FVector(moveSpeed, 0.0f, 0.0f));
	}

	if (CanMagnet)
	{
		CoinMagnet();
	}

	if (GetActorLocation().X < KillPoint)
	{
		GetCustomGameMode<ABountyDashGameMode>(GetWorld())->GameOver();
	}
}

// Called to bind functionality to input
void ABountyDashChararcter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("MoveRight", IE_Pressed,this,&ABountyDashChararcter::MoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABountyDashChararcter::MoveLeft);
	InputComponent->BindAction("Reset", IE_Pressed, this, &ABountyDashChararcter::Reset).bExecuteWhenPaused = true;


}

void ABountyDashChararcter::MoveRight()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation < TargetArray.Num() - 1)
		{
			++CurrentLocation;
		}
		else
		{
		}
	}
}

void ABountyDashChararcter::MoveLeft()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
		else
		{
		}
	}
}

void ABountyDashChararcter::MyOnComponentOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		FVector vecBetween = OtherActor->GetActorLocation() - GetActorLocation();
		float AngleBetween = FMath::Acos(FVector::DotProduct(vecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));

		AngleBetween *= (180 / PI);

		if (AngleBetween < 60.0f)
		{
			AObstacle* pObs = Cast<AObstacle>(OtherActor);
			if (!bBeingPushed)
			{
				hitObstacleSound->Play();
			}
			if (pObs&&CanSmash)
			{
				pObs->GetDestructable()->ApplyRadiusDamage(10000, GetActorLocation(), 10000, 10000, true);
			}
			else
			{
				bBeingPushed = true;
			}
		}
	}
}

void  ABountyDashChararcter::MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		bBeingPushed = false;
	}
}

void  ABountyDashChararcter::ScoreUp()
{
	Score++;
	GetCustomGameMode<ABountyDashGameMode>(GetWorld())->CharScoreUp(Score);
	dingSound->Play();
}

void ABountyDashChararcter::PowerUp(EPowerUp Type)
{
	switch (Type)
	{
	case EPowerUp::SPEED:
	{
		GetCustomGameMode<ABountyDashGameMode>(GetWorld())->ReduceGameSpeed();
		break;
	}
	case EPowerUp::SMASH:
	{
		CanSmash = true;
		FTimerHandle newTimer;
		GetWorld()->GetTimerManager().SetTimer(newTimer, this, &ABountyDashChararcter::StopSmash, SmashTime, false);
		break;
	}
	case EPowerUp::MAGNET:
	{
		CanMagnet = true;
		FTimerHandle newTimer;
		GetWorld()->GetTimerManager().SetTimer(newTimer, this, &ABountyDashChararcter::StopMagnet, MagnetTime, false);
		break;
	}
	default:
		break;
	}
}

void ABountyDashChararcter::StopMagnet()
{
	CanMagnet = false;
}

void ABountyDashChararcter::StopSmash()
{
	CanSmash = false;
}

void ABountyDashChararcter::CoinMagnet()
{
	for (TActorIterator<ACoin> coinIter(GetWorld()); coinIter; ++coinIter)
	{
		FVector between = GetActorLocation() - coinIter->GetActorLocation();
		if (FMath::Abs(between.Size()) < MagetReach)
		{
			FVector coinPos = FMath::Lerp((*coinIter)->GetActorLocation(), GetActorLocation(), 0.2f);
			(*coinIter)->SetActorLocation(coinPos);
			(*coinIter)->BeingPulled = true;
		}
	}
}

int ABountyDashChararcter::GetScore()
{
	return Score;
}

void ABountyDashChararcter::Reset()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BountyDashMap"));
}