// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUICommand/SuperManagerUICommands.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerUICommands::RegisterCommands()
{
	UI_COMMAND(
		LockActorSelection,
		"Lock Actor Selection",
		"Lock actor selection in level, once triggered, the actor will be unselectatble in level editor",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::W, EModifierKey::Alt)
	);

	UI_COMMAND(
		UnlockActorSelection,
		"Unlock All Actor",
		"Unlock all actor in level, once triggered, all actor will be selectable in level editor",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::W, EModifierKey::Alt | EModifierKey::Shift)
	);
}

#undef LOCTEXT_NAMESPACE