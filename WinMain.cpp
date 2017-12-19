#include "windows.h"
#include "bitmap.h"
#include "tchar.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#define WNDWIDTH 760
#define WNDHIGHT 480
//380    120

HINSTANCE hInst;
//优先级表
int prior[8][8]={
	{10,1,9,9,9,9,1,10},
	{1,1,7,3,3,7,1,1},
	{9,7,8,5,5,8,7,9},
	{9,3,5,0,0,5,3,9},
	{9,3,5,0,0,5,3,9},
	{9,7,8,5,5,8,7,9},
	{1,1,7,3,3,7,1,1},
	{10,1,9,9,9,9,1,10}
};

int grade;
int numb,numw;//黑棋和白棋的数目
int nowArray[4][4];
int prevArray[4][4];

int nowReversi[8][8];
int prevReversi[8][8];

bool InitWindowClass(HINSTANCE hInstance,int nCmdshow);
LRESULT CALLBACK wndProc(HWND,UINT,WPARAM,LPARAM);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	hInst=hInstance;
	MSG msg;
	InitWindowClass(hInstance,nCmdShow);
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return (int)msg.wParam;
}
HWND hWnd;
bool InitWindowClass(HINSTANCE hInstance,int nCmdShow)
{
	TCHAR szClassName[]=_T("GAME");
	HWND hWnd;
	WNDCLASSEX wndcex;
	wndcex.cbSize=sizeof(WNDCLASSEX);
	wndcex.cbClsExtra=0;
	wndcex.cbWndExtra=0;
	wndcex.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndcex.hCursor=(HCURSOR)LoadCursor(hInstance,IDC_ARROW);
	wndcex.hInstance=hInstance;
	wndcex.hIcon=(HICON)LoadIcon(hInstance,MAKEINTRESOURCE(IDI_WYICON));
	wndcex.hIconSm=LoadIcon(wndcex.hInstance,MAKEINTRESOURCE(IDI_WYICON));
	wndcex.lpfnWndProc=wndProc;
	wndcex.lpszClassName=szClassName;
	wndcex.lpszMenuName=szClassName;
	wndcex.style=0;

	RegisterClassEx(&wndcex);
	
	hWnd=CreateWindow(szClassName,szClassName,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,0,0,WNDWIDTH,WNDHIGHT,NULL,NULL,hInstance,NULL);
	
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;



}


enum STATUS{HOME,GAME_2048,GAME_HEIBAIQI,GAME_WUZICHOOSE,GAME_WUZI};//状态枚举

enum PLACE{WHITE=-1,BLACK=1,RUNNABLE};
POINT pCursor;
RECT rBack={550,320,650,420};
RECT rRestart={550,200,650,300};
RECT rUndo={430,320,530,420};
HBITMAP hRestartBitmap,hRestartBitmap2;
HBITMAP hBlackBitmap,hWhiteBitmap,hRunnableBitmap;
HBITMAP hUndoBitmap,hUndoBitmap2;

RECT rBM[3]={
	{30,110,330,210},
	{30,220,330,320},
	{30,330,330,430}
};//home中三个项目的位置

const int IDT_UPDATE=100001;

void clearArray();
void PaintHome(HWND hWnd,HDC hDC,HDC hDCM,HBITMAP hHomeBitmap,HBITMAP ahItemBitmap[],HBITMAP ahItemBitmap2[]);
void PaintGAME2048(HWND hWnd,HDC hDC,HDC hDCM,HBITMAP hBgBitmap1,HBITMAP hBackBitmap,HBITMAP hBackBitmap2,HBITMAP ahCellBitmap[]);
void PaintGAMEHEIBAIQI(HDC hDC,HDC hDCM,HBITMAP hBgBitmap2,HBITMAP hBackBitmap,HBITMAP hBackBitmap2);
//以下是关于2048的函数
void InitArray();
void SaveArray();
void SolveKey(WPARAM wParam);

bool isSame();
bool over();
POINT UpDateArray();//更新2048对应矩阵,并产生新出现的点的坐标(i,j)  若未生成  返回(-1,0)


RECT rPlayRange={60,60,420,420};//游戏区
//以下是关于黑白棋的函数

void ClearReversi();
void InitReversi();
void SaveReversi();//保存黑白子棋盘信息
void LaodReversi();//加载黑白子棋盘信息
void Player(int r,int c);
bool CanGo();
void MakeRunnable(PLACE p);//生成新的runnable,p为此步为谁走,即为他的对立面造runnable


POINT Computer();//电脑走  返回走的坐标

POINT newp;//鼠标当前位置


HBITMAP hHomeBitmap,hBgBitmap1,hBgBitmap2;
HBITMAP ahItemBitmap[3];//子项目位图
HBITMAP ahItemBitmap2[3];//子项目变化位图
HBITMAP hBackBitmap,hBackBitmap2;//返回位图对
HBITMAP ahCellBitmap[12];
HBITMAP hStartBitmap;
HBITMAP hWinBitmap1,hWinBitmap2,hLoseBitmap1,hLoseBitmap2,hTieBitmap;
HBITMAP hChooseBlackBitmap,hChooseBlackBitmap2,hChooseWhiteBitmap,hChooseWhiteBitmap2;
HBITMAP hChooseBGBitmap;//选棋时的背景位图
HBITMAP hWuziBlackBitmap,hWuziWhiteBitmap;//五子棋的黑白棋子位图

HDC hDCM;


//以下是关于五子棋的函数及全局变量


int wuziplayer;//1为黑 -1为白
int chess[15][15];//棋盘信息  1为黑  -1为白
int prevChess[15][15];//保存上一步棋盘信息
int mark[15][15];//评分表
void initChess();//初始化棋盘信息
void UpdateMark();//更新打分
void WuziComputer();//电脑走
void WuziPlayer(int r,int c);//玩家走

RECT rBlack={100,140,300,340};//选择黑棋
RECT rWhite={420,140,620,340};//选择黑棋

void PAINTGAMEWUZI(HDC hDC,HDC hDCM);//五子棋作图函数
void SaveChess();//存储棋盘信息
void LoadChess();//加载棋盘信息


