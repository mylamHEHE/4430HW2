#include "mygbn.h"

void mygbn_init_sender(struct mygbn_sender* mygbn_sender, char* ip, int port, int N, int timeout){
	struct sockaddr_in server_addrb;
	memset(&server_addrb, 0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	struct sockaddr server_addr;
	server_addr.sa_data = ip;
	server_addr.sa_family = AF_INET;

	mygbn_sender->sd = socket(AF_INET, SOCK_DGRAM, 0);
	mygbn_sender->ip = ip;
	mygbn_sender->port = port;
	mygbn_sender->N = N;
	mygbn_sender->timeout = timeout;

}

int mygbn_send(struct mygbn_sender* mygbn_sender, unsigned char* buf, int len)
{
	int n_left = len;
	int n;
	while(n_left > 0)
	{
		if(n = sendto(mygbn_sender->sd, buf + (len - n_left), 
			n_left, 0, (struct sockaddr *)mygbn_sender->server_addr,
			sizeof(mygbn_sender->server_addr)) <0)
		{
			if(errno == EINTR)
				n = 0;
			else
				return -1;
		}
		else if(n == 0){
			return 0;
		}
		n_left -= n;
	}
	return buf;
}

void mygbn_close_sender(struct mygbn_sender* mygbn_sender){
	mygbn_sender->sd = 0;
	mygbn_sender->ip = NULL;
	mygbn_sender->port = 0;
	mygbn_sender->N = 0;
	mygbn_sender->timeout = 0;
}

void mygbn_init_receiver(struct mygbn_receiver* mygbn_receiver, int port){
	struct sockaddr_in server_addrb;

	memset(&server_addrb, 0,sizeof(server_addrb));
	server_addrb.sin_family = AF_INET;
	server_addrb.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addrb.sin_port = htons(port);
	socklen_t addrLen = sizeof(server_addrb);

	struct sockaddr server_addr;
	server_addr.sa_data = *((struct server_addr*)mygbn_receiver->server_addr);
	server_addr.sa_family = AF_INET;

	if(bind(mygbn_receiver->sd,(struct sockaddr *)&(mygbn_receiver->server_addrb),
		sizeof(mygbn_receiver->server_addr))<0){
		printf("bind error: %s (Errno:%d)\n", strerror(errno),errno);
		exit(0);
	}
	mygbn_receiver->sd = socket(AF_INET, SOCK_DGRAM, 0);
	mygbn_receiver->port = port;
}

int mygbn_recv(struct mygbn_receiver* mygbn_receiver, unsigned char* buf, int len)
{
	int n_left = len;
	int n;
	while(n_left > 0)
	{
		if(n = recvfrom(mygbn_receiver->sd, buf + (len - n_left),
			 n_left, 0, (struct sockaddr *)mygbn_receiver->server_addr,
			sizeof(mygbn_receiver->server_addr)) <0)
		{
			if(errno == EINTR)
				n = 0;
			else
				return -1;
		}
		else if(n == 0){
			return 0;
		}
		n_left -= n;
	}
	return buf;
}

void mygbn_close_receiver(struct mygbn_receiver* mygbn_receiver) {
	mygbn_receiver->sd = 0;
	mygbn_receiver->port = 0;
}
