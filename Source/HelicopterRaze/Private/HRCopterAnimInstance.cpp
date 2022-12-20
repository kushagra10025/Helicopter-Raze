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
}
