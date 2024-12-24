// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	StoredAssetsData = InArgs._AssetsDataToStore;
	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
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
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();
	TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
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
				// thrid slot for display assetname
				+SHorizontalBox::Slot()
				[
				SNew(STextBlock)
					.Text(FText::FromString(DisplayAssetName))
				]
			// fourth slot for a button
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