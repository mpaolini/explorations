#include <stdio.h>
#include <netinet/in.h>
#include <ev.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>

#define PORT_NO 8000
#define BUFFER_SIZE 1024

const char *resp = "HTTP/1.0 200 OK\r\nContent-Length: 5\r\n\r\nPong!\r\n";

int total_clients = 0;  // Total number of connected clients

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
int setnonblock(int fd);

int main()
{
  struct ev_loop *loop = ev_default_loop(0);
  int sd;
  struct sockaddr_in addr;
  int addr_len = sizeof(addr);
  struct ev_io w_accept;
  int true = 1;
  // Create server socket
  if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
    {
      perror("socket error");
      return -1;
    }
  if ( setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) )
    perror("reuse addr");
  if ( setnonblock(sd) )
    perror("no block");
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT_NO);
  addr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket to address
  if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
      perror("bind error");
      return -1;
    }

  // Start listing on the socket
  if (listen(sd, 2) < 0)
    {
      perror("listen error");
      return -1;
    }

  // Initialize and start a watcher to accepts client requests
  ev_io_init(&w_accept, accept_cb, sd, EV_READ);
  ev_io_start(loop, &w_accept);

  // Start infinite loop
  while (1)
    {
      ev_loop(loop, 0);
    }

  return 0;
}

/* Accept client requests */
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int client_sd;
  struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

  if(EV_ERROR & revents)
    {
      perror("got invalid event");
      return;
    }

  // Accept client request
  client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

  if (client_sd < 0)
    {
      perror("accept error");
      return;
    }

  if ( setnonblock(client_sd) )
    perror("no block");

  total_clients ++; // Increment total_clients count

  // Initialize and start watcher to read client requests
  ev_io_init(w_client, read_cb, client_sd, EV_READ);
  ev_io_start(loop, w_client);
}

/* Read client message */
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
  char buffer[BUFFER_SIZE];
  ssize_t read;
  struct ev_io w_write;

  if(EV_ERROR & revents)
    {
      perror("got invalid event");
      return;
    }
  // Receive request from client socket
  do {
    read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);
  } while (read == BUFFER_SIZE);
  ev_io_stop(loop, watcher);
  // Start response
  ev_io_init(&w_write, write_cb, watcher->fd, EV_WRITE);
  ev_io_start(loop, &w_write);

  //free(watcher);
  total_clients --; // Decrement total_clients count
}

/* send response */
void write_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
  int sent = 0;
  if (!(revents & EV_WRITE)) {
    ev_io_stop(EV_A_ watcher);
    return;
  }
  if (send(watcher->fd, resp, (int) strlen(resp), 0) < strlen(resp)) {
    perror("send failed");
  }
  ev_io_stop(loop, watcher);
  close(watcher->fd);
}

int setnonblock(int fd)
{
  int flags;

  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);
}
