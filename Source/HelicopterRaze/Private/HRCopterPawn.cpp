// Fill out your copyright notice in the Description page of Project Settings.


#include "HRCopterPawn.h"
#include "HRPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	EngineStartTime(10.f),
	EngineStartThreshold(0.5f),
	EngineStopThreshold(0.2f),
	MaxMainRotorBaseSpeed(400.f),
	MaxTailRotorBaseSpeed(500.f),
	CalibrationDistanceFromGround(90.f),
	GroundDetectionThreshold(2.f),
	MaxHeightCheckDistance(500.f),
	MainRotorBaseSpeed(0.f),
	TailRotorBaseSpeed(0.f)
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

	bIsGrounded = false;
	bCanStopEngine = false;
	
	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation() + (-1.f * MaxHeightCheckDistance * GetActorUpVector());
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::Green, FString::Printf(TEXT("Hitting Something")));
		GEngine->AddOnScreenDebugMessage(2, DeltaTime, FColor::Green, FString::Printf(TEXT("Distance : %f"), HitResult.Distance - CalibrationDistanceFromGround));
		if((HitResult.Distance - CalibrationDistanceFromGround) <= GroundDetectionThreshold)
		{
			bIsGrounded = true;
			bCanStopEngine = true;
			TargetForwardSpeed = 0.f;
			TargetRightSpeed = 0.f;
			TargetYawRotSpeed = 0.f;
		}
	}else
	{
		GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::Red, FString::Printf(TEXT("Not Hitting Anything")));
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Black);
	
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

	const FVector Velocity(DeltaLocation.X / DeltaTime, DeltaLocation.Y / DeltaTime, DeltaLocation.Z / DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Forward Speed :  %f"), ForwardSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Right Speed :  %f"), RightSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Up Speed :  %f"), UpSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("YawRot Speed :  %f"), YawRotSpeed));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString::Printf(TEXT("Velocity :  %s"), *Velocity.ToString()));
	GEngine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Cyan, FString::Printf(TEXT("Engine On :  %s"), (bEngineStart ? *FString("True") : *FString("False"))));
	GEngine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Cyan, FString::Printf(TEXT("Grounded :  %s"), (bIsGrounded ? *FString("True") : *FString("False"))));

	MainRotorSpeed = FMath::Abs(ForwardSpeed) + FMath::Max(UpSpeed, 0.f) + FMath::Abs(RightSpeed) + MainRotorBaseSpeed;
	TailRotorSpeed = YawRotSpeed + TailRotorBaseSpeed;
}

void AHRCopterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement Controls
		// Pitch Forward and Backwards
		EIC->BindAction(MoveForwardsAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::MoveForwards);
		EIC->BindAction(MoveForwardsAction, ETriggerEvent::Completed, this, &AHRCopterPawn::MoveForwards);
		// Steer Left and Right
		EIC->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::MoveRight);
		EIC->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AHRCopterPawn::MoveRight);
		// Lift Up and Down
		EIC->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::MoveUp);
		EIC->BindAction(MoveUpAction, ETriggerEvent::Completed, this, &AHRCopterPawn::MoveUp);
		// Rotate Left and Right on Yaw Axis
		EIC->BindAction(YawRotateAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::DoYawRotation);
		EIC->BindAction(YawRotateAction, ETriggerEvent::Completed, this, &AHRCopterPawn::DoYawRotation);

		// Camera Controls
		EIC->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::LookRight);
		EIC->BindAction(LookRightAction, ETriggerEvent::Completed, this, &AHRCopterPawn::LookRight);
		EIC->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::LookUp);
		EIC->BindAction(LookUpAction, ETriggerEvent::Completed, this, &AHRCopterPawn::LookUp);
		
		// Engine Controls
		EIC->BindAction(EngineStartStopAction, ETriggerEvent::Triggered, this, &AHRCopterPawn::EngineStartStop);
	}
}

void AHRCopterPawn::BeginPlay()
{
	Super::BeginPlay();

	if(AHRPlayerController* PC = Cast<AHRPlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InGameMappingContext,0);
		}
	}
}

