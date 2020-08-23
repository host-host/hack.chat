#include<windows.h>
#include<bits/stdc++.h>
using namespace std;
const char Server_IP[]="172.16.14.190";//ÄãµÄÄÚÍøIP 
struct point {
	string name;
	SOCKET con;
};
#define po(name,con) (point){name,con}
vector<point>v;
DWORD WINAPI child_thread(LPVOID V_sock) {
	SOCKET con=(SOCKET)V_sock;
	char rev[1500],tmp1[15];
	int time=-1000;
	while(1) {
		int y=recv(con,rev,1,0);
		if(y<=0) {
			for(int i=0; i<(int)v.size(); i++)if(v[i].name==tmp1)v.erase(v.begin()+i);
			string msg=(string)"&["+tmp1+" left]";
			const char *c=msg.c_str();
			for(int i=0; i<(int)v.size(); i++)send(v[i].con,c,msg.size()+1,0);
			return 0;
		}
		rev[y]=0;
		char cmp=rev[0];
		if(cmp=='L') {
			recv(con,tmp1,15,0);
			int bj=0;
			for(int i=0; i<(int)v.size(); i++)
				if(v[i].name==tmp1) {
					send(con,"!Nickname taken!",15,0);
					bj=1;
					break;
				}
			if(bj)continue;
			send(con,"L",2,0);
			v.push_back(po(tmp1,con));
			string msg=(string)"&["+tmp1+" joined]";
			const char *c=msg.c_str();
			for(int i=0; i<(int)v.size(); i++)send(v[i].con,c,msg.size()+1,0);
		}
		if(cmp=='S') {
			int y=recv(con,rev,1400,0);
			rev[y]=0;
			string msg=(string)"&["+tmp1+"]:"+rev;
			const char *c=msg.c_str();
			for(int i=0; i<(int)v.size(); i++)send(v[i].con,c,msg.size()+1,0);
		}
		if(cmp=='<') {
			if(time>clock())continue;
			time=clock()+1000;
			send(con,".",1,0);
			for(int i=0; i<(int)v.size(); i++) {
				string q=">"+v[i].name;
				send(con,q.c_str(),q.size(),0);
			}
		}
		if(cmp=='K')for(int i=0; i<(int)v.size(); i++)if(v[i].con!=con)send(v[i].con,"K",1,0);
	}
}
int main() {
	WSADATA wsd;
	WSAStartup(MAKEWORD(2,2),&wsd);
	SOCKET SockServer;
	sockaddr_in ServerAddr,FromAddr;
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(6003);
	ServerAddr.sin_addr.S_un.S_addr=inet_addr(Server_IP);
	SockServer=socket(AF_INET,SOCK_STREAM,0);
	if(bind(SockServer,(sockaddr*)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)return 1;
	if(listen(SockServer,20)==SOCKET_ERROR)return 2;
	int Socklen=sizeof(sockaddr);
	while(1) {
		SOCKET SockFrom=accept(SockServer,(sockaddr*)&FromAddr,&Socklen);
		if(SockFrom!=INVALID_SOCKET) {
			DWORD ls_handle_id;
			CreateThread(NULL,0,child_thread,(LPVOID)SockFrom,0,&ls_handle_id);
		}
	}
	return 0;
}
