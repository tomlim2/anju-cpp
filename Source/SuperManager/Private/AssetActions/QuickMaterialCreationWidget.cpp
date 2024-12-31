// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "AssetToolsModule.h"
#include "Factories/MaterialFactoryNew.h"

#pragma region QuickMaterialCreationCore
void UQuickMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	if (bCustomMaterialName) {
		if (MaterialName.IsEmpty() || MaterialName.Equals(TEXT("M_")))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("Please enter a valid name"));
			return;
		}
	}
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<UTexture2D*> SelectedTexturesArray;
	FString SelectedTextureFolderPath;
	uint32 PinsConnectedCounter = 0;

	if (!ProcessSelectedData(SelectedAssetsData, SelectedTexturesArray, SelectedTextureFolderPath)) return;

	if (CheckIsNameUsed(SelectedTextureFolderPath, MaterialName)) return;

	UMaterial* CreatedMaterial = CreateMaterialAsset(MaterialName, SelectedTextureFolderPath);

	if (!CreatedMaterial)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Failed to create material"));
		return;
	}

	for (UTexture2D* SelectedTexture:SelectedTexturesArray)
	{
		if (!SelectedTexture) continue;

		switch (ChannelPackingType)
		{
		case E_ChannelPackingType::ECPT_NoChannelPacking:
			Default_CreateMaterialNodes(CreatedMaterial, SelectedTexture, PinsConnectedCounter);
			break;
		case E_ChannelPackingType::ECPT_ORM:
			ORM_CreateMaterialNodes(CreatedMaterial, SelectedTexture, PinsConnectedCounter);
			break;
		case E_ChannelPackingType::ECPT_MAX:
			break;
		default:
			break;
		}

	}

	if (PinsConnectedCounter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfullly connected ") + FString::FromInt(PinsConnectedCounter) + (TEXT(" pins")));
	}

	MaterialName = TEXT("M_");
}

//Process the selected data, will filter out textures, and return false if non-texture selected
bool UQuickMaterialCreationWidget::ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess,
	TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath)
{
	if (SelectedDataToProcess.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("No texture selected"));
		return false;
	}

	bool bMaterialNameSet = false;
	for (const FAssetData& SelectedData : SelectedDataToProcess)
	{
		UObject* SelectedAsset = SelectedData.GetAsset();

		if (!SelectedAsset) continue;
		UTexture2D* SelectedTexture = Cast<UTexture2D>(SelectedAsset);

		if (!SelectedTexture) {
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please select only texture\n") + SelectedAsset->GetName() + TEXT(" is not a texture"));
			return false;
		}

		OutSelectedTexturesArray.Add(SelectedTexture);

		if (OutSelectedTexturePackagePath.IsEmpty()) 
		{
			OutSelectedTexturePackagePath = SelectedData.PackagePath.ToString();
		}
		if (!bCustomMaterialName && !bMaterialNameSet)
		{
			MaterialName = SelectedAsset->GetName();
			MaterialName.RemoveFromStart(TEXT("T_"));
			MaterialName.InsertAt(0, TEXT("M_"));

			bMaterialNameSet = true;
		}

	}
	return true;
}

//Will return true if the material name is used by asset under the specified folder
bool UQuickMaterialCreationWidget::CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck)
{
	TArray<FString> ExistingAssetsPaths = UEditorAssetLibrary::ListAssets(FolderPathToCheck, false);
	for (const FString& ExistingAssetPath : ExistingAssetsPaths)
	{
		const FString ExistingAssetName = FPaths::GetBaseFilename(ExistingAssetPath);

		if (ExistingAssetName.Equals(MaterialNameToCheck))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, MaterialNameToCheck + TEXT(" is already used by asset"));
			return true;
		}
	}
	
	return false;
}

UMaterial* UQuickMaterialCreationWidget::CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial)
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();
	UObject* CreatedObject = AssetToolsModule.Get().CreateAsset(NameOfTheMaterial, PathToPutMaterial, 
		UMaterial::StaticClass(),MaterialFactory);
	return Cast<UMaterial>(CreatedObject);
}

