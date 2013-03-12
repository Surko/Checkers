#include <winsock2.h>
#include <iostream>


class Server {
public :
	Server() {

	}
int listenOnPort(SOCKET & s,int port)
{
   WSADATA wsaData;
   
   int error = (WSAStartup(MAKEWORD(2,2), &wsaData));
   // Initialize Winsock version 2.2
   if (error != 0)
   {
	  // Pretoze WinSock zle nacital tak nemozme urcite presnu chybu 
	  // co sa stalo. Spoliehame na navrat pomocou return WSAStartupu.      

      std::printf("WSAStartup failed with error %d\n", error);
      return 0;
   }

   if (wsaData.wVersion != 0x0202) // if bad version
   { 
		WSACleanup (); // unload ws2_32.dll
		return 0;
   }

	// Vytvori novy soket na ktorom pocuva pripojenia.      
 
   SOCKADDR_IN ServerAddr;

   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   if (s == INVALID_SOCKET)
    {
        return 0;
    }

    // Nastavi SOCKADDR strukturu
	  
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);    
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  
    // Asociuje adresove informacie so soketom pouzivajucim bind
	  
	int bind_error = bind(s, (SOCKADDR *)&ServerAddr, 
    sizeof(ServerAddr));
	if (bind_error = SOCKET_ERROR) {
		return 0;
	}

	// Listen for client connections. We used a backlog of 5, which
	// is normal for many applications.

    listen(s, 5); 

	return 1;
}

};