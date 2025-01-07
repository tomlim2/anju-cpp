// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperManager.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "CustomStyle/SuperManagerStyle.h"
#include "LevelEditor.h"
#include "Engine/Selection.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerModule::StartupModule()
{
	FSuperManagerStyle::InitializeIcons();
	InitCBMenuExtention();
	RegisterAdvanceDeletionTab();
	InitLevelEditorExtention();
	InitCustomSelectionEvent();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

#pragma region ContentBrowserMenuExtention

void FSuperManagerModule::InitCBMenuExtention()
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders =
		ContentBrowserModule.GetAllPathViewContextMenuExtenders(); // Get all menu extenders
	/*
	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate; // Add delegate
	CustomCBMenuDelegate.BindRaw(this,&FSuperManagerModule::CustomCBMenuExtender);

	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate); // Bind the delegate
	*/
	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::
		CreateRaw(this, &FSuperManagerModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FSuperManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());
	if (SelectedPaths.Num()>0) 
	{
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			
			FMenuExtensionDelegate::CreateRaw(this, &FSuperManagerModule::AddCBMenuEntry));
		FolderPathsSelected = SelectedPaths;
	}
	return MenuExtender;
}

void FSuperManagerModule::AddCBMenuEntry(FMenuBuilder & MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.Icon_PB_001"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteUnusedAssetButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Empty Folder")),
		FText::FromString(TEXT("Safely delete all empty Folders")),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.Icon_PB_002"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteEmptyFoldersButtonClicked)
	);
	
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Advance Deletaion")),
		FText::FromString(TEXT("List assets by specific condition in a tab for deleting")),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.Icon_PB_003"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnAdvanceDeletionButtonClicked)
	);
}

void FSuperManagerModule::OnDeleteUnusedAssetButtonClicked()
{
	if (FolderPathsSelected.Num()>1) 
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("You can only do this to one folder"));
		return;
	}
	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);
	if (AssetsPathNames.Num() == 0) 
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No asset found under selected folder"));
		return;
	}

	TArray<FAssetData> UnusedAssetsDataArray;

	for (const FString& AssetPathName : AssetsPathNames)
	{
		//Do not touch root folder
		if(
			AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) || 
			AssetPathName.Contains(TEXT("__ExternalObjects__"))
			)
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		TArray<FString> AssetReferencers = 
		UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);
		if (AssetReferencers.Num() == 0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
			UnusedAssetsDataArray.Add(UnusedAssetData);
		}
	}
	if (UnusedAssetsDataArray.Num() > 0)
	{
		EAppReturnType::Type ConfirmResult =
			DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, TEXT("A total of ") + FString::FromInt(UnusedAssetsDataArray.Num()) + " found.\nWould you like to procceed?");
		if (ConfirmResult == EAppReturnType::No) return;
		FixUpRedirectors();
		ObjectTools::DeleteAssets(UnusedAssetsDataArray);
	}
	else
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found under selected folder"));
	}
}

void FSuperManagerModule::OnDeleteEmptyFoldersButtonClicked()
{
	FixUpRedirectors();
	TArray<FString> FolderPathsArray = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0], true, true);
	uint32 Counter = 0;

	FString EmptyFolderPathsNames;
	TArray<FString> EmptyFoldersPathsArray;

	for (const FString& FolderPath : FolderPathsArray) 
	{
		if (
		FolderPath.Contains(TEXT("Developers")) ||
		FolderPath.Contains(TEXT("Collections")) ||
		FolderPath.Contains(TEXT("__ExternalActors__")) ||
		FolderPath.Contains(TEXT("__ExternalObjects__"))
		)
		{
			continue;
		}
		if (!UEditorAssetLibrary::DoesDirectoryExist(FolderPath)) continue;
		if (!UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath)) 
		{
			EmptyFolderPathsNames.Append(FolderPath);
			EmptyFolderPathsNames.Append(TEXT("\n"));

			EmptyFoldersPathsArray.Add(FolderPath);
		}
	}
	
	if (EmptyFoldersPathsArray.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No empty folder found under selected folder"), false);
		return;
	}
	
	EAppReturnType::Type ConfirmResult = DebugHeader::ShowMsgDialog(EAppMsgType::OkCancel,
			TEXT("Empty folders found in:\n") + EmptyFolderPathsNames + TEXT("\nWould you like to delete all?"), false);
	if (ConfirmResult == EAppReturnType::Cancel) return;
	for (const FString& EmptyFolderPath : EmptyFoldersPathsArray) 
	{
		if (UEditorAssetLibrary::DeleteDirectory(EmptyFolderPath))
		{

			++Counter;
		}
		else
		{
			DebugHeader::PrintLog("Failed to delete " + EmptyFolderPath);
		}
	}
	if (Counter > 0) {
		DebugHeader::ShowNotifyInfo("Successfully deleted " + FString::FromInt(Counter) + TEXT(" folders"));
	}
}

