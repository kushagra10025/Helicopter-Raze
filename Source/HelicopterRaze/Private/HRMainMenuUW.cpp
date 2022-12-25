// Fill out your copyright notice in the Description page of Project Settings.


#include "HRMainMenuUW.h"

#include "Components/Button.h"

void UHRMainMenuUW::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(PlayGameBtn)
		PlayGameBtn->OnClicked.AddDynamic(this, &UHRMainMenuUW::PlayGameBtnClicked);
	if(CustomizeBtn)
		CustomizeBtn->OnClicked.AddDynamic(this, &UHRMainMenuUW::CustomizeBtnClicked);
	if(AchievementsBtn)
		AchievementsBtn->OnClicked.AddDynamic(this, &UHRMainMenuUW::AchievementsBtnClicked);
	if(CreditsBtn)
		CreditsBtn->OnClicked.AddDynamic(this, &UHRMainMenuUW::CreditsBtnClicked);
	if(OptionsBtn)
		OptionsBtn->OnClicked.AddDynamic(this, &UHRMainMenuUW::OptionsBtnClicked);
	if(QuitGameBtn)
		QuitGameBtn->OnClicked.AddDynamic(this, &UHRMainMenuUW::QuitGameBtnClicked);
}


void UHRMainMenuUW::PlayGameBtnClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString("Play Button Clicked"));
}

void UHRMainMenuUW::CustomizeBtnClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString("Customize Button Clicked"));
}

void UHRMainMenuUW::AchievementsBtnClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString("Achievements Button Clicked"));
}

void UHRMainMenuUW::CreditsBtnClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString("Credits Button Clicked"));
}

void UHRMainMenuUW::OptionsBtnClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString("Options Button Clicked"));
}

void UHRMainMenuUW::QuitGameBtnClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Magenta, FString("Quit Game Button Clicked"));
}