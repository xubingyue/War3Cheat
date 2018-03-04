// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ShareMemory/GameShareMemory.h"
#include "Feature/CheatFeature.h"

#define _SELF L"dllmain.cpp"

static HANDLE hWorkThread = NULL;
static BOOL   bRunWorkThread = FALSE;

DWORD WINAPI _WorkThread(LPVOID )
{
	CCheatFeature CheatFeautre;
	CGameShareMemory::CheatActionContent ActionContent;


	while (bRunWorkThread)
	{
		if (!CGameShareMemory::GetInstance().ExistAction(ActionContent))
		{
			::Sleep(100);
			continue;
		}

		DWORD dwSelectedGameObject = CheatFeautre.GetSelectedGameObject();
		if (ActionContent.emActionType != em_Action_Type::PrintItem && dwSelectedGameObject == NULL)
		{
			::MessageBoxW(NULL, L"Please Select Your Operation Hero...", L"Prompt", NULL);
			continue;
		}


		switch (ActionContent.emActionType)
		{
		case em_Action_Type::ChangeItem:
			CheatFeautre.ChangeItem(dwSelectedGameObject, ActionContent.GetChangeItemParamToItemIndex(), ActionContent.GetChangeItemParamToItemId());
			break;
		case em_Action_Type::PrintItem:
			CheatFeautre.PrintItem();
			break;
		case em_Action_Type::SetSelectedHeroSkillCool:
			CheatFeautre.SetSelectedHeroSkillCool(dwSelectedGameObject);
			break;
		case em_Action_Type::SetSelectedObjectAttackType:
			CheatFeautre.SetSelectedObjectAttackType(dwSelectedGameObject);
			break;
		case em_Action_Type::SetSelectedObjectInvincible:
			CheatFeautre.SetSelectedObjectInvincible(dwSelectedGameObject);
			break;
		default:
			break;
		}
	}

	
	return 0;
}

VOID Release()
{
	if (hWorkThread != NULL)
	{
		bRunWorkThread = FALSE;
		::WaitForSingleObject(hWorkThread, INFINITE);
		::CloseHandle(hWorkThread);
		hWorkThread = NULL;

		CGameShareMemory::GetInstance().Release();
	}
}

BOOL APIENTRY DllMain(HMODULE , DWORD  ul_reason_for_call, LPVOID )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (hWorkThread == NULL && CGameShareMemory::GetInstance().Run())
		{
			bRunWorkThread = TRUE;
			hWorkThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_WorkThread, NULL, NULL, NULL);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		Release();
		break;
	}
	return TRUE;
}

