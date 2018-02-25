/*
 * mygbn.h
 */

#ifndef __mygbn_h__
#define __mygbn_h__

#define MAX_PAYLOAD_SIZE 512
#include <errno.h>
#include <sys/socket.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
struct MYGBN_Packet {
  unsigned char protocol[3];                  /* protocol string (3 bytes) "gbn" */
  unsigned char type;                         /* type (1 byte) */
  unsigned int seqNum;                        /* sequence number (4 bytes) */
  unsigned int length;                        /* length(header+payload) (4 bytes) */
  unsigned char payload[MAX_PAYLOAD_SIZE];    /* payload data */
};

struct mygbn_sender {
  int sd; // GBN sender socket
  // ... other member variables
  struct sockaddr_in server_addrb;
  struct sockaddr server_addr;
  char* ip; 
  int port; 
  int N; 
  int timeout;
//int sendn;
};

void mygbn_init_sender(struct mygbn_sender* mygbn_sender, char* ip, int port, int N, int timeout);
int mygbn_send(struct mygbn_sender* mygbn_sender, unsigned char* buf, int len);
void mygbn_close_sender(struct mygbn_sender* mygbn_sender);

struct mygbn_receiver {
  int sd; // GBN receiver socket
  // ... other member variables
  struct sockaddr_in server_addrb;
  struct sockaddr server_addr;
  int port;
 // int recvn;
};

void mygbn_init_receiver(struct mygbn_receiver* mygbn_receiver, int port);
int mygbn_recv(struct mygbn_receiver* mygbn_receiver, unsigned char* buf, int len);
void mygbn_close_receiver(struct mygbn_receiver* mygbn_receiver);
int sendn(int sd, void *buf, int buf_len);//mylam
int recvn(int sd, void *buf, int buf_len);//mylam

#endif