static UINT status;//应用程序的状态
LRESULT CALLBACK wndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	HDC hDC;
	PAINTSTRUCT ps;
	
	
	static int xView,yView;
	static bool bStartImage=true;

	
	int i;	
	
	switch(message)
	{
		case WM_CREATE:
			SetTimer(hWnd,IDT_UPDATE,150,NULL);
			clearArray();
			RECT cliRect;
			GetClientRect(hWnd,&cliRect);
			xView=cliRect.right-cliRect.left;
			yView=cliRect.bottom-cliRect.top;

			
			
			hDC=GetDC(hWnd);
			hDCM=CreateCompatibleDC(hDC);
			
			hStartBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_STARTBITMAP));
		
			hHomeBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_HOMEBITMAP));

			ahItemBitmap[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_2048BITMAP));
			ahItemBitmap[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_HEIBAIQIBITMAP));
			ahItemBitmap[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WUZIBITMAP));

			ahItemBitmap2[0]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_20482BITMAP));
			ahItemBitmap2[1]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_HEIBAIQI2BITMAP));
			ahItemBitmap2[2]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WUZI2BITMAP));

			hBackBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BACKBITMAP));
			hBackBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BACK2BITMAP));

			hRestartBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_RESTARTBITMAP));
			hRestartBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_RESTART2BITMAP));

			hUndoBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_UNDOBITMAP));
			hUndoBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_UNDO2BITMAP));

			hBgBitmap1=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BG1BITMAP));
			hBgBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BG2BITMAP));
			int i;
			for(i=0;i<12;i++)
			{
				ahCellBitmap[i]=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CELL2BITMAP+i));
			}



			hBlackBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BLACKBITMAP));
			hWhiteBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WHITEBITMAP));
			hRunnableBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_RUNNABLEBITMAP));
			
			hWinBitmap1=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WIN1BITMAP));
			hWinBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WIN2BITMAP));
			hLoseBitmap1=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LOSE1BITMAP));
			hLoseBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LOSE2BITMAP));
			hTieBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_TIEBITMAP));

			hChooseBGBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CHOOSEBGBITMAP));
			hChooseBlackBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CHOOSEBLACKBITMAP));
			hChooseBlackBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CHOOSEBLACK2BITMAP));
			hChooseWhiteBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CHOOSEWHITEBITMAP));
			hChooseWhiteBitmap2=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CHOOSEWHITE2BITMAP));

			hWuziBlackBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WUZIBLACKBITMAP));
			hWuziWhiteBitmap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WUZIWHITEBITMAP));

			ReleaseDC(hWnd,hDC);
			status=HOME;
			break;
		
		
		case WM_TIMER:
			if(wParam==IDT_UPDATE)
			{
				if(GAME_WUZICHOOSE!=status) InvalidateRect(hWnd,&rBack,1);
				if(GAME_WUZICHOOSE!=status) InvalidateRect(hWnd,&rRestart,1);
				if(GAME_HEIBAIQI==status) InvalidateRect(hWnd,&rUndo,1);
				if(HOME==status)
				{
					for(i=0;i<3;i++) InvalidateRect(hWnd,&rBM[i],1);

				}
				if(GAME_WUZICHOOSE==status) 
				{
					InvalidateRect(hWnd,&rBlack,1);
					InvalidateRect(hWnd,&rWhite,1);
				}
			}
			break;

		case WM_PAINT:
			hDC=BeginPaint(hWnd,&ps);
			switch(status)
			{
			case HOME:
				if(bStartImage)
				{
					SelectObject(hDCM,hStartBitmap);
					SelectObject(hDC,hDCM);
					BitBlt(hDC,170,0,435,480,hDCM,0,0,SRCCOPY);

					Sleep(1500);
					bStartImage=false;
					InvalidateRect(hWnd,NULL,1);
				}
				else PaintHome(hWnd,hDC,hDCM,hHomeBitmap,ahItemBitmap,ahItemBitmap2);
				break;
			case GAME_2048:
				PaintGAME2048(hWnd,hDC,hDCM,hBgBitmap1,hBackBitmap,hBackBitmap2,ahCellBitmap);
				break;
			case GAME_HEIBAIQI:
				PaintGAMEHEIBAIQI(hDC,hDCM,hBgBitmap2,hBackBitmap,hBackBitmap2);
				break;

			case GAME_WUZICHOOSE:
				SelectObject(hDCM,hChooseBGBitmap);
				BitBlt(hDC,0,0,760,480,hDCM,0,0,SRCCOPY);
				if(!PtInRect(&rBlack,pCursor)) SelectObject(hDCM,hChooseBlackBitmap);
				else SelectObject(hDCM,hChooseBlackBitmap2);
				BitBlt(hDC,rBlack.left,rBlack.top,rBlack.right-rBlack.left,rBlack.bottom-rBlack.top,hDCM,0,0,SRCCOPY);

				if(!PtInRect(&rWhite,pCursor)) SelectObject(hDCM,hChooseWhiteBitmap);
				else SelectObject(hDCM,hChooseWhiteBitmap2);
				BitBlt(hDC,rWhite.left,rWhite.top,rWhite.right-rWhite.left,rWhite.bottom-rWhite.top,hDCM,0,0,SRCCOPY);
				break;

			case GAME_WUZI:
				PAINTGAMEWUZI(hDC,hDCM);
				break;
			}
			


			EndPaint(hWnd,&ps);
			break;
		case WM_DESTROY:
			DeleteObject(hHomeBitmap);
			for(i=0;i<sizeof(ahItemBitmap)/sizeof(HBITMAP);i++)
			{
				DeleteObject(ahItemBitmap[i]);
				DeleteObject(ahItemBitmap2[i]);
			}
			ReleaseDC(hWnd,hDCM);
			DeleteDC(hDCM);



			PostQuitMessage(0);
			break;
		case WM_MOUSEMOVE:
			pCursor.x=LOWORD(lParam);
			pCursor.y=HIWORD(lParam);
			rand();
			

			break;
		case WM_LBUTTONUP:
			if(HOME==status&&PtInRect(&rBM[0],pCursor)) 
			{
				status=GAME_2048;
				InitArray();
			}
			if(HOME==status&&PtInRect(&rBM[1],pCursor))
			{
				status=GAME_HEIBAIQI;
				ClearReversi();
				InitReversi();
			}
			if(HOME==status&&PtInRect(&rBM[2],pCursor))
			{
				status=GAME_WUZICHOOSE;
				//UpdateMark();
			}

			//五子棋
			if(GAME_WUZI==status&&PtInRect(&rRestart,pCursor))
			{
				initChess();
				UpdateMark();
				if(-1==wuziplayer) chess[7][7]=1;
			}
			if(GAME_WUZI==status&&PtInRect(&rUndo,pCursor))
			{
				LoadChess();
			}
			if(GAME_WUZI==status&&PtInRect(&rPlayRange,pCursor))
			{
				
				int r=(pCursor.y-60)/24,c=(pCursor.x-60)/24;
				if(chess[r][c]!=0) break;
				WuziPlayer(r,c);
				WuziComputer();
			}
			if(GAME_WUZICHOOSE==status)
			{
				if(PtInRect(&rBlack,pCursor)) 
				{
					wuziplayer=1;
					status=GAME_WUZI;
					initChess();
				}
				if(PtInRect(&rWhite,pCursor))
				{
					wuziplayer=-1;
					status=GAME_WUZI;
					initChess();
					chess[7][7]=1;
				}
			}
			



			if(HOME!=status&&PtInRect(&rBack,pCursor)) 
			{
				status=HOME;
				clearArray();
				initChess();
			}
			if(GAME_2048==status&&PtInRect(&rRestart,pCursor))
			{
				clearArray();
				InitArray();
			}


			if(GAME_HEIBAIQI==status&&PtInRect(&rRestart,pCursor))
			{
				ClearReversi();
				InitReversi();
			}

			if(GAME_HEIBAIQI==status&&PtInRect(&rUndo,pCursor))
			{
				LaodReversi();
			}
			if(GAME_HEIBAIQI==status&&PtInRect(&rPlayRange,pCursor))
			{
				
				int r=(pCursor.y-60)/45,c=(pCursor.x-60)/45;
				if(RUNNABLE==nowReversi[r][c])
				{
					SaveReversi();
					Player(r,c);
					
					
					InvalidateRect(hWnd,NULL,0);
					HDC hDC=GetDC(hWnd);
					PaintGAMEHEIBAIQI(hDC,hDCM,hBgBitmap2,hBackBitmap,hBackBitmap2);
		
					Sleep(1200);
					if(!CanGo())
					{
						MakeRunnable(WHITE);
						if(!CanGo())
						{
							HDC hDC=GetDC(hWnd);			
				
							if(numb>numw) 
							{
								SelectObject(hDCM,hWinBitmap1);
							}
							if(numb==numw)
							{
								SelectObject(hDCM,hTieBitmap);
							}
							if(numb<numw)							
							{
								SelectObject(hDCM,hLoseBitmap1);
							}
							SelectObject(hDC,hDCM);
							BitBlt(hDC,xView/2-112,yView/2-112,224,224,hDCM,0,0,SRCCOPY);
							Sleep(2500);

							ReleaseDC(hWnd,hDC);
						}
						MakeRunnable(BLACK);
					}
					Computer();
					
					if(!CanGo())
					{
						MakeRunnable(BLACK);
						if(!CanGo())
						{
							HDC hDC=GetDC(hWnd);
							if(numb>numw) //wsprintf(res,L"YOU WIN!");
							{
								SelectObject(hDCM,hWinBitmap2);
							}
							if(numb==numw) //wsprintf(res,L"TO BE BETTER!");
							{
								SelectObject(hDCM,hTieBitmap);
							}
							if(numb<numw) //wsprintf(res,L"YOU LOSE!");
							{
								SelectObject(hDCM,hLoseBitmap2);
							}	//MessageBox(hWnd,res,res,MB_OK);
							SelectObject(hDC,hDCM);
							BitBlt(hDC,xView/2-112,yView/2-112,224,224,hDCM,0,0,SRCCOPY);
							Sleep(2500);
							ReleaseDC(hWnd,hDC);
						}
						MakeRunnable(WHITE);
					}

	
					ReleaseDC(hWnd,hDC);
				}
				if(!CanGo()) 
				{
					MessageBox(hWnd,L"玩家无子可下,电脑继续",L"玩家无子可下,电脑继续",MB_OK);
				}
	}
			


			InvalidateRect(hWnd,NULL,1);
			break;
		case WM_KEYUP:
			if(VK_UP==wParam||VK_DOWN==wParam||VK_LEFT==wParam||VK_RIGHT==wParam)
			{
				SaveArray();
				SolveKey(wParam);

				if(!isSame()||over()) newp=UpDateArray();
				if(over()) 
				{
					TCHAR res[20];
					wsprintf(res,L"Your Grade is:%d",grade);
					MessageBox(hWnd,res,res,MB_OK);

				}
				InvalidateRect(hWnd,NULL,1);

			}
			break;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
			break;
	}

	return 0;
}


