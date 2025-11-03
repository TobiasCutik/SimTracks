// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/Utils/Public/TextUtils.h"

FText UTextUtils::AsEnNumber(const float Value)
{
	return FText::AsNumber(Value, nullptr, FInternationalization::Get().GetCulture("en-US"));
}

FText UTextUtils::AsEnNumber(const double Value)
{
	return FText::AsNumber(Value, nullptr, FInternationalization::Get().GetCulture("en-US"));
}
