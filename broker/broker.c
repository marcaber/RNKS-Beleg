#include "broker.h"
#include <malloc.h>    /* broker.c */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "data.h"
#include "config.h"
#include <winsock2.h>
#include "brokerSy.h"
#include "topicMgmt.h"

#include <direct.h>
#include <stdlib.h>

#define DEBUG




void fillAnnnouncement(struct MCAnnouncement *req, char *topic, char *mcAddr)
{
	req->header.ReqType = 'M';
	req->header.msgLength = sizeof(struct MCAnnouncement);
	
	strcpy(req->topic, topic);
	strcpy(req->mcAddress, mcAddr);
}


void processMsg(struct genericMsg *pubPtr)
{
	struct MCAnnouncement req;
	struct publishMsg *ptr;
	struct list_entry *listEntry = NULL;

	static struct list_type *listHeadPtr = NULL;
	char mcAddr[IP6_ADDR_LENGTH];

	if (pubPtr == NULL)
		return;

	////Test
	//printf("ReqTyp: %c        ", pubPtr->header.ReqType);
	//ptr = (struct publishMsg *)pubPtr;
	//printf("Topic: %s          ", ptr->topic);
	//printf("Type: %d          ", ptr->payload.payloadType);
	//printf("Float: %g            ", ptr->payload.floatValue);
	//printf("Int: %d              ", ptr->payload.intValue);
	////Testende

	switch ((pubPtr->header).ReqType) {
	case CONNECT:
		printf("GOT A CONNECT MESSAGE\n");
		break;	// we don't support 
	case PUBLISH:
		printf("GOT A PUBLISH MESSAGE\n");
		ptr = (struct publishMsg *)pubPtr;

		printf("in process msg:\n packetId %d \t QoS %d\n", ptr->packetId, ptr->qos);

		if ((ptr->payload).payloadType == 0)  //INTEGER 
			printf("Topic %s \t Int Value %d\n\n", ptr->topic, (ptr->payload).intValue);
		else
			printf("Topic %s \t Float Value %f\n\n", ptr->topic, (ptr->payload).floatValue);



		//Initialize topic-MCAddress-List
		if (!listHeadPtr)
			listHeadPtr = init_list();

		listEntry = find_topic(listHeadPtr, ptr->topic);
		if (listEntry == NULL){
			newMCAddr(mcAddr);
			if ((add_topic(listHeadPtr, ptr->topic, mcAddr)) == 0) //alles GUT!
			{
				fillAnnnouncement(&req, ptr->topic, mcAddr);
				sendUDPMsg((struct genericMsg*)&req, DEFAULT_ANNOUNCEMENT_MC, DEFAULT_SUBSCRIBER_PORT);
				Sleep(5000);

			}
			else
				printf("Add new topic failed\n");
		}
		else
			getAddr(listEntry, mcAddr);

		//now forward publish message towards subscriber mc address 
		sendUDPMsg(pubPtr, mcAddr, DEFAULT_SUBSCRIBER_PORT);
		break;
	case SUBSCRIBE:
		// we don't support yet
		break;

	default:
		fprintf(stderr, "processMsg: Not supported message type\n");

	}
}



int main(int argc, char *argv[]) {
	int i;
	char *Server = NULL;
	char *PortPubl = DEFAULT_BROKER_PUBLISH_PORT;
	char *PortSubs = DEFAULT_BROKER_SUBSCRIBE_PORT;

	//Programmparameter �berpr�fen
	//...

	printf("Subscriber Port: %s  ... Publisher Port: %s\n", PortSubs, PortPubl);

	printf("Broker active and waiting... (^C exits server) \n");

	initServer(Server, PortPubl, PortSubs);  /* Establish Server TCP-Socket (Publish) and UDP Port for subscriber*/

	exitServer();
}
