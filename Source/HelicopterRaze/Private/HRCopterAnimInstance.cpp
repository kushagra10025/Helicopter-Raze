// Fill out your copyright notice in the Description page of Project Settings.


#include "HRCopterAnimInstance.h"

#include "HRCopterPawn.h"

void UHRCopterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CopterPawnRef = Cast<AHRCopterPawn>(TryGetPawnOwner());
}

void UHRCopterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!CopterPawnRef) return;

	MainRotorAngle = FMath::Fmod(MainRotorAngle + (CopterPawnRef->GetMainRotorSpeed() * DeltaSeconds), 360.f);
	TailRotorAngle = FMath::Fmod(TailRotorAngle + (CopterPawnRef->GetTailRotorSpeed() * DeltaSeconds), 360.f);
}
