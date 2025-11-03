// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/VerticalBoxSlot.h"
#include "TrackSliderUI.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChanged, float);

UCLASS(Abstract)
class SIMTRACKS_API UTrackSliderUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetValue(float Value);
	float GetValue();
	void SetMaxValue(float Value);
	void SetupMouseDraggingSteps(float CheckpointStepSize);
	void StepCheckpointForward();
	void StepCheckpointBackward();
	void SetScreenWidth(unsigned int Width);
	FOnMouseCaptureEndEvent& GetOnMouseCaptureEnd() const;

	FOnValueChanged ValueChanged;

private:
	UFUNCTION()
	void StartEnforcingCheckpointSteps();
	UFUNCTION()
	void StopEnforcingCheckpointSteps();
	UFUNCTION()
	void OnTrackSliderChanged(float Value);

	void UpdateCursorBodyPosition() const;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* Slider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* CursorBody;

	float CheckpointStep = 0.f;
	bool bEnforceCheckpointSteps = false;

	UPROPERTY()
	UVerticalBoxSlot* CursorBodySlot = nullptr;
	float DefaultCursorBodyPaddingLeft = 0.f;
	unsigned int ScreenWidth = 0;
};