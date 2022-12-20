// Fill out your copyright notice in the Description page of Project Settings.


#include "HRCopterPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

AHRCopterPawn::AHRCopterPawn():
	MainBladeAttachSocketName("MainRotorBladeSocket"),
	TailBladeAttachSocketName("TailRotorBladeSocket"),
	bCanChangeBladesToBlurBlades(false),
	ForwardSpeedFactor(600.f),
	ForwardSpeedAccel(2.f),
	ForwardInclineFactor(-0.008333f),
	RightSpeedFactor(300.f),
	RightSpeedAccel(2.f),
	RightInclineFactor(0.016667f),
	UpSpeedFactor(200.f),
	UpSpeedAccel(2.f),
	YawRotateSpeedFactor(100.f),
	YawRotateSpeedAccel(2.f),
	MainRotorBaseSpeed(400.f)
{
	PrimaryActorTick.bCanEverTick = true;

	CopterBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CopterBody"));
	SetRootComponent(CopterBody);
	CopterBody->SetCollisionProfileName(FName(TEXT("BlockAll")));

	MainBlades = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBlades"));
	MainBlades->SetupAttachment(CopterBody, MainBladeAttachSocketName);
	TailBlades = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailBlades"));
	TailBlades->SetupAttachment(CopterBody, TailBladeAttachSocketName);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;

}

void AHRCopterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO Move Physics Logic in Sub-step Tick
	// Handle Movement Speed
	ForwardSpeed = FMath::FInterpTo(ForwardSpeed, TargetForwardSpeed, GetWorld()->GetDeltaSeconds(), ForwardSpeedAccel);
	RightSpeed = FMath::FInterpTo(RightSpeed, TargetRightSpeed, GetWorld()->GetDeltaSeconds(), RightSpeedAccel);
	UpSpeed = FMath::FInterpTo(UpSpeed, TargetUpSpeed, GetWorld()->GetDeltaSeconds(), UpSpeedAccel);
	YawRotSpeed = FMath::FInterpTo(YawRotSpeed, TargetYawRotSpeed, GetWorld()->GetDeltaSeconds(), YawRotateSpeedAccel);

	// NOTE Might not work with Irregular Terrain
	// Try Line Trace from Below the Helicopter and then Project Depending on the Impact Normal. 
	const FVector ProjectedForwardAndRightVector = FVector::VectorPlaneProject((GetActorForwardVector() * ForwardSpeed) + (GetActorRightVector() * RightSpeed), FVector(0.f, 0.f, 1.f));
	DeltaLocation = (ProjectedForwardAndRightVector + (GetActorUpVector() * UpSpeed)) * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(DeltaLocation, true);

	const float NewCopterRotRoll = RightSpeed * RightInclineFactor;
	const float NewCopterRotPitch = ForwardSpeed * ForwardInclineFactor;
	const float NewCopterRotYaw = (YawRotSpeed * GetWorld()->GetDeltaSeconds()) + GetActorRotation().Yaw;
	const FRotator NewCopterRotation(NewCopterRotPitch, NewCopterRotYaw, NewCopterRotRoll);
	SetActorRotation(NewCopterRotation);

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Forward Speed :  %f"), ForwardSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Right Speed :  %f"), RightSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Up Speed :  %f"), UpSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("YawRot Speed :  %f"), YawRotSpeed));
}

void AHRCopterPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHRCopterPawn::UpdateBladesIfPossible(bool bSetBlurBlade)
{
	if(!bCanChangeBladesToBlurBlades) return;

	if(bSetBlurBlade)
	{
		MainBlades->SetStaticMesh(MainBladesBlurMesh);
		TailBlades->SetStaticMesh(TailBladesBlurMesh);
	}else
	{
		MainBlades->SetStaticMesh(MainBladesDefaultMesh);
		TailBlades->SetStaticMesh(TailBladesDefaultMesh);
	}
	
}

void AHRCopterPawn::MoveForwards(const float Value)
{
	TargetForwardSpeed = ForwardSpeedFactor * Value;
}

void AHRCopterPawn::MoveRight(const float Value)
{
	TargetRightSpeed = RightSpeedFactor * Value;
}

void AHRCopterPawn::MoveUp(const float Value)
{
	TargetUpSpeed = UpSpeedFactor * Value;
}

void AHRCopterPawn::DoYawRotation(const float Value)
{
	TargetYawRotSpeed = YawRotateSpeedFactor * Value;
}