void AHRCopterPawn::OnEngineStartStop()
{
	if(bEngineStart)
	{
		// Begin Engine Start
		// TODO Do Null Check for PlayerCameraManager and Subclass of EngineStartStopCameraShake
		UCameraShakeBase* EngineStartStopCamShakeInstance = UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraShake(EngineStartStopCameraShakeClass);
		MainRotorBaseSpeed = FMath::FInterpTo(MainRotorBaseSpeed, MaxMainRotorBaseSpeed, GetWorld()->GetDeltaSeconds(), EngineStartTime);
		TailRotorBaseSpeed = FMath::FInterpTo(TailRotorBaseSpeed, MaxTailRotorBaseSpeed, GetWorld()->GetDeltaSeconds(), EngineStartTime);
		if(MainRotorBaseSpeed >= (MaxMainRotorBaseSpeed - EngineStartThreshold) && TailRotorBaseSpeed >= (MaxTailRotorBaseSpeed - EngineStartThreshold))
		{
			// Engine Has Started
			UGameplayStatics::GetPlayerCameraManager(this, 0)->StopCameraShake(EngineStartStopCamShakeInstance);
			bIsEngineOn = true;
			GetWorldTimerManager().ClearTimer(THEngineStartStop);
		}
	}else
	{
		if(!bCanStopEngine) return;
		// Begin Engine Stop
		UCameraShakeBase* EngineStartStopCamShakeInstance = UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraShake(EngineStartStopCameraShakeClass);
		bIsEngineOn = false;
		TargetForwardSpeed = 0.f;
		TargetRightSpeed = 0.f;
		TargetUpSpeed = 0.f;
		TargetYawRotSpeed = 0.f;
		
		MainRotorBaseSpeed = FMath::FInterpTo(MainRotorBaseSpeed, 0.f, GetWorld()->GetDeltaSeconds(), EngineStartTime);
		TailRotorBaseSpeed = FMath::FInterpTo(TailRotorBaseSpeed, 0.f, GetWorld()->GetDeltaSeconds(), EngineStartTime);
		if(MainRotorBaseSpeed <= (EngineStopThreshold) && TailRotorBaseSpeed <= (EngineStopThreshold))
		{
			// Engine Has Stopped
			UGameplayStatics::GetPlayerCameraManager(this, 0)->StopCameraShake(EngineStartStopCamShakeInstance);
			GetWorldTimerManager().ClearTimer(THEngineStartStop);
		}
	}
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

void AHRCopterPawn::MoveForwards(const FInputActionValue& Value)
{
	if(!bIsEngineOn || bIsGrounded) return;
	TargetForwardSpeed = ForwardSpeedFactor * Value.Get<float>();
}

void AHRCopterPawn::MoveRight(const FInputActionValue& Value)
{
	if(!bIsEngineOn || bIsGrounded) return;
	TargetRightSpeed = RightSpeedFactor * Value.Get<float>();
}

void AHRCopterPawn::MoveUp(const FInputActionValue& Value)
{
	if(!bIsEngineOn) return;
	TargetUpSpeed = UpSpeedFactor * Value.Get<float>();
}

void AHRCopterPawn::DoYawRotation(const FInputActionValue& Value)
{
	if(!bIsEngineOn || bIsGrounded) return;
	TargetYawRotSpeed = YawRotateSpeedFactor * Value.Get<float>();
}

void AHRCopterPawn::EngineStartStop()
{
	// TODO Fix Bug where the Engine Stops after reaching the ground Threshold if Engine Stop is initiated earlier
	// If Not able to Fix, Display UI -> Safety Measure Enabled (not a error, a feature :))
	bEngineStart = !bEngineStart;
	GetWorldTimerManager().SetTimer(THEngineStartStop, this, &AHRCopterPawn::OnEngineStartStop, 0.05f, true);
}

void AHRCopterPawn::LookRight(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void AHRCopterPawn::LookUp(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}

