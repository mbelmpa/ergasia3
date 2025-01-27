#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
int main() 
{
 int client_socket;
 struct sockaddr_in server_addr; // Domh gia thn dieuthunsh tou server
 int product_id;//to ID tou proiontos pou tha zhthsei o client 
 char buffer[1024]; // Buffer gia thn apanthsh tou server
 if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Dhmiourgia socket gia ton client
 {
  perror("Socket creation failed");
  exit(EXIT_FAILURE);
 }
 server_addr.sin_family = AF_INET; // Orismos twn xarakthristikwn ths sundeshs me ton server
 server_addr.sin_port = htons(PORT);
 if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) // Metatroph ths dieuthunshs tou server apo string se binary morfh
 {
  perror("Invalid address");
  close(client_socket);
  exit(EXIT_FAILURE);
 }
 if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) // Sundesh tou client me ton server
 {
  perror("Connection failed");
  close(client_socket);
  exit(EXIT_FAILURE);
 }
 printf("Connected to the server.\n");
 for (int i = 0; i < 10; i++)  // Stelnei 10 aitimata gia paraggelies proiontwn ston server
 {
  product_id = rand() % 20; // tuxaio proion anamesa se 0 se 19
  write(client_socket, &product_id, sizeof(product_id));  // Grafei to ID tou proiontos sto socket gia apostolh ston server
  read(client_socket, buffer, sizeof(buffer)); // Diavazei thn apanthsh tou server gia thn paraggelia
  printf("Server response: %s\n", buffer);
  sleep(1); // xronokatherhsh
 }
 close(client_socket); //kleinei to socket tou client
 printf("Client disconnected.\n");
 return 0;
}
