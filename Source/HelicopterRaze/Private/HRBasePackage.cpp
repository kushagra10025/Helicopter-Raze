// Fill out your copyright notice in the Description page of Project Settings.


#include "HRBasePackage.h"

// Sets default values
AHRBasePackage::AHRBasePackage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHRBasePackage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHRBasePackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