void UQuickMaterialCreationWidget::Default_CreateMaterialNodes(UMaterial* CreatedMaterial,
	UTexture2D* SelectedTexture, uint32& PinsConnectedCounter)
{
	UMaterialExpressionTextureSample* TextureSampleNode = NewObject<UMaterialExpressionTextureSample>(CreatedMaterial);
	if (!TextureSampleNode) return;

	if (!CreatedMaterial->GetExpressionInputForProperty(MP_BaseColor)->IsConnected()) {
		if (TryConnectBaseColor(TextureSampleNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}

	if (!CreatedMaterial->GetExpressionInputForProperty(MP_Metallic)->IsConnected())
	{
		if(TryConnectMetallic(TextureSampleNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}

	if (!CreatedMaterial->GetExpressionInputForProperty(MP_Roughness)->IsConnected())
	{
		if (TryConnectMetallic(TextureSampleNode, SelectedTexture, CreatedMaterial)) 
		{
			PinsConnectedCounter++;
			return;
		}
	}

	if (CreatedMaterial->GetExpressionInputForProperty(MP_Normal)->IsConnected())
	{
		if (TryConnectNormal(TextureSampleNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}

	if (CreatedMaterial->GetExpressionInputForProperty(MP_AmbientOcclusion)->IsConnected())
	{
		if (TryConnectAO(TextureSampleNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}

	DebugHeader::PrintLog(TEXT("Failed to connect the texture: ") + SelectedTexture->GetName());

	return;
}

void UQuickMaterialCreationWidget::ORM_CreateMaterialNodes(UMaterial* CreatedMaterial, UTexture2D* SelectedTexture, uint32& PinsConnectedCounter)
{
	UMaterialExpressionTextureSample* TextureSamplerNode =
		NewObject<UMaterialExpressionTextureSample>(CreatedMaterial);
	if (!TextureSamplerNode) return;
	if (!CreatedMaterial->GetExpressionInputForProperty(MP_BaseColor)->IsConnected())
	{
		if (TryConnectBaseColor(TextureSamplerNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}
	if (!CreatedMaterial->GetExpressionInputForProperty(MP_Normal)->IsConnected())
	{
		if (TryConnectNormal(TextureSamplerNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}
	if(!CreatedMaterial->GetExpressionInputForProperty(MP_Roughness)->IsConnected())
	{
		if (TryConnectORM(TextureSamplerNode, SelectedTexture, CreatedMaterial)) {
			PinsConnectedCounter += 3;
			return;
		}
	}

}

#pragma endregion

#pragma region CreateMaterialNodeConnectPins
bool UQuickMaterialCreationWidget::TryConnectBaseColor(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial)
{
	for (const FString& BaseColorName : BaseColorArray) {
		if (SelectedTexture->GetName().Contains(BaseColorName))
		{
			//connect pins to base color socket here
			TextureSamplerNode->Texture = SelectedTexture;
			CreatedMaterial->GetExpressionCollection().AddExpression(TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_BaseColor)->Connect(0, TextureSamplerNode);
			CreatedMaterial->PostEditChange();

			TextureSamplerNode->MaterialExpressionEditorX -= 600;
			return true;
		}
	}
	return false;
}
bool UQuickMaterialCreationWidget::TryConnectMetallic(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial)
{
	for (const FString& MetallicName : MetallicArray)
	{
		if (SelectedTexture->GetName().Contains(MetallicName))
		{
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
			SelectedTexture->SRGB = false;
			SelectedTexture->PostEditChange();

			TextureSamplerNode->Texture = SelectedTexture;
			TextureSamplerNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;
		
			CreatedMaterial->GetExpressionCollection().AddExpression(TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_Metallic)->Connect(0, TextureSamplerNode);
			CreatedMaterial->PostEditChange();

			TextureSamplerNode->MaterialExpressionEditorX -= 600;
			TextureSamplerNode->MaterialExpressionEditorY += 240;
			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectRoughness(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial)
{
	for (const FString& RoughnessName : RoughnessArray)
	{
		if (SelectedTexture->GetName().Contains(RoughnessName))
		{
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
			SelectedTexture->SRGB = false;
			SelectedTexture->PostEditChange();

			TextureSamplerNode->Texture = SelectedTexture;
			TextureSamplerNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			CreatedMaterial->GetExpressionCollection().AddExpression(TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_Roughness)->Connect(0, TextureSamplerNode);

			TextureSamplerNode->MaterialExpressionEditorX -= 600;
			TextureSamplerNode->MaterialExpressionEditorY += 480;

			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectNormal(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial)
{
	for (const FString& NormalName : NormalArray)
	{
		if (SelectedTexture->GetName().Contains(NormalName))
		{
			TextureSamplerNode->Texture = SelectedTexture;
			TextureSamplerNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_Normal;

			CreatedMaterial->GetExpressionCollection().AddExpression(TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_Normal)->Connect(0,TextureSamplerNode);
			CreatedMaterial->PostEditChange();

			TextureSamplerNode->MaterialExpressionEditorX -= 600;
			TextureSamplerNode->MaterialExpressionEditorY += 720;

			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectAO(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial)
{
	for (const FString& AOName:AmbientOcclusionArray)
	{
		if (SelectedTexture->GetName().Contains(AOName))
		{
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
			SelectedTexture->SRGB = false;
			SelectedTexture->PostEditChange();

			TextureSamplerNode->Texture = SelectedTexture;
			TextureSamplerNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_LinearColor;

			CreatedMaterial->GetExpressionCollection().AddExpression(TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_AmbientOcclusion)->Connect(0, TextureSamplerNode);
			CreatedMaterial->PostEditChange();

			TextureSamplerNode->MaterialExpressionEditorX -= 600;
			TextureSamplerNode->MaterialExpressionEditorY += 960;

			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectORM(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial)
{
	for (const FString& ORM_Name : ORMArray)
	{
		if (SelectedTexture->GetName().Contains(ORM_Name))
		{
			SelectedTexture->CompressionSettings = TextureCompressionSettings::TC_Masks;
			SelectedTexture->SRGB = false;
			SelectedTexture->PostEditChange();

			TextureSamplerNode->Texture = SelectedTexture;
			TextureSamplerNode->SamplerType = EMaterialSamplerType::SAMPLERTYPE_Masks;

			CreatedMaterial->GetExpressionCollection().AddExpression(TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_AmbientOcclusion)->Connect(1, TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_Roughness)->Connect(2, TextureSamplerNode);
			CreatedMaterial->GetExpressionInputForProperty(MP_Metallic)->Connect(3, TextureSamplerNode);
			
			TextureSamplerNode->MaterialExpressionEditorX -= 600;
			TextureSamplerNode->MaterialExpressionEditorY += 960;

			return true;
		}
	}
	return false;
}

#pragma endregion

