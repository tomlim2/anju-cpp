// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStyle/SuperManagerStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

FName FSuperManagerStyle::StyleSetName = FName("SuperManagerStyle");
TSharedPtr<FSlateStyleSet> FSuperManagerStyle::CreatedSlateStyleSet = nullptr;

void FSuperManagerStyle::InitializeIcons()
{
	if (!CreatedSlateStyleSet.IsValid()) {
		CreatedSlateStyleSet = CreateSlateStyleSet();
		FSlateStyleRegistry::RegisterSlateStyle(*CreatedSlateStyleSet);
	}
}

void FSuperManagerStyle::ShutDown()
{
	if (CreatedSlateStyleSet.IsValid()) {
	FSlateStyleRegistry::UnRegisterSlateStyle(*CreatedSlateStyleSet);
	CreatedSlateStyleSet.Reset();
	}
}

TSharedRef<FSlateStyleSet> FSuperManagerStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> CustomStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));
	const FString IconDirectory =
	IPluginManager::Get().FindPlugin(TEXT("SuperManager"))->GetBaseDir() / "Resources";

	CustomStyleSet->SetContentRoot(IconDirectory);
	const FVector2D Icon16x16(16.f, 16.f);
	CustomStyleSet->Set("ContentBrowser.Icon_PB_001", new FSlateImageBrush(IconDirectory/"Icon_PB_001.png", Icon16x16));
	CustomStyleSet->Set("ContentBrowser.Icon_PB_002", new FSlateImageBrush(IconDirectory/"Icon_PB_002.png", Icon16x16));
	CustomStyleSet->Set("ContentBrowser.Icon_PB_003", new FSlateImageBrush(IconDirectory/"Icon_PB_003.png", Icon16x16));
	CustomStyleSet->Set("ContentBrowser.Icon_PB_004", new FSlateImageBrush(IconDirectory/"Icon_PB_004.png", Icon16x16));
	return CustomStyleSet;
}