void PaintHome(HWND hWnd,HDC hDC,HDC hDCM,HBITMAP hHomeBitmap,HBITMAP ahItemBitmap[],HBITMAP ahItemBitmap2[])
{
	RECT cliRect;
	GetClientRect(hWnd,&cliRect);
	int xView=cliRect.right-cliRect.left;
	int yView=cliRect.bottom-cliRect.top;
	
	SelectObject(hDCM,hHomeBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,0,0,xView,yView,hDCM,0,0,SRCCOPY);
		
	int i;
	
	for(i=0;i<3;i++)
	{
		
		if(PtInRect(rBM+i,pCursor))
		{
			SelectObject(hDCM,ahItemBitmap2[i]);
		}
		else 
		{
			SelectObject(hDCM,ahItemBitmap[i]);
		}
		SelectObject(hDC,hDCM);
		BitBlt(hDC,rBM[i].left,rBM[i].top
				,rBM[i].right-rBM[i].left,rBM[i].bottom-rBM[i].top,hDCM,0,0,SRCCOPY);
				
	}

	HFONT hf,hof;
	hf=CreateFont(12,0,0,0,FW_BOLD,0,0,0,CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,L"宋体");
	hof=(HFONT)SelectObject(hDC,hf);
	TCHAR maker[]=L"制作人:康康(Kelvin) QQ:1642127033";
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC,RGB(0,0,215));
	TextOut(hDC,xView-240,yView-17,maker,wcslen(maker));

	
	SelectObject(hDC,hof);
	DeleteObject(hf);



	
}







