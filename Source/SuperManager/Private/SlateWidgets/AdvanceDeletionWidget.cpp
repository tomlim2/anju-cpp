// Fill out your copyright notice in the Description page of Project Settings.
#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	StoredAssetsData = InArgs._AssetsDataToStore;
	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;

	ChildSlot
		[   //Main vertical box
			SNew(SVerticalBox)				
				// First vertical slot for title text
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Advance Deletion"))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]
				//Second for dropdown to specify the listing condition
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
				]
				//third for the asset list
				+SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				[
					SNew(SScrollBox)

						+SScrollBox::Slot()
						[
							SNew(SListView<TSharedPtr<FAssetData>>)
								.ItemHeight(24.f)
								.ListItemsSource(&StoredAssetsData)
								.OnGenerateRow(this,&SAdvanceDeletionTab::OnGenerateRowForList)
						]
				]
				//fourth for three buttons
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
				]
		];
}

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!AssetDataToDisplay.IsValid()) return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	const FString DisplayAssetClassName = AssetDataToDisplay->GetClass()->GetName();
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();
	
	FSlateFontInfo AssetClassNameFont = GetEmbossedTextFont();
	AssetClassNameFont.Size = 10;

	FSlateFontInfo AssetNameFont = GetEmbossedTextFont();
	AssetNameFont.Size = 15;
	
	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		.Padding(FMargin(5.f))
		[
			SNew(SHorizontalBox)
				// First slot for check box
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(.05f)
				[
					ConstructCheckBox(AssetDataToDisplay) // need to understand this part
				]
			
				// Second slot for display asset class name
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Fill)
				.FillWidth(.5f)
				[
					ConstructTextForRowWidget(DisplayAssetClassName, AssetClassNameFont)
				]
				// thrid slot for display assetname
				
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Fill)
				[
					ConstructTextForRowWidget(DisplayAssetName, AssetNameFont)
				]
				
			// fourth slot for a button
			+SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Fill)
				[
					ConstructButtonForRowWidget(AssetDataToDisplay)
				]
			//SNew(STextBlock).Text(FText::FromString(DisplayAssetName))
		];
	return ListViewRowWidget;	
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> ConstructedCheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);
		return ConstructedCheckBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch (NewState)
	{
	case ECheckBoxState::Checked:
		DebugHeader::PrintLog(AssetData->AssetName.ToString() + " is unchecked");
		break;
	case ECheckBoxState::Unchecked:
		DebugHeader::PrintLog(AssetData->AssetName.ToString() + " is checked");
		break;
	case ECheckBoxState::Undetermined:	
		DebugHeader::PrintLog(AssetData->AssetName.ToString());
		break;
	default:
		break;
	
	}
	DebugHeader::PrintLog(AssetData->AssetName.ToString());
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse)
{
	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(FontToUse)
		.ColorAndOpacity(FColor::White);

	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SButton> ConstructedButton = SNew(SButton)
		.Text(FText::FromString("Delete"))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteButtonClicked, AssetDataToDisplay);
	return ConstructedButton;
}

FReply SAdvanceDeletionTab::OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	DebugHeader::PrintLog(ClickedAssetData->AssetName.ToString() + " is clicked");
	return FReply::Handled();
}
