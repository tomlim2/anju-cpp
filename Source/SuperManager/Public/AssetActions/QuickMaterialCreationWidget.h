// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "QuickMaterialCreationWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:

#pragma region QuickMaterialCreationCore
	UFUNCTION(BlueprintCallable)
	void CreateMaterialFromSelectedTextures();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bCustomMaterialName = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures", meta = (EditCondition = "bCustomMaterialName"))
	FString MaterialName = TEXT("M_");
#pragma endregion

#pragma region SupportedTextureNames
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SupperotedTextureNames")
	TArray<FString> BaseColorArray = {
		TEXT("_BaseColor"),
		TEXT("_Albedo"),
		TEXT("_Diffuse"),
		TEXT("_diff")
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupperotedTextureNames")
	TArray<FString> MetallicArray = {
		TEXT("Metallic"),
		TEXT("metal")
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupperotedTextureNames")
	TArray<FString> RoughnessArray = {
		TEXT("_Roughness"),
		TEXT("_RoughnessMap"),
		TEXT("_rough")
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupperotedTextureNames")
	TArray<FString> NormalArray = {
		TEXT("_Normal"),
		TEXT("_NormalMap"),
		TEXT("_nor")
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupperotedTextureNames")
	TArray<FString> AmbientOcclusionArray = {
		TEXT("_AmbientOcclusion"),
		TEXT("_AmbientOcclusionMap"),
		TEXT("_AO")
	};

#pragma endregion

private:
#pragma region QuickMaterialCreation
	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath);
	bool CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck);
	UMaterial* CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial);
	void Default_CreateMaterialNodes(UMaterial* CreatedMaterial, UTexture2D* SelectedTexture, uint32& PinsConnectedCounter);
#pragma endregion

#pragma region CreateMaterialNodeConnectPins
	bool TryConnectBaseColor(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectMetallic(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectRoughness(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectNormal(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectAO(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);

#pragma endregion

};
