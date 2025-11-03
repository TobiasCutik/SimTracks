// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/SimTrackElementManager.h"

USimTrackElement* USimTrackElementManager::AddNewElement(UWidget* Owner, TSubclassOf<USimTrackUI> SimTrackUIClass, TSubclassOf<UTrackArrowUI> TrackArrowUIClass, 
	TSubclassOf<UHistoryTrackUI> HistoryTrackUIClass, unsigned int Id, const FString& Name)
{
	const int Index = SimTrackElements.Add(NewObject<USimTrackElement>(Owner, USimTrackElement::StaticClass()));
	const auto NewElement = SimTrackElements[Index];

	NewElement->CreateUI(Owner, SimTrackUIClass, TrackArrowUIClass, HistoryTrackUIClass, Id, Name);
	ActiveSimTrackElement = NewElement;

	return NewElement;
}

USimTrackElement* USimTrackElementManager::GetElement(unsigned int Id) const
{
	for (const auto ArrowSimTrackPair : SimTrackElements)
	{
		if (ArrowSimTrackPair->HasId(Id))
			return ArrowSimTrackPair;
	}

	return nullptr;
}

void USimTrackElementManager::AddAnalysisPatchToActiveTrack(float SimulationTime, float PerformanceValue, float ColorValue, float CheckpointStepSize, float TimelineLengthInSeconds, unsigned int ScreenWidth) const
{
	if (!ActiveSimTrackElement)
		return;

	const float CheckpointStepPercentage = CheckpointStepSize / TimelineLengthInSeconds;
	const float WidthInPixel = CheckpointStepPercentage * ScreenWidth;

	ActiveSimTrackElement->GetSimTrackWidget()->AddAnalysisPatch(SimulationTime, PerformanceValue, ColorValue, WidthInPixel);
}

void USimTrackElementManager::UpdateActiveSimTrackWidgetWidth(float SimulationTime, float TimelineLengthInSeconds, unsigned int ScreenWidth) const
{
	if (!ActiveSimTrackElement)
		return;

	const float TimelinePercentage = SimulationTime / TimelineLengthInSeconds;
	const float SimTrackEndPosition = TimelinePercentage * ScreenWidth;
	const float SimTrackWidth = SimTrackEndPosition - ActiveSimTrackElement->GetSimTrackWidget()->LeftPadding;

	const auto ActiveSimTrackWidget = ActiveSimTrackElement->GetSimTrackWidget();

	if (SimTrackWidth >= ActiveSimTrackWidget->GetWidth())
		ActiveSimTrackWidget->SetWidth(SimTrackWidth);
}

void USimTrackElementManager::SetAllSimTracksVisuallyInactive()
{
	for (const auto SimTrackElement : SimTrackElements)
		SimTrackElement->GetSimTrackWidget()->SetActiveVisually(false);
}

void USimTrackElementManager::SetActiveSimTrackElement(unsigned int Id)
{
	for (const auto SimTrackElement : SimTrackElements)
	{
		if (SimTrackElement->HasId(Id))
		{
			SimTrackElement->GetSimTrackWidget()->SetActiveVisually(true);
			ActiveSimTrackElement = SimTrackElement;
			break;
		}
	}
}

void USimTrackElementManager::SetSimTracksVisuallyActive(TArray<unsigned int> Ids)
{
	for (const auto SimTrackElement : SimTrackElements)
	{
		for (const auto Id : Ids)
		{
			if (SimTrackElement->HasId(Id))
			{
				SimTrackElement->GetSimTrackWidget()->SetActiveVisually(true);
				break;
			}
		}
	}
}

void USimTrackElementManager::UpdateAnalysisPatchSizes(float CheckpointStepSize, float TimelineLengthInSeconds, unsigned int ScreenWidth)
{
	const float CheckpointStepPercentage = CheckpointStepSize / TimelineLengthInSeconds;
	const float WidthInPixel = CheckpointStepPercentage * ScreenWidth;

	for (const auto SimTrackElement : SimTrackElements)
		SimTrackElement->GetSimTrackWidget()->UpdateAnalysisPatchSizes(WidthInPixel);
}

void USimTrackElementManager::DeleteActiveSimTrackElement()
{
	if (!ActiveSimTrackElement)
		return;

	SimTrackElements.Remove(ActiveSimTrackElement);
}

void USimTrackElementManager::ShowEvaluationValuesAt(float SimulationTime, float TimelineLengthInSeconds, unsigned int ScreenWidth)
{
	for (const auto SimTrackElement : SimTrackElements)
		SimTrackElement->GetSimTrackWidget()->ShowEvaluationValueAt(SimulationTime, TimelineLengthInSeconds, ScreenWidth);
}