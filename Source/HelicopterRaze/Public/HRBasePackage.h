// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HRBasePackage.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UPhysicsConstraintComponent;
class UCableComponent;

UCLASS()
class HELICOPTERRAZE_API AHRBasePackage : public AActor
{
	GENERATED_BODY()
	
public:	
	AHRBasePackage();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Package|SocketNames")
	FName CableStartSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Package|SocketNames")
	FName CableFLConnectSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Package|SocketNames")
	FName CableFRConnectSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Package|SocketNames")
	FName CableRLConnectSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Package|SocketNames")
	FName CableRRConnectSocketName;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Mesh", meta=(AllowPrivateAccess=True))
	TObjectPtr<USceneComponent> ActorRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Mesh", meta=(AllowPrivateAccess=True))
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Mesh", meta=(AllowPrivateAccess=True))
	TObjectPtr<UStaticMeshComponent> ConnectorMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Physics", meta=(AllowPrivateAccess=True))
	TObjectPtr<UPhysicsConstraintComponent> FirstPhyConstraint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Physics", meta=(AllowPrivateAccess=True))
	TObjectPtr<UPhysicsConstraintComponent> SecondPhyConstraint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Rendering", meta=(AllowPrivateAccess=True))
	TObjectPtr<UCableComponent> CableFL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Rendering", meta=(AllowPrivateAccess=True))
	TObjectPtr<UCableComponent> CableFR;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Rendering", meta=(AllowPrivateAccess=True))
	TObjectPtr<UCableComponent> CableRL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Package|Rendering", meta=(AllowPrivateAccess=True))
	TObjectPtr<UCableComponent> CableRR;
public:
	
};
