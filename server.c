#include "server.h"
#include "list.h"

int chat_serv_sock_fd; //server socket

/////////////////////////////////////////////
// USE THESE LOCKS AND COUNTER TO SYNCHRONIZE

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // mutex lock
pthread_mutex_t rw_lock = PTHREAD_MUTEX_INITIALIZER;  // read/write lock

/////////////////////////////////////////////


char const *server_MOTD = "Thanks for connecting to the BisonChat Server.\n\nchat>";

struct node *head = NULL;
struct room *roomList = NULL;

void sigintHandler(int sig_num) {
  printf("\nShutting down server...\n");
  CLOSE_SOCKET(chat_serv_sock_fd);
  exit(0);
}

int get_server_socket() {
  int sockfd;
  struct sockaddr_in server_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == SOCKET_ERROR_CHECK) {
    perror("Socket creation failed");
    return SOCKET_ERROR_CHECK;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR_CHECK) {
    perror("Bind failed");
    return SOCKET_ERROR_CHECK;
  }

  return 0;
}

int accept_client(int serv_sock) {
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int client_socket = accept(serv_sock, (struct sockaddr *)&client_addr, &client_len);

  if (client_socket == SOCKET_ERROR_CHECK) {
    perror("Client accept failed");
    return SOCKET_ERROR_CHECK;
  }

  printf("New client connected\n");
  return client_socket;
}

int main() {
  signal(SIGINT, sigintHandler);

  roomList = createRoom("Lobby");

  chat_serv_sock_fd = get_server_socket();
  if(chat_serv_sock_fd == SOCKET_ERROR_CHECK) {
    return 1;
  }

  if (start_server(chat_serv_sock_fd, BACKLOG) == SOCKET_ERROR_CHECK) {
    return 1;
  }

  printf("Server running on PORT: %d\n", PORT);

  while (1) {
    int new_client = accept_client(chat_serv_sock_fd);
    if (new_client != SOCKET_ERROR_CHECK) {
      pthread_t new_client_thread;
      pthread_create(&new_client_thread, NULL, client_receive, (void *)&new_client);
    }
  }

  return 0;
}
