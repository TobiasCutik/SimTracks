// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/ButtonUI.h"

FOnButtonClickedEvent& UButtonUI::GetOnClickedEvent() const
{
	return Button->OnClicked;
}