#pragma once
#include <winsock2.h>

extern LPSTR PrintError(int ErrorCode);

extern int add_client(SOCKET sock, int sockType);

extern int remove_client(SOCKET sock);

extern unsigned int fill_set(fd_set *fds);

extern SOCKET get_sender(fd_set *fds);

extern void newMCAddr(char *lastMCAddr);

extern void sendUDPMsg(struct genericMsg* annMsg, char *mcGroupAddr, char *port);

extern int initServer(char *Server, char *PublisherPort, char *SubscriberPort);

extern int exitServer();