// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HRCopterPawn.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UStaticMesh;

UCLASS()
class HELICOPTERRAZE_API AHRCopterPawn : public APawn
{
	GENERATED_BODY()

public:
	AHRCopterPawn();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Copter|Mesh", meta=(AllowPrivateAccess=True))
	TObjectPtr<USkeletalMeshComponent> CopterBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Copter|Mesh", meta=(AllowPrivateAccess=True))
	TObjectPtr<UStaticMeshComponent> MainBlades;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Copter|Mesh", meta=(AllowPrivateAccess=True))
	TObjectPtr<UStaticMeshComponent> TailBlades;

	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh")
	FName MainBladeAttachSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh")
	FName TailBladeAttachSocketName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Copter|Camera", meta=(AllowPrivateAccess=True))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Copter|Camera", meta=(AllowPrivateAccess=True))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh|Blades")
	TObjectPtr<UStaticMesh> MainBladesDefaultMesh;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh|Blades")
	TObjectPtr<UStaticMesh> TailBladesDefaultMesh;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh|Blades")
	bool bCanChangeBladesToBlurBlades;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh|Blades|Blur", meta=(EditCondition="bCanChangeBladesToBlurBlades"))
	TObjectPtr<UStaticMesh> MainBladesBlurMesh;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Mesh|Blades|Blur", meta=(EditCondition="bCanChangeBladesToBlurBlades"))
	TObjectPtr<UStaticMesh> TailBladesBlurMesh;

	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float ForwardSpeedFactor;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float ForwardSpeedAccel;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float RightSpeedFactor;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float RightSpeedAccel;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float UpSpeedFactor;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float UpSpeedAccel;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
    float YawRotateSpeedFactor;
	UPROPERTY(EditDefaultsOnly, Category="Copter|Movement")
	float YawRotateSpeedAccel;

	float ForwardSpeed;
	float TargetForwardSpeed;
	float RightSpeed;
	float TargetRightSpeed;
	float UpSpeed;
	float TargetUpSpeed;
	float YawRotSpeed;
	float TargetYawRotSpeed;

	FVector DeltaLocation;
	
public:
	UFUNCTION(BlueprintCallable, Category="Copter|Blades")
	void UpdateBladesIfPossible(bool bSetBlurBlade);

	UFUNCTION(BlueprintCallable, Category="Copter|Movement")
	void MoveForwards(const float Value);
	UFUNCTION(BlueprintCallable, Category="Copter|Movement")
	void MoveRight(const float Value);
	UFUNCTION(BlueprintCallable, Category="Copter|Movement")
	void MoveUp(const float Value);
	UFUNCTION(BlueprintCallable, Category="Copter|Movement")
	void DoYawRotation(const float Value);

};
