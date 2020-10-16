#include <ButtonController.h>


//消息队列句柄
extern QueueHandle_t Key_Queue;
//图片数据全局变量
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP PNGLEDbutton_close;
extern GUI_CONST_STORAGE GUI_BITMAP PNGLEDbutton_open;

/*********************************************************************************************************

										ButtonController应用程序

**********************************************************************************************************/
#define ID_WINDOW_0   (GUI_ID_USER + 1)
#define ID_BUTTON_0   (GUI_ID_USER + 2)
#define ID_BUTTON_1   (GUI_ID_USER + 3)

/****************全局变量******************/
static u8 keystate=0;
static u8 showkey=0;

/*ButtonController对话框*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateButtonController[] = {
{ WINDOW_CreateIndirect, "", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x64, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 174, 97, 200, 60, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 700, 0, 100, 100, 0, 0x0, 0 },
};


/*********************************************************************
按键自定义
**********************************************************************/
/*
*********************************************************************************************************
*	函 数 名: _cbButtonBack
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButtonBack(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, 40, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, 40, 5);				
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}


/*
*********************************************************************************************************
*	函 数 名: __cbLEDBUTTON
*	功能说明: LED灯按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbLEDBUTTON(WM_MESSAGE * pMsg) 
{

	switch (pMsg->MsgId) 
	{		
		case WM_PAINT:
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_close, 0, 0);
		
			if(showkey)
			{
				if(keystate==1)
				{
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_open, 0, 0);
				}
				else if(keystate==0)
				{
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&PNGLEDbutton_close, 0, 0);

				}
		
			}	
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}




/*ButtonController回调函数*/
static void _cbCallbackButtonController(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	static u8 key=0;
	WM_HWIN hWin = pMsg->hWin;
	int     NCode;
	int     Id;

	// USER START (Optionally insert additional variables)
	// USER END

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:


		//
		// Initialization of 'Button'
		//
	xQueueReceive(Key_Queue,&key,10);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "LED1");
		WM_SetCallback(hItem, _cbLEDBUTTON);

	
	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "BACK");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonBack);

		// USER START (Optionally insert additional code for further widget initialization)
		// USER END
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		
		
		NCode = pMsg->Data.v;
		switch (Id) {
	
			case ID_BUTTON_0: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
					showkey=1;
					if(key==0)
					{
					key=1;
					keystate=1;
					}
					else if(key==1)
					{
					key=0;
					keystate=0;
					}
					xQueueSend(Key_Queue,&key,10);
					

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
			
		case ID_BUTTON_1: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				GUI_EndDialog(hWin, 0);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
			// USER START (Optionally insert additional code for further Ids)
			// USER END
		}
		break;
		// USER START (Optionally insert additional message handling)
		// USER END
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*ButtonController应用创建*/
void App_ButtonController(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreateButtonController,
		GUI_COUNTOF(_aDialogCreateButtonController),
		&_cbCallbackButtonController,
		hWin,
		0,
		0);
}