void PaintGAME2048(HWND hWnd,HDC hDC,HDC hDCM,HBITMAP hBgBitmap1,HBITMAP hBackBitmap,HBITMAP hBackBitmap2,HBITMAP ahCellBitmap[])
{
	
	
	
	SelectObject(hDCM,hBgBitmap1);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,0,0,760,480,hDCM,0,0,SRCCOPY);

	if(PtInRect(&rBack,pCursor)) SelectObject(hDCM,hBackBitmap2); 
	else SelectObject(hDCM,hBackBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rBack.left,rBack.top,100,100,hDCM,0,0,SRCCOPY);
	

	if(PtInRect(&rRestart,pCursor)) SelectObject(hDCM,hRestartBitmap2); 
	else SelectObject(hDCM,hRestartBitmap);
	SelectObject(hDC,hDCM);	
	BitBlt(hDC,rRestart.left,rRestart.top,100,100,hDCM,0,0,SRCCOPY);
	


	HBRUSH hOldBrush,hBrush=CreateSolidBrush(RGB(160,160,160));
	hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
	Rectangle(hDC,60,60,420,420);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	HPEN hPen=CreatePen(0,5,RGB(130,130,130));
	HPEN hOldPen=(HPEN)SelectObject(hDC,hPen);
	int i;
	for(i=0;i<5;i++)
	{
		MoveToEx(hDC,60+i*90,60,NULL);
		LineTo(hDC,60+i*90,420);
		
		MoveToEx(hDC,60,60+i*90,NULL);
		LineTo(hDC,420,60+i*90);
	}//格子


	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);


	hPen=CreatePen(0,3,RGB(255,215,0));
	hOldPen=(HPEN)SelectObject(hDC,hPen);
	

	//新出现的
	if(newp.x!=-1)
	for(i=0;i<2;i++)
	{
		MoveToEx(hDC,60+(newp.y+i)*90,60+newp.x*90,NULL);
		LineTo(hDC,60+(newp.y+i)*90,150+newp.x*90);
		
		MoveToEx(hDC,60+newp.y*90,60+(newp.x+i)*90,NULL);
		LineTo(hDC,150+newp.y*90,60+(newp.x+i)*90);
	}

	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);


	int j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(0==nowArray[i][j]) continue;
			SelectObject(hDCM,ahCellBitmap[nowArray[i][j]-1]);
			SelectObject(hDC,hDCM);
			BitBlt(hDC,63+90*j,63+90*i,85,85,hDCM,0,0,SRCCOPY);

		}
	}
	TCHAR res[20];
	wsprintf(res,L"Grade:%4d",grade);


	HFONT hf,hof;
	hf=CreateFont(40,0,0,0,FW_BOLD,0,0,0,CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,L"宋体");
	hof=(HFONT)SelectObject(hDC,hf);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC,RGB(255,215,0));
	TextOut(hDC,rBack.left-40,62,res,wcslen(res));
	
	
	SelectObject(hDC,hof);
	DeleteObject(hf);


	
}



void clearArray()
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			nowArray[i][j]=0;
			prevArray[i][j]=0;
		}
	}
	grade=0;
}


void InitArray()
{
	int n=5;
	while(n--)
	{
		int index=rand()%16;
		nowArray[index/4][index%4]=(int)(rand()%4/3.0+1);
	}
	grade=0;
	newp.x=-1;
}



void SaveArray()
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			prevArray[i][j]=nowArray[i][j];
		}
	}

}

bool over()
{
	bool ret=true;
	int i,j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(nowArray[i][j]==0
				||(i>0&&nowArray[i][j]==nowArray[i-1][j])
				||(i<3&&nowArray[i][j]==nowArray[i+1][j])
				||(j>0&&nowArray[i][j]==nowArray[i][j-1])
				||(j<3&&nowArray[i][j]==nowArray[i][j+1])) ret=false;
		}
	}
	return ret;
}


POINT UpDateArray()
{
	int r,c;
	POINT pret;
	int count=1000;
	while(count--)
	{
		r=rand()%4;
		c=rand()%4;
		if(0==nowArray[r][c])
		{
			nowArray[r][c]=(int)(rand()%4/3.0+1);
			pret.x=r;
			pret.y=c;
			break;
		}
		if(count==0) 
		{
			if(over())
			{
				TCHAR mes[100];
				wsprintf(mes,L"GAME OVER!YOUR GRADE is %d",grade);
				MessageBox(hWnd,mes,L"GAME OVER",MB_OK);
			
			}
			pret.x=-1;
		}
	}
	return pret;	
}

