#pragma once
#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace DebugHeader
{
	static void PrintLog(const FString& Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}

	EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type MsgType, const FString& Message, bool bShowMsgAsWarning = true)
	{
		if (bShowMsgAsWarning) {
			FText MsgTitle = FText::FromString(TEXT("Warning"));
			return FMessageDialog::Open(MsgType, FText::FromString(Message), &MsgTitle);
		}
		else
		{
			return FMessageDialog::Open(MsgType, FText::FromString(Message));
		}
	
	}

	static void ShowNotifyInfo(const FString& Message) {
		FNotificationInfo NotifyInfo(FText::FromString(Message));
		NotifyInfo.bUseLargeFont = false;
		NotifyInfo.FadeOutDuration = 7.0f;

		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}
}
