// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/SimTrackUI.h"
#include "Kismet/KismetMathLibrary.h"
#include "SimTracks/Simulation/Utils/Public/TextUtils.h"

bool USimTrackUI::Initialize()
{
	if (!Super::Initialize())
		return false;

	EvaluationValueSlot = Cast<UHorizontalBoxSlot>(EvaluationValue->Slot);
	return true;
}

void USimTrackUI::SetWidth(float Width)
{
	SizeBox->SetWidthOverride(Width);
}

float USimTrackUI::GetWidth() const
{
	return SizeBox->WidthOverride;
}

void USimTrackUI::SetName(FText Name)
{
	TrackName->SetText(Name);
}

float USimTrackUI::GetHeight() const
{
	return SizeBox->HeightOverride;
}

void USimTrackUI::SetActiveVisually(bool bActive)
{
	if (bActiveVisually == bActive)
		return;

	bActiveVisually = bActive;

	if (bActiveVisually)
		Button->SetBackgroundColor(ActiveBackgroundColor);
	else
		Button->SetBackgroundColor(DefaultBackgroundColor);
}

void USimTrackUI::AddAnalysisPatch(float SimulationTime, float PerformanceValue, float ColorValue, float WidthInPixel)
{
	for (const auto Patch : AnalysisPatches)
	{
		if (Patch->WasCreatedAtTime(SimulationTime))
			return;
	}

	const auto AnalysisPatch = CreateWidget<UAnalysisPatchUI>(this, AnalysisPatchUI);
	const FVector2D DesiredSize = FVector2D(WidthInPixel, GetHeight());
	const auto Color = UKismetMathLibrary::LinearColorLerp(WorstPerformanceColor, BestPerformanceColor, ColorValue);
	AnalysisPatch->SetColorAndOpacity(Color);

	AnalysisBox->AddChild(AnalysisPatch);

	AnalysisPatch->SetDesiredSize(DesiredSize);
	AnalysisPatch->SimulationTime = SimulationTime;
	AnalysisPatch->EvaluationParamValue = PerformanceValue;
	AnalysisPatches.Add(AnalysisPatch);
}

void USimTrackUI::UpdateAnalysisPatchSizes(float WidthInPixel)
{
	const FVector2D DesiredSize = FVector2D(WidthInPixel, GetHeight());

	for (const auto& AnalysisPatch : AnalysisPatches)
		AnalysisPatch->SetDesiredSize(DesiredSize);
}

void USimTrackUI::UpdateAnalysisPatchValues(float SimulationTime, float PerformanceValue, float ColorValue)
{
	for (const auto Patch : AnalysisPatches)
	{
		if (Patch->WasCreatedAtTime(SimulationTime))
		{
			const auto NewColor = UKismetMathLibrary::LinearColorLerp(WorstPerformanceColor, BestPerformanceColor, ColorValue);
			Patch->SetColorAndOpacity(NewColor);

			Patch->EvaluationParamValue = PerformanceValue;
			EvaluationValue->SetText(UTextUtils::AsEnNumber(PerformanceValue));

			break;
		}
	}
}

void USimTrackUI::ShowEvaluationValueAt(float SimulationTime, float TimelineLengthInSeconds, unsigned int ScreenWidth)
{
	for (const auto Patch : AnalysisPatches)
	{
		if (Patch->WasCreatedAtTime(SimulationTime))
		{
			EvaluationValue->SetText(UTextUtils::AsEnNumber(Patch->EvaluationParamValue));

			if (!EvaluationValueSlot)
				return;

			const float LocalSimTrackTime = SimulationTime - AnalysisPatches[0]->SimulationTime;
			const float SimTrackPixelWidthBeforeTime = LocalSimTrackTime / TimelineLengthInSeconds * ScreenWidth;

			FMargin Margin = FMargin();
			Margin.Right = GetWidth() - SimTrackPixelWidthBeforeTime;
			EvaluationValueSlot->SetPadding(Margin);

			break;
		}
	}
}