void add(WPARAM wParam)
{
	int i,j,t;
	switch(wParam)
	{
	case VK_UP:
		for(i=0;i<3;i++)
		{
			for(j=0;j<4;j++)
			{
				if(0==nowArray[i][j]) continue;
				if(nowArray[i][j]==nowArray[i+1][j])
				{	
					grade+=1<<nowArray[i][j];
					nowArray[i][j]++;
					nowArray[i+1][j]=0;
				}
				
			}
		}
		break;
	case VK_DOWN:
		for(i=3;i>0;i--)
		{
			for(j=0;j<4;j++)
			{
				if(0==nowArray[i][j]) continue;
				if(nowArray[i][j]==nowArray[i-1][j])
				{
					grade+=1<<nowArray[i][j];
					nowArray[i][j]++;
					nowArray[i-1][j]=0;
				}
				
			}
		}
		break;
		case VK_LEFT:
		for(j=0;j<3;j++)
		{
			for(i=0;i<4;i++)
			{
				if(0==nowArray[i][j]) continue;
				if(nowArray[i][j]==nowArray[i][j+1])
				{
					grade+=1<<nowArray[i][j];
					nowArray[i][j]++;
					nowArray[i][j+1]=0;
				}
				
			}
		}
		break;
		
		case VK_RIGHT:
		for(j=3;j>0;j--)
		{
			for(i=0;i<4;i++)
			{
				if(0==nowArray[i][j]) continue;
				if(nowArray[i][j]==nowArray[i][j-1])
				{
					grade+=1<<nowArray[i][j];
					nowArray[i][j]++;
					nowArray[i][j-1]=0;
				}
				
			}
		}
		
		break;
	}
}


void move(WPARAM wParam)
{
	int i,j,t;
	switch(wParam)
	{
	case VK_UP:
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(0==nowArray[i][j]) continue;
				for(t=i-1;t>=0;t--)
				{
					if(0==nowArray[t][j])
					{
						nowArray[t][j]=nowArray[t+1][j];
						nowArray[t+1][j]=0;
					}
				}
			}
		}
		


		break;
	case VK_DOWN:
		for(i=3;i>=0;i--)
		{
			for(j=0;j<4;j++)
			{
				if(0==nowArray[i][j]) continue;
				for(t=i+1;t<4;t++)
				{
					if(0==nowArray[t][j])
					{
						
						nowArray[t][j]=nowArray[t-1][j];
						nowArray[t-1][j]=0;
					}
				}
			}
		}
		break;
		case VK_LEFT:
		for(j=1;j<4;j++)
		{
			for(i=0;i<4;i++)
			{
				if(0==nowArray[i][j]) continue;
				for(t=j-1;t>=0;t--)
				{
					if(0==nowArray[i][t])
					{
						nowArray[i][t]=nowArray[i][t+1];
						nowArray[i][t+1]=0;
					}
				}
			}
		}
		break;
		
		case VK_RIGHT:
		for(j=2;j>=0;j--)
		{
			for(i=0;i<4;i++)
			{
				if(0==nowArray[i][j]) continue;
				for(t=j+1;t<4;t++)
				{
					if(0==nowArray[i][t])
					{
						nowArray[i][t]=nowArray[i][t-1];
						nowArray[i][t-1]=0;
					}
				}
			}
		}
		
		break;
	}

}


bool isSame()
{
	bool ret=true;
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(nowArray[i][j]!=prevArray[i][j]) ret=false;
		}
	}
	return ret;

}

void SolveKey(WPARAM wParam)
{
	move(wParam);	
	add(wParam);
	move(wParam);
	
}






//*******************************黑白棋*************************
void ComputeNum()//计算黑白子数量
{
	numb=numw=0;
	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if(BLACK==nowReversi[i][j]) numb++;
			else if(WHITE==nowReversi[i][j]) numw++;
}

void PaintGAMEHEIBAIQI(HDC hDC,HDC hDCM,HBITMAP hBgBitmap2,HBITMAP hBackBitmap,HBITMAP hBackBitmap2)
{

	ComputeNum();

	SelectObject(hDCM,hBgBitmap2);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,0,0,760,480,hDCM,0,0,SRCCOPY);

	if(PtInRect(&rBack,pCursor)) SelectObject(hDCM,hBackBitmap2); 
	else SelectObject(hDCM,hBackBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rBack.left,rBack.top,100,100,hDCM,0,0,SRCCOPY);
	
	if(PtInRect(&rRestart,pCursor)) SelectObject(hDCM,hRestartBitmap2); 
	else SelectObject(hDCM,hRestartBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rRestart.left,rRestart.top,100,100,hDCM,0,0,SRCCOPY);

	if(PtInRect(&rUndo,pCursor)) SelectObject(hDCM,hUndoBitmap2); 
	else SelectObject(hDCM,hUndoBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rUndo.left,rUndo.top,100,100,hDCM,0,0,SRCCOPY);


	//格子
	HBRUSH hOldBrush,hBrush=CreateSolidBrush(RGB(160,160,160));
	hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
	Rectangle(hDC,60,60,420,420);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	HPEN hPen=CreatePen(0,3,RGB(130,130,130));
	HPEN hOldPen=(HPEN)SelectObject(hDC,hPen);
	int i;
	for(i=0;i<9;i++)
	{
		MoveToEx(hDC,60+i*45,60,NULL);
		LineTo(hDC,60+i*45,420);
		
		MoveToEx(hDC,60,60+i*45,NULL);
		LineTo(hDC,420,60+i*45);
	}


	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);


	int j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(0==nowReversi[i][j]) continue;
			if(WHITE==nowReversi[i][j]) SelectObject(hDCM,hWhiteBitmap);
			if(BLACK==nowReversi[i][j]) SelectObject(hDCM,hBlackBitmap);
			if(RUNNABLE==nowReversi[i][j]) SelectObject(hDCM,hRunnableBitmap);
			SelectObject(hDC,hDCM);
			BitBlt(hDC,63+45*j,63+45*i,40,40,hDCM,0,0,SRCCOPY);

		}
	}
		
	TCHAR res[20];
	HFONT hf,hof;
	hf=CreateFont(30,0,0,0,FW_BOLD,0,0,0,CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,L"宋体");
	hof=(HFONT)SelectObject(hDC,hf);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC,0x218868);
	wsprintf(res,L"黑子:%2d",numb);
	TextOut(hDC,rBack.left-20,62,res,wcslen(res));
	
	wsprintf(res,L"白子:%2d",numw);
	TextOut(hDC,rBack.left-20,100,res,wcslen(res));
	

	SelectObject(hDC,hof);
	DeleteObject(hf);

}


void ClearReversi()
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			nowReversi[i][j]=0;
			prevReversi[i][j]=0;
		}
	}
}


