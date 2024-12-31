// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "QuickMaterialCreationWidget.generated.h"

UENUM(BlueprintType)
enum class E_ChannelPackingType :uint8
{
	ECPT_NoChannelPacking UMETA(DisplayName = "No Channel Packing"),
	ECPT_ORM UMETA(DisplayName = "OcclusionRoughnessMetallic"),
	ECPT_MAX UMETA(DisplayName = "DefaultMAX")
};
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "CreateMaterialFromSelectedTextures")
	E_ChannelPackingType ChannelPackingType = E_ChannelPackingType::ECPT_NoChannelPacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bCustomMaterialName = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures", meta = (EditCondition = "bCustomMaterialName"))
	FString MaterialName = TEXT("M_");

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "CreateMaterialFromSelectedTextures")
	bool bCreateMaterialInstance = false;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupperotedTextureNames")
	TArray<FString> ORMArray = {
		TEXT("_arm"),
		TEXT("_OcclusionRoughnessMetallic"),
		TEXT("_ORM")
	};

#pragma endregion

private:
#pragma region QuickMaterialCreation
	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProcess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath);
	bool CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck);
	UMaterial* CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial);
	void Default_CreateMaterialNodes(UMaterial* CreatedMaterial, UTexture2D* SelectedTexture, uint32& PinsConnectedCounter);
	void ORM_CreateMaterialNodes(UMaterial* CreatedMaterial, UTexture2D* SelectedTexture, uint32& PinsConnectedCounter);
#pragma endregion

#pragma region CreateMaterialNodeConnectPins
	bool TryConnectBaseColor(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectMetallic(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectRoughness(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectNormal(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectAO(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
	bool TryConnectORM(UMaterialExpressionTextureSample* TextureSamplerNode, UTexture2D* SelectedTexture, UMaterial* CreatedMaterial);
#pragma endregion
	class UMaterialInstanceConstant* CreateMaterialInstanceAsset(UMaterial* CreatedMaterial, FString NameOfMaterialInstance, const FString& PathToPutMaterialInstance);
};
