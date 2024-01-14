#include "UI/DialogBoxUI.h"

void UDialogBoxUI::NativeDestruct()
{
	Super::NativeDestruct();

	if (ClickOKBtnFunction.IsBound())
		ClickOKBtnFunction.Unbind();
	if (ClickNoBtnFunction.IsBound())
		ClickNoBtnFunction.Unbind();
}

void UDialogBoxUI::OnClickOKBtn()
{
	if (IsInViewport()) {
		RemoveFromParent();
		if (ClickOKBtnFunction.IsBound()) {
			ClickOKBtnFunction.Execute();
		}
	}
}

void UDialogBoxUI::OnClickNoBtn()
{
	if (IsInViewport()) {
		RemoveFromParent();
		if (ClickNoBtnFunction.IsBound()) {
			ClickNoBtnFunction.Execute();
		}
	}
}