void InitReversi()
{
	int i,j;
	nowReversi[3][3]=BLACK;
	nowReversi[3][4]=WHITE;
	
	nowReversi[4][4]=BLACK;
	nowReversi[4][3]=WHITE;

	MakeRunnable(WHITE);
	SaveReversi();
}


void SaveReversi()
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			prevReversi[i][j]=nowReversi[i][j];
		}
	}

}


void LaodReversi()
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			nowReversi[i][j]=prevReversi[i][j];
		}
	}

}


void UpdateReversi(int r,int c);//翻转并生成新的runnable 输入为下的位置
void Player(int r,int c)
{
	nowReversi[r][c]=BLACK;
	UpdateReversi(r,c);
}



void UpdateReversi(int r,int c)
{
	int i,j,t,s;
	for(i=r-1;i>=0;i--)
	{
		if((i==0&&nowReversi[i][c]!=nowReversi[r][c])
			||0==nowReversi[i][c]
			||RUNNABLE==nowReversi[i][c])
		{
			for(t=r-1;t>i;t--)
				nowReversi[t][c]*=-1;
			break;
		}
		if(nowReversi[i][c]==nowReversi[r][c]) break;
		nowReversi[i][c]=nowReversi[r][c];
	}//翻转上面的
	for(i=r+1;i<8;i++)
	{
		if((i==7&&nowReversi[i][c]!=nowReversi[r][c])
			||0==nowReversi[i][c]
			||RUNNABLE==nowReversi[i][c])
		{
			for(t=r+1;t<i;t++)
				nowReversi[t][c]*=-1;
			break;
		}
		if(nowReversi[i][c]==nowReversi[r][c]) break;
		nowReversi[i][c]=nowReversi[r][c];
	}//翻转下面的
	for(j=c-1;j>=0;j--)
	{
		if((j==0&&nowReversi[r][j]!=nowReversi[r][c])
			||0==nowReversi[r][j]
			||RUNNABLE==nowReversi[r][j])
		{
			for(s=c-1;s>j;s--)
				nowReversi[r][s]*=-1;
			break;
		}
		if(nowReversi[r][j]==nowReversi[r][c]) break;
		nowReversi[r][j]=nowReversi[r][c];
	}//翻转左面的

	for(j=c+1;j<8;j++)
	{
		if((j==7&&nowReversi[r][j]!=nowReversi[r][c])
			||0==nowReversi[r][j]
			||RUNNABLE==nowReversi[r][j])
		{
			for(s=c+1;s<j;s++)
				nowReversi[r][s]*=-1;
			break;
		}
		if(nowReversi[r][j]==nowReversi[r][c]) break;
		nowReversi[r][j]=nowReversi[r][c];
	}//翻转右面的
	

	for(i=r-1,j=c-1;i>=0&&j>=0;--i,--j)
	{
		if(((i==0||j==0)&&nowReversi[i][j]!=nowReversi[r][c])
			||0==nowReversi[i][j]
			||RUNNABLE==nowReversi[i][j])
		{
			for(t=r-1,s=c-1;t>i&&s>j;--t,--s)
				nowReversi[t][s]*=-1;
			break;
		}

		if(nowReversi[i][j]==nowReversi[r][c]) break;
		nowReversi[i][j]=nowReversi[r][c];
	}//左上角
	for(i=r+1,j=c-1;i<8&&j>=0;++i,--j)
	{
		if(((i==7||j==0)&&nowReversi[i][j]!=nowReversi[r][c])
			||0==nowReversi[i][j]
			||RUNNABLE==nowReversi[i][j])
		{
			for(t=r+1,s=c-1;t<i&&s>j;++t,--s)
				nowReversi[t][s]*=-1;
			break;
		}
		if(nowReversi[i][j]==nowReversi[r][c]) break;
		nowReversi[i][j]=nowReversi[r][c];
	}//左下角
	for(i=r-1,j=c+1;i>=0&&j<8;--i,++j)
	{
		if(((i==0||j==7)&&nowReversi[i][j]!=nowReversi[r][c])
			||0==nowReversi[i][j]
			||RUNNABLE==nowReversi[i][j])
		{
			for(t=r-1,s=c+1;t>i&&s<j;--t,++s)
				nowReversi[t][s]*=-1;
			break;
		}
		if(nowReversi[i][j]==nowReversi[r][c]) break;
		nowReversi[i][j]=nowReversi[r][c];
	}//右上角
	for(i=r+1,j=c+1;i<8&&j<8;++i,++j)
	{
		if(((i==7||j==7)&&nowReversi[i][j]!=nowReversi[r][c])
			||0==nowReversi[i][j]
			||RUNNABLE==nowReversi[i][j])
		{
			for(t=r+1,s=c+1;t<i&&s<j;++t,++s)
				nowReversi[t][s]*=-1;
			break;
		}
		if(nowReversi[i][j]==nowReversi[r][c]) break;
		nowReversi[i][j]=nowReversi[r][c];
	}//右下角
	

	//生成新的runnable
	MakeRunnable((PLACE)nowReversi[r][c]);
}

int Counter(int r,int c,PLACE p);//计算吃字数
void MakeRunnable(PLACE p)//生成新的runnable,p为此步为谁走,即为他的对立面造runnable
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(RUNNABLE==nowReversi[i][j]) nowReversi[i][j]=0;
		}
	}//runnable清零
	
	int t,s;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(0!=nowReversi[i][j]) continue;
			
			int cou=Counter(i,j,p);

			if(cou!=0) nowReversi[i][j]=RUNNABLE;
		}
	}
}

