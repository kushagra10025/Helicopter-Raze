// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HRCopterAnimInstance.generated.h"

class AHRCopterPawn;

/**
 * 
 */
UCLASS()
class HELICOPTERRAZE_API UHRCopterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Copter|References", meta=(AllowPrivateAccess=True))
	TObjectPtr<AHRCopterPawn> CopterPawnRef;
};
