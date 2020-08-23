#include<windows.h>
#include<windowsx.h>
#include<bits\stdc++.h>
#include"main.h"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)
using namespace std;
const char Ip[20]="172.16.14.165";
HWND hwnd_login,hwnd_out,hwnd_ts,hwnd_name,hwnd_in,hwnd_send,hwnd_list,hwnd_back,hwnd_add,hwnd_del,hwnd_ip,hwnd_shack;
int step,step_bj1,step_bj2,last=-2000;
SOCKET Sock;
int size=100;
void work_1(int n_cmd) {
	ShowWindow(hwnd_out,n_cmd);
	ShowWindow(hwnd_in,n_cmd);
	ShowWindow(hwnd_send,n_cmd);
	ShowWindow(hwnd_add,n_cmd);
	ShowWindow(hwnd_del,n_cmd);
	ShowWindow(hwnd_shack,n_cmd);
}
void work_2(int n_cmd) {
	ShowWindow(hwnd_back,n_cmd);
	ShowWindow(hwnd_list,n_cmd);
}
char cmp[1500],buf[200000],IIIPPP[20];
int nownownow=7;
HFONT font;
char time_l[20],time_n[20];
void show_time(HWND hwndout) {
	time_t timep;
	time(&timep);
	strftime(time_n,sizeof(time_n),"%Y-%m-%d %H:%M",localtime(&timep));
	if(strcmp(time_l,time_n)) {
		strcpy(time_l,time_n);
		strcat(buf,"           ");
		strcat(buf,time_n);
		strcat(buf,"\r\n");
	}
}
HWND hwnd;
char buf_name[1000]="Online users:";
DWORD WINAPI Shack(LPVOID V_sock) {
	MessageBox(hwnd,"You have unread messages!","Intention",MB_ICONEXCLAMATION|MB_OK|MB_SYSTEMMODAL);
}
DWORD WINAPI hide(LPVOID V_sock) {
	int bbb=1;
	while(1) {
		if(KEY_DOWN(VK_MENU)&&KEY_DOWN('X')&&!KEY_DOWN(VK_CONTROL)&&!KEY_DOWN(VK_RETURN)) {
			bbb^=1;
			ShowWindow(hwnd,bbb);
			Sleep(500);
		}
		Sleep(30);
	}
}
int time_shack=-100000;
char Name[15];
DWORD WINAPI receive(LPVOID V_sock) {
	SOCKET con=(SOCKET)V_sock;
	char rev[1000];
	while(1) {
		int y=recv(con,rev,1000,0);
		rev[y]=0;
		if(y<=0) {
			MessageBox(hwnd,"Disconneted!","Error!",MB_ICONEXCLAMATION|MB_OK);
			exit(0);
		}
		char pan=rev[0];
		if(pan=='!')MessageBox(hwnd,rev+1,"Error!",MB_ICONEXCLAMATION|MB_OK);
		if(pan=='L') {
			ShowWindow(hwnd_ts,0);
			ShowWindow(hwnd_name,0);
			ShowWindow(hwnd_login,0);
			ShowWindow(hwnd_ip,0);
			step=1;
			work_1(1);
		}
		if(pan=='.')buf_name[13]=0;
		if(pan=='&') {
			show_time(hwnd_out);
			strcat(buf,rev+1);
			strcat(buf,"\r\n");
			SetWindowText(hwnd_out,buf);
		}
		if(pan=='>') {
			int p=0;
			while(buf_name[p])p++;
			for(int i=0; rev[i]; i++)if(rev[i]=='>') {
					buf_name[p++]='\r';
					buf_name[p++]='\n';
				} else buf_name[p++]=rev[i];
			buf_name[p]=0;
			SetWindowText(hwnd_list,buf_name);
		}
		if(pan=='K') {
			DWORD ls_handle_id;
			CreateThread(NULL,0,Shack,(LPVOID)Sock,0,&ls_handle_id);
		}
	}
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam) {
	if(Message==WM_SIZE) {
		RECT p;
		GetClientRect(hwnd,&p);
		int x=p.right-p.left,y=p.bottom-p.top;
		MoveWindow(hwnd_out,0,0,x,y-size,1);
		MoveWindow(hwnd_in,0,y-size,x,size-30,1);
		MoveWindow(hwnd_send,x-50,y-30,50,30,1);
		MoveWindow(hwnd_list,0,30,130,y/2,1);
		MoveWindow(hwnd_add,x-180,y-30,80,30,1);
		MoveWindow(hwnd_del,x-260,y-30,80,30,1);
		MoveWindow(hwnd_shack,x-101,y-30,50,30,1);
		InvalidateRect(hwnd_send,0,1);
		InvalidateRect(hwnd_add,0,1);
		InvalidateRect(hwnd_del,0,1);
		InvalidateRect(hwnd_shack,0,1);
		UpdateWindow(hwnd_send);
		UpdateWindow(hwnd_add);
		UpdateWindow(hwnd_del);
		UpdateWindow(hwnd_shack);
	} else if(Message==WM_CREATE) {
		HINSTANCE hin=((LPCREATESTRUCT)lParam)->hInstance;
		hwnd_ts=CreateWindow("edit","      IP:\r\nName:",WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE|WS_DISABLED,212,152,47,36,hwnd,NULL,hin,NULL);
		hwnd_name=CreateWindow("edit",NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE,259,170,180,18,hwnd,NULL,hin,NULL);
		hwnd_ip=CreateWindow("edit","172.16.14.165",WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE,259,152,180,18,hwnd,NULL,hin,NULL);
		hwnd_login=CreateWindow("BUTTON","Login",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,259,200,72,30,hwnd,(HMENU)LOGIN,NULL,NULL);
		hwnd_out=CreateWindow("edit",NULL,WS_CHILD|WS_VSCROLL|WS_HSCROLL|WS_BORDER|ES_READONLY|ES_MULTILINE,0,0,0,0,hwnd,NULL,hin,NULL);
		hwnd_in=CreateWindow("edit",NULL,WS_CHILD|WS_VSCROLL|WS_BORDER|ES_MULTILINE,0,0,0,0,hwnd,NULL,hin,NULL);
		hwnd_send=CreateWindow("BUTTON","Send",WS_CHILD|BS_DEFPUSHBUTTON,0,0,0,0,hwnd,(HMENU)SEND,NULL,NULL);
		hwnd_add=CreateWindow("BUTTON","Increase",WS_CHILD|BS_DEFPUSHBUTTON,0,0,0,0,hwnd,(HMENU)ADD,NULL,NULL);
		hwnd_del=CreateWindow("BUTTON","Decrease",WS_CHILD|BS_DEFPUSHBUTTON,0,0,0,0,hwnd,(HMENU)DEL,NULL,NULL);
		hwnd_shack=CreateWindow("BUTTON","Shake",WS_CHILD|BS_DEFPUSHBUTTON,0,0,0,0,hwnd,(HMENU)SHACK,NULL,NULL);
		hwnd_back=CreateWindow("BUTTON","Back",WS_CHILD|BS_DEFPUSHBUTTON,0,0,50,30,hwnd,(HMENU)BACK,NULL,NULL);
		hwnd_list=CreateWindow("edit",NULL,WS_CHILD|WS_VSCROLL|WS_HSCROLL|WS_BORDER|ES_READONLY|ES_MULTILINE,0,0,100,100,hwnd,(HMENU)LIST,hin,NULL);
		Edit_LimitText(hwnd_in,1300);
		Edit_LimitText(hwnd_name,18);
		Edit_LimitText(hwnd_ip,16);
		font=CreateFont(-nownownow*2,-nownownow,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Î¢ÈíÑÅºÚ");
		SendMessage(hwnd_out,WM_SETFONT,(WPARAM)font,(LPARAM)NULL);
		SendMessage(hwnd_in,WM_SETFONT,(WPARAM)font,(LPARAM)NULL);
	} else if(Message==WM_CLOSE)DestroyWindow(hwnd);
	else if(Message==WM_DESTROY)PostQuitMessage(0);
	else if(Message==WM_COMMAND) {
		switch(LOWORD(wParam)) {
			case SHACK: {
				if(time_shack+3000>=clock()) {
					MessageBox(hwnd,"You shack too fast!","Error!",MB_ICONEXCLAMATION|MB_OK);
					break;
				}
				time_shack=clock();
				send(Sock,"KS[Shacked everyone]",20,0);
				break;
			}
			case LOGIN: {
				memset(Name,0,sizeof(Name));
				GetWindowText(hwnd_name,Name,15);
				GetWindowText(hwnd_ip,IIIPPP,15);
				int bj=0;
				for(int i=0; Name[i]; i++) {
					if('0'<=Name[i]&&Name[i]<='9')continue;
					if('a'<=Name[i]&&Name[i]<='z')continue;
					if('A'<=Name[i]&&Name[i]<='Z')continue;
					if(Name[i]=='_')continue;
					MessageBox(hwnd,"Nickname must consist of up to 24 letters,numbers,and underscores!","Error!",MB_ICONEXCLAMATION|MB_OK);
					bj=1;
					break;
				}
				if(!Name[0]) {
					MessageBox(hwnd,"Nickname can't be empty!","Error!",MB_ICONEXCLAMATION|MB_OK);
					break;
				}
				if(bj)break;
				WSADATA wsd;
				sockaddr_in ServerAddr;
				ServerAddr.sin_family          =AF_INET;
				ServerAddr.sin_port            =htons(6003);
				ServerAddr.sin_addr.S_un.S_addr=inet_addr(IIIPPP);
				WSAStartup(MAKEWORD(2,2),&wsd);
				Sock=socket(AF_INET,SOCK_STREAM,0);
				if(connect(Sock,(sockaddr*)&ServerAddr,sizeof(sockaddr))==SOCKET_ERROR) {
					MessageBox(NULL,"Connect Error!","Error!",MB_ICONEXCLAMATION|MB_OK);
					break;
				}
				DWORD ls_handle_id;
				CreateThread(NULL,0,receive,(LPVOID)Sock,0,&ls_handle_id);
				send(Sock,"L",1,0);
				send(Sock,Name,15,0);
				break;
			}
			case SEND: {
				int Get=GetWindowText(hwnd_in,cmp,1300);
				int bj=0;
				for(int i=0; i<Get; i++)
					if(cmp[i]!='\n'&&cmp[i]!='\r'&&cmp[i]!=' ') {
						bj=1;
						break;
					}
				if(bj==0) {
					SetWindowText(hwnd_in,"");
					break;
				}
				if(last>clock()) {
					MessageBox(hwnd,"You enter message too fast!","Error!",MB_ICONEXCLAMATION|MB_OK);
					break;
				}
				last=clock()+1000;
				send(Sock,"S",1,0);
				send(Sock,cmp,Get,0);
				SetWindowText(hwnd_in,"");
				break;
			}
			case RC_CLEAR: {
				buf[0]=0;
				SetWindowText(hwnd_out,"");
				break;
			}
			case RC_ABOUT: {
				MessageBox(hwnd,"Software Author : New In Town\nPress Ctrl+Enter to send messages.\nPress Alt+X to hide windows.","About",MB_ICONEXCLAMATION|MB_OK);
				break;
			}
			case RC_EXIT: {
				exit(0);
				break;
			}
			case ADD: {
				nownownow++;
				font=CreateFont(-nownownow*2,-nownownow,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Î¢ÈíÑÅºÚ");
				SendMessage(hwnd_out,WM_SETFONT,(WPARAM)font,(LPARAM)NULL);
				SendMessage(hwnd_in,WM_SETFONT,(WPARAM)font,(LPARAM)NULL);
				InvalidateRect(hwnd_out,0,1);
				InvalidateRect(hwnd_in,0,1);
				UpdateWindow(hwnd_out);
				UpdateWindow(hwnd_in);
				break;
			}
			case DEL: {
				nownownow--;
				if(nownownow<=3)nownownow=3;
				font=CreateFont(-nownownow*2,-nownownow,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"Î¢ÈíÑÅºÚ");
				SendMessage(hwnd_out,WM_SETFONT,(WPARAM)font,(LPARAM)NULL);
				SendMessage(hwnd_in,WM_SETFONT,(WPARAM)font,(LPARAM)NULL);
				InvalidateRect(hwnd_out,0,1);
				InvalidateRect(hwnd_in,0,1);
				UpdateWindow(hwnd_out);
				UpdateWindow(hwnd_in);
				break;
			}
			case RC_SET: {
				if(step==0){
					MessageBox(hwnd,"You must login first!","Error!",MB_ICONEXCLAMATION|MB_OK);
					break;
				}
				send(Sock,"<",1,0);
				step=2;
				work_1(0);
				work_2(1);
				break;
			}
			case BACK: {
				step=1;
				work_2(0);
				work_1(1);
				break;
			}
		}
		return 0;
	} else return DefWindowProc(hwnd,Message,wParam,lParam);
	return 0;
}
int WinMain(HINSTANCE hin,HINSTANCE w,LPSTR lp,int n_cmd) {
	srand(static_cast<unsigned int>(time(0)));
	MSG Msg;
	WNDCLASSEX wc;
	memset(&wc,0,sizeof(wc));
	wc.hIcon        =LoadIcon(NULL,"A");
	wc.hCursor      =LoadCursor(NULL,IDC_ARROW);
	wc.hIconSm      =LoadIcon(hin,"A");
	wc.lpfnWndProc  =WndProc;
	wc.lpszMenuName ="MAINMENU";
	wc.lpszClassName="q";
	wc.cbSize       =sizeof(WNDCLASSEX);
	wc.hInstance    =hin;
	wc.hbrBackground=(HBRUSH)(5);
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL,"Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"q","hack.chat",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,640,480,NULL,NULL,hin,NULL);
	if(hwnd==NULL) {
		MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	ShowWindow(hwnd,n_cmd);
	UpdateWindow(hwnd);
	DWORD Hide;
	CreateThread(NULL,0,hide,(LPVOID)NULL,0,&Hide);
	while(GetMessage(&Msg,NULL,0,0)>0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		if(KEY_DOWN(VK_CONTROL)&&KEY_DOWN(VK_RETURN)) {
			SendMessage(hwnd,WM_COMMAND,(WPARAM)SEND,(LPARAM)NULL);
			Sleep(10);
		}
	}
	return Msg.wParam;
}/*-std=c++11 -lwsock32 -lgdi32*/
