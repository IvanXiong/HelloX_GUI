//***********************************************************************/
//    Author                    : Garry
//    Original Date             : April 11,2009
//    Module Name               : GUISHELL.H
//    Module Funciton           : 
//                                Window procedures for GUI shell are implemented in this
//                                file.Their definition are in GUISHELL.H file.
//
//    Last modified Author      :
//    Last modified Date        :
//    Last modified Content     :
//                                1.
//                                2.
//    Lines number              :
//***********************************************************************/

#include "..\INCLUDE\KAPI.H"
#include "..\INCLUDE\STDIO.H"
#include "..\INCLUDE\STRING.H"
#include "..\INCLUDE\VESA.H"
#include "..\INCLUDE\VIDEO.H"
#include "..\INCLUDE\CLIPZONE.H"
#include "..\INCLUDE\GDI.H"
#include "..\INCLUDE\WNDMGR.H"
#include "..\INCLUDE\GLOBAL.H"
#include "..\INCLUDE\GUISHELL.H"

#include "..\INCLUDE\MSGBOX.H"
#include "..\include\bmpbtn.h"
#include "..\include\launch.h"

//Window procedure of htreader programmer.
static DWORD HTReaderProc(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	HANDLE hDC = GetClientDC(hWnd);
	TCHAR  szMouseInfo[64];
	int x,y;
	DWORD  dwWndStatus = 0;
	DWORD  dwHitResult = 0;

	switch(message)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		break;
	case WM_MOUSEMOVE:
		dwWndStatus = GetWindowStatus(hWnd);
		if(dwWndStatus & WST_ONFOCUS)  //Only available in focus status.
		{
			y = lParam & 0x0000FFFF;
			x = (lParam >> 16) & 0x0000FFFF;
			sprintf(szMouseInfo,"��ǰ���λ��(�������Ļ): x = %d,y = %d",x,y);
			TextOut(hDC,0,0,szMouseInfo);
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_CLOSE:
		CloseWindow(hWnd);
	default:
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//Window procedure for telephone book program.
static DWORD TelBookProc(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	HANDLE hDC = GetClientDC(hWnd);
	switch(message)
	{
	case WM_PAINT:
	case WM_DRAW:
		TextOut(hDC,0,0,   "����    �Ա�    �绰����      ��ͥ��ַ");
		TextOut(hDC,0,21,  "ŷ����  ��      13827655562   �㶫ʡ������");
		TextOut(hDC,0,42,  "����    Ů      13515890743   �������һ�������198��");
		TextOut(hDC,0,63,  "������  ��      0755-88795479 �����б�����");
		TextOut(hDC,0,84,  "�ϴ�    ��      13977900059   �����з�̨����������");
		TextOut(hDC,0,105, "�ܲ�    ��      13209876543   ����ʡ�����л�����");
		break;
	case WM_CLOSE:
		CloseWindow(hWnd);
	default:
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//Window procedure for system setting pannel program.
static DWORD SysSetProc(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	HANDLE hDC = GetClientDC(hWnd);
	static int x = 0;
	static int y = 0;
	static TCHAR  Key[2] = {0};

	switch(message)
	{
	case WM_CHAR:
		Key[0] = (TCHAR)lParam;
		if(Key[0] == VK_RETURN)  //Change line.
		{
			x = 0;
			y += 21;
			break;
		}
		if(Key[0] == VK_BACKSPACE)
		{
			if(x >= 8)
			{
				x -= 8;
				TextOut(hDC,x,y,(TCHAR*)" ");  //Erase the character in this position.
				break;
			}
		}
		TextOut(hDC,x,y,(TCHAR*)&Key);
		x += 8;
		break;
	case WM_CLOSE:
		if(MessageBox(hWnd,"�ĵ��ѱ��޸ġ��Ƿ���������޸Ľ��б��棿","��ʾ",MB_YESNOCANCEL) != ID_CANCEL)
		{
			CloseWindow(hWnd);
		}
		break;
	case WM_LBUTTONDBLCLK:
		MessageBox(hWnd,"��ϣ���˳��ó�����","��ʾ",MB_YESNOCANCEL);
		break;
	case WM_CHILDCLOSE:
		DestroyWindow((HANDLE)lParam);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//A local helper routine to create three child window in main frame window.
static DWORD MainFrameOnCreate(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	TCHAR* pszWndTitle1 = "�绰��";
	TCHAR* pszWndTitle2 = "���ı��༭��(ֻ֧��Ӣ�ı༭) - HEdit0.2";
	TCHAR* pszWndTitle3 = "���ı��Ķ�����-HTReader v0.1";

	CreateWindow(
		WS_WITHCAPTION | WS_WITHBORDER,
		pszWndTitle1,
		100,
		INDICATEBAND_HEIGHT + 50,
		500,
		300,
		TelBookProc,
		hWnd,
		NULL,
		GlobalParams.COLOR_WINDOW,
		NULL);

	CreateWindow(
		WS_WITHCAPTION | WS_WITHBORDER,
		pszWndTitle2,
		250,
		INDICATEBAND_HEIGHT + 150,
		500,
		300,
		SysSetProc,
		hWnd,
		NULL,
		GlobalParams.COLOR_WINDOW,
		NULL);

	CreateWindow(
		WS_WITHCAPTION | WS_WITHBORDER,
		pszWndTitle3,
		400,
		INDICATEBAND_HEIGHT + 250,
		500,
		300,
		HTReaderProc,
		hWnd,
		NULL,
		//COLOR_TASKBAND,
		GlobalParams.COLOR_WINDOW,
		NULL);
	
	return 0;
}

//This routine will load all GUI application's profile in Hello China's GUI application
//directory,which is /guiapp/.
/*static void LoadAppProfile(HANDLE hMainFrame)
{
	HANDLE hBmpBtn = CreateBitmapButton(
		hMainFrame,
		"Clendar",
		1024,
		10,
		10,
		128,
		128,
		NULL,
		NULL);
}*/

//Window procedure for Main frame window,this window occupies almost the whole screen.
DWORD MainFrameProc(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	switch(message)
	{
	case WM_CREATE:
		//MainFrameOnCreate(hWnd,message,wParam,lParam);
		LoadAppProfile(hWnd);
		break;
	case WM_DRAW:
		break;
	case WM_COMMAND:
		LaunchApplication(wParam);
		break;
	case WM_CHILDCLOSE:
		DestroyWindow((HANDLE)lParam);
		break;
	case WM_LBUTTONDBLCLK:
		MessageBox(hWnd,"�ղ��������˫������Ļ��","Notification",MB_OKCANCEL);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//Window procedure for task band.
DWORD TaskBandProc(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	return DefWindowProc(hWnd,message,wParam,lParam);
}

//Window procedure for Inidcate band window.
DWORD IndicateBandProc(HANDLE hWnd,UINT message,WORD wParam,DWORD lParam)
{
	return DefWindowProc(hWnd,message,wParam,lParam);
}