// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HRMainMenuUW.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HELICOPTERRAZE_API UHRMainMenuUW : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* PlayGameBtn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* CustomizeBtn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* AchievementsBtn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* CreditsBtn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* OptionsBtn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenuWidget", meta=(BindWidget))
	UButton* QuitGameBtn;

	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category="MainMenuWidget|Buttons")
	void PlayGameBtnClicked();
	UFUNCTION(BlueprintNativeEvent, Category="MainMenuWidget|Buttons")
	void CustomizeBtnClicked();
	UFUNCTION(BlueprintNativeEvent, Category="MainMenuWidget|Buttons")
	void AchievementsBtnClicked();
	UFUNCTION(BlueprintNativeEvent, Category="MainMenuWidget|Buttons")
	void CreditsBtnClicked();
	UFUNCTION(BlueprintNativeEvent, Category="MainMenuWidget|Buttons")
	void OptionsBtnClicked();
	UFUNCTION(BlueprintNativeEvent, Category="MainMenuWidget|Buttons")
	void QuitGameBtnClicked();

};
