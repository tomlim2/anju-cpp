// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SuperManager/Public/AssetActions/QuickAssetActions.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeQuickAssetActions() {}
// Cross Module References
	BLUTILITY_API UClass* Z_Construct_UClass_UAssetActionUtility();
	SUPERMANAGER_API UClass* Z_Construct_UClass_UQuickAssetActions();
	SUPERMANAGER_API UClass* Z_Construct_UClass_UQuickAssetActions_NoRegister();
	UPackage* Z_Construct_UPackage__Script_SuperManager();
// End Cross Module References
	DEFINE_FUNCTION(UQuickAssetActions::execRemoveUnsuedAssets)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->RemoveUnsuedAssets();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UQuickAssetActions::execAddPrefixes)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->AddPrefixes();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UQuickAssetActions::execDuplicateAssets)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_NumOfDuplicates);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->DuplicateAssets(Z_Param_NumOfDuplicates);
		P_NATIVE_END;
	}
	void UQuickAssetActions::StaticRegisterNativesUQuickAssetActions()
	{
		UClass* Class = UQuickAssetActions::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "AddPrefixes", &UQuickAssetActions::execAddPrefixes },
			{ "DuplicateAssets", &UQuickAssetActions::execDuplicateAssets },
			{ "RemoveUnsuedAssets", &UQuickAssetActions::execRemoveUnsuedAssets },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UQuickAssetActions_AddPrefixes_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UQuickAssetActions_AddPrefixes_Statics::Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "ModuleRelativePath", "Public/AssetActions/QuickAssetActions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UQuickAssetActions_AddPrefixes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UQuickAssetActions, nullptr, "AddPrefixes", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UQuickAssetActions_AddPrefixes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UQuickAssetActions_AddPrefixes_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_UQuickAssetActions_AddPrefixes()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UQuickAssetActions_AddPrefixes_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics
	{
		struct QuickAssetActions_eventDuplicateAssets_Parms
		{
			int32 NumOfDuplicates;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_NumOfDuplicates;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::NewProp_NumOfDuplicates = { "NumOfDuplicates", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(QuickAssetActions_eventDuplicateAssets_Parms, NumOfDuplicates), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::NewProp_NumOfDuplicates,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "ModuleRelativePath", "Public/AssetActions/QuickAssetActions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UQuickAssetActions, nullptr, "DuplicateAssets", nullptr, nullptr, Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::PropPointers), sizeof(Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::QuickAssetActions_eventDuplicateAssets_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::Function_MetaDataParams), Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::QuickAssetActions_eventDuplicateAssets_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets_Statics::Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "ModuleRelativePath", "Public/AssetActions/QuickAssetActions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UQuickAssetActions, nullptr, "RemoveUnsuedAssets", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets_Statics::Function_MetaDataParams), Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UQuickAssetActions);
	UClass* Z_Construct_UClass_UQuickAssetActions_NoRegister()
	{
		return UQuickAssetActions::StaticClass();
	}
	struct Z_Construct_UClass_UQuickAssetActions_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UQuickAssetActions_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAssetActionUtility,
		(UObject* (*)())Z_Construct_UPackage__Script_SuperManager,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UQuickAssetActions_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UQuickAssetActions_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UQuickAssetActions_AddPrefixes, "AddPrefixes" }, // 1249991336
		{ &Z_Construct_UFunction_UQuickAssetActions_DuplicateAssets, "DuplicateAssets" }, // 2031861646
		{ &Z_Construct_UFunction_UQuickAssetActions_RemoveUnsuedAssets, "RemoveUnsuedAssets" }, // 4098812079
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UQuickAssetActions_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UQuickAssetActions_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Object" },
		{ "IncludePath", "AssetActions/QuickAssetActions.h" },
		{ "ModuleRelativePath", "Public/AssetActions/QuickAssetActions.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UQuickAssetActions_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UQuickAssetActions>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UQuickAssetActions_Statics::ClassParams = {
		&UQuickAssetActions::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UQuickAssetActions_Statics::Class_MetaDataParams), Z_Construct_UClass_UQuickAssetActions_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UQuickAssetActions()
	{
		if (!Z_Registration_Info_UClass_UQuickAssetActions.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UQuickAssetActions.OuterSingleton, Z_Construct_UClass_UQuickAssetActions_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UQuickAssetActions.OuterSingleton;
	}
	template<> SUPERMANAGER_API UClass* StaticClass<UQuickAssetActions>()
	{
		return UQuickAssetActions::StaticClass();
	}
	UQuickAssetActions::UQuickAssetActions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuickAssetActions);
	UQuickAssetActions::~UQuickAssetActions() {}
	struct Z_CompiledInDeferFile_FID_ue5extools_Plugins_SuperManager_Source_SuperManager_Public_AssetActions_QuickAssetActions_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_ue5extools_Plugins_SuperManager_Source_SuperManager_Public_AssetActions_QuickAssetActions_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UQuickAssetActions, UQuickAssetActions::StaticClass, TEXT("UQuickAssetActions"), &Z_Registration_Info_UClass_UQuickAssetActions, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UQuickAssetActions), 464734914U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_ue5extools_Plugins_SuperManager_Source_SuperManager_Public_AssetActions_QuickAssetActions_h_1910381248(TEXT("/Script/SuperManager"),
		Z_CompiledInDeferFile_FID_ue5extools_Plugins_SuperManager_Source_SuperManager_Public_AssetActions_QuickAssetActions_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_ue5extools_Plugins_SuperManager_Source_SuperManager_Public_AssetActions_QuickAssetActions_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
