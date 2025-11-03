// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/Input/Public/SimTracksController.h"

void ASimTracksController::SetupInput(USimTracksUI* SimTracksWidget)
{
	if (!SimTracksWidget)
		return;

	InputComponent->BindAction("Play_Pause", IE_Pressed, SimTracksWidget, &USimTracksUI::Play_PauseSimulation);
	InputComponent->BindAction("Delete", IE_Pressed, SimTracksWidget, &USimTracksUI::DeleteActiveTrack);
}