void FSuperManagerModule::OnAdvanceDeletionButtonClicked()
{
	FixUpRedirectors();
	FGlobalTabmanager::Get()->TryInvokeTab(FName("AdvanceDeletion"));
}

void FSuperManagerModule::FixUpRedirectors()
{
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassPaths.Add(UObjectRedirector::StaticClass()->GetClassPathName());

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	if (AssetList.Num() == 0) return;
	
	TArray<FString> ObjectPaths;

	for (const FAssetData& Asset : AssetList)
	{
		ObjectPaths.Add(Asset.PackageName.ToString());
	}

	// Load assets
	TArray<UObject*> Objects;
	bool Result = AssetViewUtils::LoadAssetsIfNeeded(ObjectPaths, Objects, true, true);

	if (Result)
	{
		// Convert objects to object redirectors
		TArray<UObjectRedirector*> Redirectors;
		for (UObject* Object : Objects)
		{
			Redirectors.Add(CastChecked<UObjectRedirector>(Object));
		}

		// Call fix up redirectors from asset tools
		FAssetToolsModule& AssetToolsModule =
			FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

		AssetToolsModule.Get().FixupReferencers(Redirectors);
	}

}

#pragma endregion

#pragma region CustomEditorTab
void FSuperManagerModule::RegisterAdvanceDeletionTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("AdvanceDeletion"),
		FOnSpawnTab::CreateRaw(this, &FSuperManagerModule::OnSpawnAdvanceDeletionTab))
		.SetDisplayName(FText::FromString(TEXT("Advance Deletion")))
		.SetAutoGenerateMenuEntry(false) // do not appear in the tool menu
		.SetIcon(FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.Icon_PB_004"));
}

TSharedRef<SDockTab> FSuperManagerModule::OnSpawnAdvanceDeletionTab(const FSpawnTabArgs &SpawnedTadArgs)
{
	return
		SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SAdvanceDeletionTab)
				.AssetsDataToStore(GetAllAssetDataUnderSelectedFolder())
				.CurrentSelectedFolder(FolderPathsSelected[0])
		];
}

TArray<TSharedPtr<FAssetData>> FSuperManagerModule::GetAllAssetDataUnderSelectedFolder()
{
	TArray<TSharedPtr<FAssetData>> AvailableAssetsData;
	// Check if FolderPathsSelected is valid
	

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);
	for (const FString& AssetPathName : AssetsPathNames)
	{
		//Do not touch root folder
		if (
			AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) ||
			AssetPathName.Contains(TEXT("__ExternalObjects__"))
			)
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;
		const FAssetData Data = UEditorAssetLibrary::FindAssetData(AssetPathName);
		AvailableAssetsData.Add(MakeShared<FAssetData>(Data));
	}

	return AvailableAssetsData;
}

#pragma endregion

#pragma region ProcessDataForAdvanceDeletionTab
bool FSuperManagerModule::DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete)
{
	TArray<FAssetData> AssetDataForDeletion;
	AssetDataForDeletion.Add(AssetDataToDelete);
	if (ObjectTools::DeleteAssets(AssetDataForDeletion) > 0) {
		return true;
	}
	return false;
}
bool FSuperManagerModule::DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete)
{
	if (ObjectTools::DeleteAssets(AssetsToDelete) > 0)
	{
		return true;
	}
	return false;
}

