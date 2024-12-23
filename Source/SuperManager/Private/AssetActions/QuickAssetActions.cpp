#include "AssetActions/QuickAssetActions.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"


void UQuickAssetActions::DuplicateAssets(int32 NumOfDuplicates)
{
	if (NumOfDuplicates <= 0)
	{

		ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enger a VALID number"), true);
		return;
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		for(int32 i = 0; i<NumOfDuplicates;i++)
		{
			const FString SourceAssetPath = SelectedAssetData.ObjectPath.ToString();
			const FString NewDuplicatedAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i+1);
			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewDuplicatedAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
			{
				UEditorAssetLibrary::SaveAsset(NewPathName,false);
				++Counter;
			}
		}
	}
	if (Counter > 0)
	{
		ShowNotifyInfo("Successfully duplicated " + FString::FromInt(Counter) + " files");
		// PrintLog(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"));
	}
}

void UQuickAssetActions::AddPrefixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for(UObject* SelectedObject:SelectedObjects)
	{
		if (!SelectedObject) continue;

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass());
		
		if (!PrefixFound || PrefixFound->IsEmpty()) 
		{
			PrintLog("Failed to find prefix for class " + SelectedObject->GetClass()->GetName());
			continue;
		}

		FString OldName = SelectedObject->GetName();

		if (OldName.StartsWith(*PrefixFound))
		{
			PrintLog(OldName + " alreay has prefix added");
			continue;
		}

		if (SelectedObject->IsA<UMaterialInstanceConstant>()) {
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName;

		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix);

		++Counter;
	}

	if (Counter > 0)
	{
		ShowNotifyInfo("Successfully renamed " + FString::FromInt(Counter) + " assets");
	}
}

void UQuickAssetActions::RemoveUnsuedAssets()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsData;

	FixUpReirectors();

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.ObjectPath.ToString());
		
		if(AssetReferencers.Num() == 0)
		{
			UnusedAssetsData.Add(SelectedAssetData);
		}
	}
	if (UnusedAssetsData.Num() == 0) 
	{
		ShowMsgDialog(EAppMsgType::Ok,TEXT("No unused asset found among selected assets"),false);
		return;
	}
	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData, true);
	if (NumOfAssetsDeleted == 0) return;
	ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted) + TEXT(" unused assets")));
}

void UQuickAssetActions::FixUpReirectors()
{
	// Load module
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

	// Create filter with asset paths
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassPaths.Add(UObjectRedirector::StaticClass()->GetClassPathName());

	// Query for assets in selected paths
	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	if (AssetList.Num() == 0) return;

	// Get paths for each asset
	TArray<FString> ObjectPaths;

	for (const FAssetData& Asset : AssetList)
	{
		ObjectPaths.Add(Asset.GetObjectPathString());
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