// Fill out your copyright notice in the Description page of Project Settings.


#include "HRBasePackage.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CableComponent/Classes/CableComponent.h"


AHRBasePackage::AHRBasePackage():
	CableStartSocketName("CableAttachPoint"),
	CableFLConnectSocketName("ConnectPoint_FL"),
	CableFRConnectSocketName("ConnectPoint_FR"),
	CableRLConnectSocketName("ConnectPoint_RL"),
	CableRRConnectSocketName("ConnectPoint_RR")
{
 	PrimaryActorTick.bCanEverTick = true;

	ActorRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(ActorRootComponent);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(ActorRootComponent);
	
	ConnectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectorMesh"));
	ConnectorMesh->SetupAttachment(ActorRootComponent);
	ConnectorMesh->SetSimulatePhysics(true);
	ConnectorMesh->SetCollisionProfileName(FName("PhysicsActor"));

	// Added only Two Physics Constraint By Default to add more add through Blueprints
	FirstPhyConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FirstPhyConstraint"));
	FirstPhyConstraint->SetupAttachment(ActorRootComponent);
	FirstPhyConstraint->ComponentName1.ComponentName = FName("BaseMesh");
	FirstPhyConstraint->ComponentName2.ComponentName = FName("ConnectorMesh");
	FirstPhyConstraint->SetDisableCollision(true);
	
	SecondPhyConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("SecondPhyConstraint"));
	SecondPhyConstraint->SetupAttachment(ActorRootComponent);
	SecondPhyConstraint->ComponentName1.ComponentName = FName("BaseMesh");
	SecondPhyConstraint->ComponentName2.ComponentName = FName("ConnectorMesh");
	SecondPhyConstraint->SetDisableCollision(true);

	CableFL = CreateDefaultSubobject<UCableComponent>(TEXT("CableFL"));
	CableFL->SetupAttachment(BaseMesh, CableStartSocketName);
	CableFR = CreateDefaultSubobject<UCableComponent>(TEXT("CableFR"));
	CableFR->SetupAttachment(BaseMesh, CableStartSocketName);
	CableRL = CreateDefaultSubobject<UCableComponent>(TEXT("CableRL"));
	CableRL->SetupAttachment(BaseMesh, CableStartSocketName);
	CableRR = CreateDefaultSubobject<UCableComponent>(TEXT("CableRR"));
	CableRR->SetupAttachment(BaseMesh, CableStartSocketName);

	CableFL->AttachEndTo.ComponentProperty = FName("ConnectorMesh");
	CableFL->AttachEndToSocketName = CableFLConnectSocketName;
	CableFL->EndLocation = FVector(0.f, 0.f, 0.f);
	
	CableFR->AttachEndTo.ComponentProperty = FName("ConnectorMesh");
	CableFR->AttachEndToSocketName = CableFRConnectSocketName;
	CableFR->EndLocation = FVector(0.f, 0.f, 0.f);
	
	CableRL->AttachEndTo.ComponentProperty = FName("ConnectorMesh");
	CableRL->AttachEndToSocketName = CableRLConnectSocketName;
	CableRL->EndLocation = FVector(0.f, 0.f, 0.f);
	
	CableRR->AttachEndTo.ComponentProperty = FName("ConnectorMesh");
	CableRR->AttachEndToSocketName = CableRRConnectSocketName;
	CableRR->EndLocation = FVector(0.f, 0.f, 0.f);
	
}

void AHRBasePackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHRBasePackage::BeginPlay()
{
	Super::BeginPlay();
	
}