int Counter(int r,int c,PLACE p)//计算吃字数
{

			int count=0,cou=0,t,s;
			//上
			for(t=r-1;t>=0;--t)
			{
				if(nowReversi[t][c]==-p)	break;
				
				if(0==t||0==nowReversi[t][c]||RUNNABLE==nowReversi[t][c])
				{
					count=0;
					break;
				}
				
				if(nowReversi[t][c]==p) ++count;
			}
			cou+=count;
			//下
			for(t=r+1;t<8;++t)
			{
				if(nowReversi[t][c]==-p) break;
				if(7==t||0==nowReversi[t][c]||RUNNABLE==nowReversi[t][c])	
				{
					count=0;
					break;
				}
				if(nowReversi[t][c]==p) ++count;
			}
			cou+=count;
			//左
			for(t=c-1;t>=0;--t)
			{
				if(nowReversi[r][t]==-p) break;
				if(0==t||0==nowReversi[r][t]||RUNNABLE==nowReversi[r][t])	
				{
					count=0;
					break;
				}
				if(nowReversi[r][t]==p) ++count;
			}
			cou+=count;
			//右
			for(t=c+1;t<8;++t)
			{
				if(nowReversi[r][t]==-p) 	break;
				
				if(7==t||0==nowReversi[r][t]||RUNNABLE==nowReversi[r][t])	
				{
					count=0;
					break;
				}
				if(nowReversi[r][t]==p) ++count;
			
			}
			cou+=count;
			//左上
			for(t=r-1,s=c-1;t>=0&&s>=0;--t,--s)
			{
				if(nowReversi[t][s]==-p)	break;
				
				if(0==t||0==s||0==nowReversi[t][s]||RUNNABLE==nowReversi[t][s])
				{
					count=0;
					break;
				}
				
				if(nowReversi[t][s]==p) ++count;
			}
			cou+=count;
			//左下
			for(t=r+1,s=c-1;t<8&&s>=0;++t,--s)
			{
				if(nowReversi[t][s]==-p)	break;
				
				if(7==t||0==s||0==nowReversi[t][s]||RUNNABLE==nowReversi[t][s])
				{
					count=0;
					break;
				}
				
				if(nowReversi[t][s]==p) ++count;
			}
			cou+=count;
			//右上
			for(t=r-1,s=c+1;t>=0&&s<8;--t,++s)
			{
				if(nowReversi[t][s]==-p)	break;

				if(0==t||7==s||0==nowReversi[t][s]||RUNNABLE==nowReversi[t][s])
				{
					count=0;
					break;
				}
				
				if(nowReversi[t][s]==p) ++count;
			}
			cou+=count;
			//右下
			for(t=r+1,s=c+1;t<8&&s<8;++t,++s)
			{
				if(nowReversi[t][s]==-p)	break;
				
				if(7==t||7==s||0==nowReversi[t][s]||RUNNABLE==nowReversi[t][s])	
				{
					count=0;
					break;
				}
				
				if(nowReversi[t][s]==p) ++count;
			}
			cou+=count;

			return cou;
}


POINT Computer()
{

	int r=-1,c=-1,i,j,maxprior=0;
	for(i=0;i<8;++i)
	{
		for(j=0;j<8;j++)
		{
			if(RUNNABLE==nowReversi[i][j]&&maxprior<prior[i][j])
			{
				maxprior=prior[i][j];
			}
		}
	}

	for(i=0;i<8;++i)
	{
		for(j=0;j<8;j++)
		{
			if(RUNNABLE==nowReversi[i][j]&&maxprior==prior[i][j]&&(r<0||c<0||Counter(r,c,BLACK)<Counter(i,j,BLACK))) 
			{
					r=i;
					c=j;
			}
		}
	}

	if(r>=0&&c>=0)	
	{
		nowReversi[r][c]=WHITE; 
		UpdateReversi(r,c);
	}
	else 
	{
		MessageBox(hWnd,L"电脑无字可走,玩家继续",L"电脑无字可走,玩家继续",MB_OK);
		MakeRunnable(WHITE);
	}
	POINT ret={r,c};
	return ret;
}



bool CanGo()
{
	int i,j;
	bool ret=false;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(RUNNABLE==nowReversi[i][j]) ret=true;
		}
	}
	return ret;
}


void initChess()
{
	int i,j;
	for(i=0;i<15;++i)
	{
		for(j=0;j<15;++j)
			chess[i][j]=0;
	}
}
int tupleScoreTable[]={7,//五元组为空
					35,//只有一颗电脑
					800,//只有两颗电脑
					15000,//只有三颗电脑
					800000,//只有四颗电脑
					15,//只有一颗玩家
					400,//只有两颗玩家
					1800,//只有三颗玩家
					100000,//只有四颗玩家
					0,//玩家电脑都有
					//0//五元组不存在
};


void UpdateMark()
{
	int i,j,k,q;
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			int score=0;
			for(k=-4;k<=0;k++)//上下方向
			{
				if(i+k<0||i+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=i+k;q<=i+k+4;q++)
				{
					if(wuziplayer==chess[q][j]) ++cp;
					if(wuziplayer+chess[q][j]==0) ++cc;
				}
				if(!cp) score+=tupleScoreTable[cc];
				else if(!cc) score+=tupleScoreTable[cp+4];
				else score+=tupleScoreTable[9];


			}
			
			for(k=-4;k<=0;k++)//左右方向
			{
				if(j+k<0||j+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=j+k;q<=j+k+4;q++)
				{
					if(wuziplayer==chess[i][q]) ++cp;
					if(wuziplayer+chess[i][q]==0) ++cc;
				}
				if(!cp) score+=tupleScoreTable[cc];
				else if(!cc) score+=tupleScoreTable[cp+4];
				else score+=tupleScoreTable[9];


			}

			for(k=-4;k<=0;k++)//主对角线方向
			{
				if(j+k<0||j+k+4>=15||i+k<0||i+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=k;q<=k+4;q++)
				{
					if(wuziplayer==chess[i+q][j+q]) ++cp;
					if(wuziplayer+chess[i+q][j+q]==0) ++cc;
				}
				if(!cp) score+=tupleScoreTable[cc];
				else if(!cc) score+=tupleScoreTable[cp+4];
				else score+=tupleScoreTable[9];


			}
			
			for(k=-4;k<=0;k++)//从对角线方向
			{
				if(j-k<0||j-(k+4)>=15||i+k<0||i+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=k;q<=k+4;q++)
				{
					if(wuziplayer==chess[i+q][j-q]) ++cp;
					if(wuziplayer+chess[i+q][j-q]==0) ++cc;
				}
				if(!cp) score+=tupleScoreTable[cc];
				else if(!cc) score+=tupleScoreTable[cp+4];
				else score+=tupleScoreTable[9];


			}

			mark[i][j]=score;
			
		}
	}


}

