// Fill out your copyright notice in the Description page of Project Settings.
#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"
#include "SuperManager.h"

#define ListAll TEXT("List All Available Assets")
#define ListUnused TEXT("List Unsued Assets")
#define ListSameName TEXT("List Assets With Same Name")

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	StoredAssetsData = InArgs._AssetsDataToStore;
	DisplayedAssetsData = StoredAssetsData;

	CheckBoxesArray.Empty();
	AssetsDataToDeleteArray.Empty();
	ComboBoxSourceItems.Empty();

	ComboBoxSourceItems.Add(MakeShared<FString>(ListAll));
	ComboBoxSourceItems.Add(MakeShared<FString>(ListUnused));
	ComboBoxSourceItems.Add(MakeShared<FString>(ListSameName));

	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;

	ChildSlot
		[   //Main vertical box
			SNew(SVerticalBox)				
				// First vertical slot for title text
				
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Advance Deletion")))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]
				
				//Second for dropdown to specify the listing condition
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					//Combox Box Slot
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							ConstructComboBox()
						]
						//Help text for combo box
						+SHorizontalBox::Slot()
						.FillWidth(.6f)
						[
							ConstructComboHelpTexts(TEXT("Specify the listing condition in the drop down. Left mouse click to go to where asset is located"),
								ETextJustify::Center)
						]
						+ SHorizontalBox::Slot()
						.FillWidth(.1f)
						[
							ConstructComboHelpTexts(TEXT("Current Folder:\n") + InArgs._CurrentSelectedFolder,
								ETextJustify::Right)
						]

				]
				//Third slot for the asset list
				+SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				[
					SNew(SScrollBox)
					+SScrollBox::Slot()
						[
							ConstructAssetListView()
						]
				]
				//fourth for three buttons
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

						//Button1 slot
						+SHorizontalBox::Slot()
						.FillWidth(10.f)
						.Padding(5.f)
						[
							ConstructDeleteAllButton()
						]
						//Button2 slot
						+ SHorizontalBox::Slot()
						.FillWidth(10.f)
						.Padding(5.f)
						[
							ConstructSelectAllButton()
						]
						//Button3 slot
						+ SHorizontalBox::Slot()
						.FillWidth(10.f)
						.Padding(5.f)
						[
							ConstructDeselectAllButton()
						]
				]
		];
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetListView()
{
	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
		.ItemHeight(26.f)
		.ListItemsSource(&DisplayedAssetsData)
		.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList)
		.OnMouseButtonClick(this,&SAdvanceDeletionTab::OnRowWidgetMouseButtonClicked)
		;
	return ConstructedAssetListView.ToSharedRef();
}

void SAdvanceDeletionTab::RefreshAssetListView()
{
	AssetsDataToDeleteArray.Empty();
	CheckBoxesArray.Empty();
	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}
#pragma region ComboBoxForListingCondition

TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvanceDeletionTab::ConstructComboBox()
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedComboBox =
		SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&ComboBoxSourceItems)
		.OnGenerateWidget(this, &SAdvanceDeletionTab::OnGenerateComboContent)
		.OnSelectionChanged(this, &SAdvanceDeletionTab::OnComboSelectionChanged)
		[
			SAssignNew(ComboDisplayTextBlock, STextBlock)
				.Text(FText::FromString(TEXT("List Assets Option")))
		]
		;
	return ConstructedComboBox;
}

TSharedRef<SWidget> SAdvanceDeletionTab::OnGenerateComboContent(TSharedPtr<FString> SourceItem)
{
	TSharedRef<STextBlock> ConstructedComboText = SNew(STextBlock)
		.Text(FText::FromString(*SourceItem.Get()));
	return ConstructedComboText;
}

void SAdvanceDeletionTab::OnComboSelectionChanged(
	TSharedPtr<FString> SelectedOption, 
	ESelectInfo::Type InSelectInfo
)
{
	DebugHeader::PrintLog(*SelectedOption.Get());
	ComboDisplayTextBlock->SetText(FText::FromString(*SelectedOption.Get()));

	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	if(*SelectedOption.Get() == ListAll)
	{
		//List all stored asset data
		DisplayedAssetsData = StoredAssetsData;
		RefreshAssetListView();
	}
	else if (*SelectedOption.Get() == ListUnused)
	{
		//List all unused assets
		SuperManagerModule.ListUnusedAssetsForAssetList(StoredAssetsData, DisplayedAssetsData);
		//DisplayedAssetsData = StoredAssetsData;
		RefreshAssetListView();
	}
	else if (*SelectedOption.Get() == ListSameName)
	{
		SuperManagerModule.ListSameNameAssetsForAssetList(StoredAssetsData, DisplayedAssetsData);
		RefreshAssetListView();
	}

	//Pass data for our module to filter

}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructComboHelpTexts(const FString& TextContent, ETextJustify::Type TextJustify)
{
	TSharedRef<STextBlock> ConstructedHelpText = 
		SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Justification(TextJustify)
		.AutoWrapText(true);

	return ConstructedHelpText;
}