void FSuperManagerModule::ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData)
{
	OutUnusedAssetsData.Empty();
	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(DataSharedPtr->GetObjectPathString());
		if (AssetReferencers.Num()==0)
		{
			OutUnusedAssetsData.Add(DataSharedPtr);
		}
	}
}

void FSuperManagerModule::ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData)
{
	OutSameNameAssetsData.Empty();
	//Multimap
	TMultiMap<FString, TSharedPtr<FAssetData>> AssetsInfoMultiMap;
	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter) 
	{
		AssetsInfoMultiMap.Emplace(DataSharedPtr->AssetName.ToString(), DataSharedPtr);
	}
	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		TArray<TSharedPtr<FAssetData>> OutAssetsData;
		AssetsInfoMultiMap.MultiFind(DataSharedPtr->AssetName.ToString(), OutAssetsData);

		if (OutAssetsData.Num() <= 1) continue;
		
		for (const TSharedPtr<FAssetData>& SameNameData : OutAssetsData) 
		{
			if (SameNameData.IsValid()) 
			{
				OutSameNameAssetsData.AddUnique(SameNameData);
			}
		}
	}
}

void FSuperManagerModule::SyscCBToClickedAssetForAssetList(const FString& AssetPathToSync)
{
	TArray<FString> AssetsPathToSync;
	AssetsPathToSync.Add(AssetPathToSync);
	UEditorAssetLibrary::SyncBrowserToObjects(AssetsPathToSync);
}

#pragma endregion

#pragma region LevelEditorMenuExtension
void FSuperManagerModule::InitLevelEditorExtention()
{
	FLevelEditorModule& LevelEditorModule = 
	FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	
	TArray<FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors>& LevelEditorMenuExtenders = 
	LevelEditorModule.GetAllLevelViewportContextMenuExtenders();

	LevelEditorMenuExtenders.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(this,&FSuperManagerModule::CustomLevelEditorMenuExtender));
}

TSharedRef<FExtender> FSuperManagerModule::CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor *> SelectedActors)
{
	TSharedRef<FExtender>MenuExtender = MakeShareable(new FExtender());
	
	if (SelectedActors.Num() > 0)
	{
		MenuExtender->AddMenuExtension(
			FName("ActorOptions"),
			EExtensionHook::Before,
			UICommandList,
			FMenuExtensionDelegate::CreateRaw(this, &FSuperManagerModule::AddLevelEditorMenuEntry)
		);
	}
    return MenuExtender;
}

void FSuperManagerModule::AddLevelEditorMenuEntry(FMenuBuilder &MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Lock Actor Selection")),
		FText::FromString(TEXT("Prevent actor from being selected")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this,&FSuperManagerModule::OnLockActorSelectionButtonClicked)
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Unlock all actor selection")),
		FText::FromString(TEXT("Remove the selection constraint on all actor")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this,&FSuperManagerModule::OnUnlockActorSelectionButtonClicked)
	);
}

void FSuperManagerModule::OnLockActorSelectionButtonClicked()
{
	DebugHeader::PrintLog("Lock Actor Selection Button Clicked");
}

void FSuperManagerModule::OnUnlockActorSelectionButtonClicked()
{
	DebugHeader::PrintLog("Unlock Actor Selection Button Clicked");
}

#pragma endregion

#pragma region SelectionLock
void FSuperManagerModule::InitCustomSelectionEvent()
{
	USelection* UserSelection = GEditor->GetSelectedActors();
	UserSelection->SelectObjectEvent.AddRaw(this, &FSuperManagerModule::OnActorSelected);
}
void FSuperManagerModule::OnActorSelected(UObject *SelectedObject)
{
	if(AActor* SelectedActor = Cast<AActor>(SelectedObject))
	{
		
		DebugHeader::PrintLog(SelectedActor->GetActorLabel());
	}
}
#pragma endregion

void FSuperManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("AdvanceDeletion"));
	FSuperManagerStyle::ShutDown();
}

#undef LOCTEXT_NAMESPACE

	
IMPLEMENT_MODULE(FSuperManagerModule, SuperManager)