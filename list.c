#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

//insert link at the first location
struct node* insertFirstU(struct node *head, int socket, char *username) {
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  strcpy(newNode->username, username);
  newNode->socket = socket;
  newNode->next = head;
  return newNode;
}
//find a link with given user
struct node* findU(struct node *head, char* username) {

   //start from the first link
   struct node *current = head;
   while (current != NULL) {
    if (strcmp(current->username, username) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

struct node* removeUser(struct node *head, char *username) {
  struct node *current = had;
  struct node *previous = NULL;

  while (current != NULL) {
    if (strcmp(current->username, username) == 0) {
      if(previous == NULL) {
        head = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      return head;
    }
    previous = current;
    current = current->next;
  }
  return head;
}

void listUsers(struct node *head, char *output) {
  struct node *current = head;
  while (current != NULL) {
    strcat(output, current->username);
    strcat(output, "\n");
    current = current->next;
  }
}
   
struct room* createRoom(const char *roomName) {
  struct room *newRoom = (struct room *)malloc(sizeof(struct room));
  strcpy(newRoom->roomName, roomName);
  newRoom->userList = NULL;
  newRoom->next = NULL;
  return newRoom; 
}

struct room* findRoom(struct room *head, char *roomName) {
  struct room *current = head;
  while (current != NULL) {
    if (strcmp(current->roomName, roomName) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

bool addUserToRoom(struct room *roomList, char *roomName, char *username) {
  struct room *room = findRoom(roomList, roomName);
  if (room == NULL) {
    return false;
  }
  room->userList = insertFirstU(room->userList, -1, username);
  return true;
}

bool removeUserFromRoom(struct room *roomList, char *roomName, char *username) {
  struct room *room = findRoom(roomList, roomName);
  if (room == NULL) {
    return false;
  }
  room->userList = removeUser(room->userList, username);
  return true;
}

void listRooms(struct room *roomList, char *output) {
  struct room *current = roomList;
  while (current != NULL) {
    strcat(output, current->roomName);
    strcat(output, "\n");
    current = current->next;
  }
}

void updateUserInRooms(struct room *roomList, char *oldUsername, char *newUsername) {
  struct room *currentRoom = roomList;
  while (currentRoom != NULL) {
    struct node *currentUser = currentRoom->userList;
    while (currentUser != NULL) {
      if (strcmp(currentUser->username, oldUsername) == 0) {
        strcpy(currentUser->username, newUsername);
      }
      currentUser = currentUser->next;
    }
    currentRoom = currentRoom->next;
  }
}

void removeUserFromAllRooms(struct room *roomList, char *username) {
  struct room *currentRoom = roomList;
  while (currentRoom != NULL) {
    currentRoom->userList = removeUser(currentRoom->userList, username);
    currentRoom = currentRoom->next;
  }
}