#pragma endregion

#pragma region RowWidgetForAssetListView
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

void SAdvanceDeletionTab::OnRowWidgetMouseButtonClicked(TSharedPtr<FAssetData> ClickedData)
{
	FSuperManagerModule& SuperManagerModule =
		FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	SuperManagerModule.SyscCBToClickedAssetForAssetList(ClickedData->PackageName.ToString());
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> ConstructedCheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);

	CheckBoxesArray.Add(ConstructedCheckBox);
	return ConstructedCheckBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch (NewState)
	{
	case ECheckBoxState::Checked:
		AssetsDataToDeleteArray.AddUnique(AssetData);
		break;
	case ECheckBoxState::Unchecked:
		if (AssetsDataToDeleteArray.Contains(AssetData)) {
			AssetsDataToDeleteArray.Remove(AssetData);
		}
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
	FSuperManagerModule& SuperManagerModule = 
		FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));
	const bool bAssetDeleted = SuperManagerModule.DeleteSingleAssetForAssetList(*ClickedAssetData.Get());
	if (bAssetDeleted) {
		//Updating the list source items
		if (StoredAssetsData.Contains(ClickedAssetData))
		{
			StoredAssetsData.Remove(ClickedAssetData);
		}
		if (DisplayedAssetsData.Contains(ClickedAssetData))
		{
			DisplayedAssetsData.Contains(ClickedAssetData);
		}

		//Refresh the list view
		RefreshAssetListView();
	}
	return FReply::Handled();
}

#pragma endregion

#pragma region TabButtons
TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeleteAllButton()
{
	TSharedRef<SButton> DeleteAllButton = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteAllButtonClicked)
		;
	DeleteAllButton->SetContent(ConstructTextForTabButtons(TEXT("Delete Selected")));
	return DeleteAllButton;
}

FReply SAdvanceDeletionTab::OnDeleteAllButtonClicked()
{
	if (AssetsDataToDeleteArray.Num() == 0) {
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No asset currently selected"));
		return FReply::Handled();
	}

	TArray<FAssetData> AssetDataToDelete;
	for (const TSharedPtr<FAssetData>& Data : AssetsDataToDeleteArray) {
		AssetDataToDelete.Add(*Data.Get());
	}
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("superManager"));

	const bool bAssetsDeleted = SuperManagerModule.DeleteMultipleAssetsForAssetList(AssetDataToDelete);

	if (bAssetsDeleted)
	{
		for (const TSharedPtr<FAssetData>& DeletedData : AssetsDataToDeleteArray)
		{
			if (StoredAssetsData.Contains(DeletedData))
			{
				StoredAssetsData.Remove(DeletedData);
			}
			if (DisplayedAssetsData.Contains(DeletedData))
			{
				DisplayedAssetsData.Remove(DeletedData);
			}
		}

		RefreshAssetListView();
	}
	return FReply::Handled();
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructSelectAllButton()
{
	TSharedRef<SButton> SelectAsllButton = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnSelectAllButtonClicked)
		;
	SelectAsllButton->SetContent(ConstructTextForTabButtons(TEXT("Select All")));
	return SelectAsllButton;
}

FReply SAdvanceDeletionTab::OnSelectAllButtonClicked()
{
	if (CheckBoxesArray.Num() == 0) return FReply::Handled();

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray) 
	{
		if (!CheckBox->IsChecked()) 
		{
			CheckBox->ToggleCheckedState();
		}
	}
	return FReply::Handled();
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeselectAllButton()
{
	TSharedRef<SButton> DeselectAsllButton = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeselectAllButtonClicked)
		;
	DeselectAsllButton->SetContent(ConstructTextForTabButtons(TEXT("Deselect All")));
	return DeselectAsllButton;
}

FReply SAdvanceDeletionTab::OnDeselectAllButtonClicked()
{
	if (CheckBoxesArray.Num() == 0) return FReply::Handled();
	
	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray) 
	{
		if (CheckBox->IsChecked())
		{
			CheckBox->ToggleCheckedState();
		}
	}
	
	return FReply::Handled();
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForTabButtons(const FString& TextContent)
{
	FSlateFontInfo ButtonTextFont = GetEmbossedTextFont();
	ButtonTextFont.Size = 15;

	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(ButtonTextFont)
		.Justification(ETextJustify::Center)
		;
	return ConstructedTextBlock;
}

#pragma endregion