bool WuziIsOver(int r,int c)
{
		int k,q;
		for(k=-4;k<=0;k++)//上下方向
		{
			if(r+k<0||r+k+4>=15) continue;
			int cp=0,cc=0;
			for(q=r+k;q<=r+k+4;q++)
			{
				if(wuziplayer==chess[q][c]) ++cp;
				if(wuziplayer+chess[q][c]==0) ++cc;
			}
			if(cc==5||cp==5) return 1;
		}
			
			for(k=-4;k<=0;k++)//左右方向
			{
				if(c+k<0||c+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=c+k;q<=c+k+4;q++)
				{
					if(wuziplayer==chess[r][q]) ++cp;
					if(wuziplayer+chess[r][q]==0) ++cc;
				}
				if(cc==5||cp==5) return 1;

			}

			for(k=-4;k<=0;k++)//主对角线方向
			{
				if(c+k<0||c+k+4>=15||r+k<0||r+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=k;q<=k+4;q++)
				{
					if(wuziplayer==chess[r+q][c+q]) ++cp;
					if(wuziplayer+chess[r+q][c+q]==0) ++cc;
				}
				if(cc==5||cp==5) return 1;

			}
			
			for(k=-4;k<=0;k++)//从对角线方向
			{
				if(c-k<0||c-(k+4)>=15||r+k<0||r+k+4>=15) continue;
				int cp=0,cc=0;
				for(q=k;q<=k+4;q++)
				{
					if(wuziplayer==chess[r+q][c-q]) ++cp;
					if(wuziplayer+chess[r+q][c-q]==0) ++cc;
				}
				if(cc==5||cp==5) return 1;
			}
			return 0;
}

bool WuziIsOver()//无条件判断
{
	int i,j;
	for(i=0;i<15;++i)
	{
		for(j=0;j<15;++j)
		{
			if(WuziIsOver(i,j)) return 1; 
		}
	}
	return 0;
}


void WuziComputer()
{
	if(!WuziIsOver())
	{
		Sleep(800);
		UpdateMark();

		int i,j,r=0,c=0,max=0;
		for(i=0;i<15;i++)
		{
			for(j=0;j<15;j++)
			{
				if(!chess[i][j]&&mark[i][j]>max)	
				{
					max=mark[i][j];
					r=i;
					c=j;
				}
			}
		}//可下且分最高

		chess[r][c]=wuziplayer*-1;
		if(WuziIsOver(r,c)) 
		{
			HDC hDC=GetDC(hWnd);
			SelectObject(hDCM,hLoseBitmap2);
			SelectObject(hDC,hDCM);
			BitBlt(hDC,380-112,240-112,224,224,hDCM,0,0,SRCCOPY);
			ReleaseDC(hWnd,hDC);
			Sleep(2000);
		}
	}
}

void WuziPlayer(int r,int c)
{

	if(!WuziIsOver())
	{
		SaveChess();
		chess[r][c]=wuziplayer;
	
		if(WuziIsOver(r,c)) 
		{
			HDC hDC=GetDC(hWnd);
			SelectObject(hDCM,hWinBitmap2);
			SelectObject(hDC,hDCM);
			BitBlt(hDC,380-112,240-112,224,224,hDCM,0,0,SRCCOPY);
			ReleaseDC(hWnd,hDC);
			Sleep(2000);
		}
	}

}



void PAINTGAMEWUZI(HDC hDC,HDC hDCM)//绘制五子棋
{
	SelectObject(hDCM,hBgBitmap2);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,0,0,760,480,hDCM,0,0,SRCCOPY);

	if(PtInRect(&rBack,pCursor)) SelectObject(hDCM,hBackBitmap2); 
	else SelectObject(hDCM,hBackBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rBack.left,rBack.top,100,100,hDCM,0,0,SRCCOPY);
	
	if(PtInRect(&rRestart,pCursor)) SelectObject(hDCM,hRestartBitmap2); 
	else SelectObject(hDCM,hRestartBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rRestart.left,rRestart.top,100,100,hDCM,0,0,SRCCOPY);

	if(PtInRect(&rUndo,pCursor)) SelectObject(hDCM,hUndoBitmap2); 
	else SelectObject(hDCM,hUndoBitmap);
	SelectObject(hDC,hDCM);
	BitBlt(hDC,rUndo.left,rUndo.top,100,100,hDCM,0,0,SRCCOPY);


	//格子
	HBRUSH hOldBrush,hBrush=CreateSolidBrush(RGB(220,170,57));
	hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
	Rectangle(hDC,61,61,419,419);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	hBrush=CreateSolidBrush(RGB(220,170,57));
	hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
	Rectangle(hDC,72,72,408,408);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);
	HPEN hPen=CreatePen(0,1,RGB(0,0,0));
	HPEN hOldPen=(HPEN)SelectObject(hDC,hPen);
	int i;
	for(i=0;i<15;++i)
	{
		
		MoveToEx(hDC,72+i*24,72,NULL);
		LineTo(hDC,72+i*24,408);

		MoveToEx(hDC,72,72+i*24,NULL);
		LineTo(hDC,408,72+i*24);
	}
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);


	int j;
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			if(0==chess[i][j]) continue;
			if(-1==chess[i][j]) SelectObject(hDCM,hWuziWhiteBitmap);
			if(1==chess[i][j]) SelectObject(hDCM,hWuziBlackBitmap);
			SelectObject(hDC,hDCM);
			BitBlt(hDC,62+24*j,62+24*i,20,20,hDCM,0,0,SRCCOPY);

		}
	}

}


void SaveChess()
{

	int i,j;
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			prevChess[i][j]=chess[i][j];
		}
	}

}

void LoadChess()
{
	int i,j;
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			chess[i][j]=prevChess[i][j];
		}
	}
}


