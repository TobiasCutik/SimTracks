// Copyright (c) 2025 Tobias Cutik

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ButtonUI.generated.h"

UCLASS(Abstract)
class SIMTRACKS_API UButtonUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnButtonClickedEvent& GetOnClickedEvent() const;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button;
};