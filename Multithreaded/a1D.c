//159335 Assignment 1
//NAME: PeiQi Jin, ID: 12207549

#include <stdio.h>
#include <winsock.h>
#include <string.h>
#include <windows.h>
// Define the port number to identify this process
#define MYPORT 3490

// thread function
unsigned long CALLBACK Send(void *argument){
	unsigned fd2=(unsigned) argument;
	int n;
	char *header="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
   char data[512];
   char filename[256] = "index.html";
   FILE *f;
	long fsize;
    
	n=recv(fd2,data,512,0);              // recieve the request using fd
	data[n]=0;                          // NUL terminate it
	sscanf(data,"GET /%s ",filename);   // get the name of the file
	f=fopen(filename,"rb");		         // open the file (might be binary)
	send(fd2,header,strlen(header),0);   // send the header
   
	if(f==NULL){
		//printf("File does not exist!\n");
	}else{
		fseek(f,0,SEEK_END);
		fsize = ftell(f);
		rewind(f);
		char buffer[fsize];
		fread(buffer, 1,fsize,f);
		send(fd2,buffer,fsize,0);
		//check size of image
		//printf("Complete! size is %d\n",fsize);
		Sleep(100);
	}
	fclose(f);                          // close the file
	closesocket(fd2);                    // close the socket

	return 0;
}


int main() {
	int s;
	unsigned fd;
   struct sockaddr_in my_addr;
   WSADATA wsaData;
   // Initialize windos sockets
   WSAStartup(MAKEWORD(1, 1), &wsaData);
 
   // Construct address information
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons(MYPORT);
   my_addr.sin_addr.s_addr = INADDR_ANY; 
   memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero) );

   // Create a socket and bind it the port MYPORT
   s=socket(PF_INET,SOCK_STREAM, 0);
   bind(s, (struct sockaddr *)&my_addr, sizeof(my_addr));

   // Allow up to 10 incoming connections
   listen(s,10);
	

	unsigned long id;
	int num=1;
   while(1) {
		fd = accept(s,NULL,NULL);           // wait for a request
		//check fd is negative
		if(fd>0){
		//check how many thread
		//printf("CreateThread %d\n",num);
		num++;
		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Send,(void*)fd,0,&id);
		}
   }
}
