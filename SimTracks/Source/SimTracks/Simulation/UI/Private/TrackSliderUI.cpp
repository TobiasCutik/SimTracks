// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/TrackSliderUI.h"
#include "Components/Image.h"

bool UTrackSliderUI::Initialize()
{
	if (!Super::Initialize())
		return false;

	Slider->OnMouseCaptureBegin.AddDynamic(this, &UTrackSliderUI::StartEnforcingCheckpointSteps);
	Slider->OnMouseCaptureEnd.AddDynamic(this, &UTrackSliderUI::StopEnforcingCheckpointSteps);
	Slider->OnValueChanged.AddDynamic(this, &UTrackSliderUI::OnTrackSliderChanged);

	CursorBodySlot = Cast<UVerticalBoxSlot>(CursorBody->Slot);
	DefaultCursorBodyPaddingLeft = -CursorBody->Brush.ImageSize.X / 2.f;

	return true;
}

void UTrackSliderUI::OnTrackSliderChanged(float Value)
{
	if (!bEnforceCheckpointSteps)
		return;

	const float AmountBeyondCheckpointStep = FMath::Fmod(Value, CheckpointStep);
	const float CorrectedValue = GetValue() - AmountBeyondCheckpointStep;

	SetValue(CorrectedValue);
}

void UTrackSliderUI::StepCheckpointForward()
{
	SetValue(GetValue() + CheckpointStep);
}

void UTrackSliderUI::StepCheckpointBackward()
{
	SetValue(GetValue() - CheckpointStep);
}

void UTrackSliderUI::SetScreenWidth(unsigned int Width)
{
	ScreenWidth = Width;
}

void UTrackSliderUI::SetValue(float Value)
{
	Slider->SetValue(Value);
	ValueChanged.Broadcast(Value);

	UpdateCursorBodyPosition();
}

float UTrackSliderUI::GetValue()
{
	return Slider->GetValue();
}

void UTrackSliderUI::SetMaxValue(float Value)
{
	Slider->SetMaxValue(Value);
}

void UTrackSliderUI::SetupMouseDraggingSteps(float CheckpointStepSize)
{
	CheckpointStep = CheckpointStepSize;
}

void UTrackSliderUI::StartEnforcingCheckpointSteps()
{
	bEnforceCheckpointSteps = true;
}

void UTrackSliderUI::StopEnforcingCheckpointSteps()
{
	bEnforceCheckpointSteps = false;
}

void UTrackSliderUI::UpdateCursorBodyPosition() const
{
	if (!CursorBodySlot)
		return;

	const float TimelinePercentage = Slider->GetValue() / Slider->MaxValue;
	const float SliderPosition = TimelinePercentage * ScreenWidth;

	FMargin Margin = FMargin();
	Margin.Left = SliderPosition + DefaultCursorBodyPaddingLeft;
	CursorBodySlot->SetPadding(Margin);
}

FOnMouseCaptureEndEvent& UTrackSliderUI::GetOnMouseCaptureEnd() const
{
	return Slider->OnMouseCaptureEnd;
}