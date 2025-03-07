#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXPRODUCTS 20
#define MAXCLIENTS 5

struct Product //domh proiontos
{
 char description[50];
 float price;
 int item_count;
 int total_requests;
 int total_sold;
};

//katalogos proiontwn
struct Product catalog[MAXPRODUCTS];

void initialize_catalog() //sinartisi gia arxikopoihsh katalogou proionton
{
 for (int i = 0; i < MAXPRODUCTS; i++) 
 {
  printf(catalog[i].description, sizeof(catalog[i].description), "Product %d", i + 1); //dimiourgia perigrafhs proiontos
  catalog[i].price = (i + 1) * 10.0;
  catalog[i].item_count = 2; //2 temaxia gia kathe proion
  catalog[i].total_requests = 0;
  catalog[i].total_sold = 0;
 }
}

void handle_client(int client_socket) //sinartisi eksipiretisis pelatwn
{
 char buffer[1024]; //buffer epikoinwnias
 int product_id;
 while (read(client_socket, &product_id, sizeof(product_id)) > 0) 
 {
  if (product_id < 0 || product_id >= MAXPRODUCTS) //elegxos an to product_id einai egkyro
  {
   printf(buffer, sizeof(buffer), "Invalid product ID.\n"); //minima lathous
  }
  else
  {
   catalog[product_id].total_requests++; // aukshsh arithmou aithmatwn
   if (catalog[product_id].item_count > 0)
   {
    catalog[product_id].item_count--; //meiwsh pososthtas
    catalog[product_id].total_sold++; //aukshsh pwlhsewn
    printf(buffer, sizeof(buffer), "Order successful. Product: %s, Price: %.2f\n",
    catalog[product_id].description, catalog[product_id].price); //epituxia paraggelias
   }
   else
   {
    printf(buffer, sizeof(buffer), "Product out of stock: %s\n",
    catalog[product_id].description); // eksantlhsh proiontos
   }
  } 
  write(client_socket, buffer, strlen(buffer));
  sleep(1); // xronokathisterhsh
 }
 close(client_socket);
 exit(0); // termatismos diergasias pelath
}

int main() 
{
 int server_fd, client_socket; //metavlhtes twn sockets
 struct sockaddr_in server_addr, client_addr; //domes gia dieuthinseis server kai client
 socklen_t addr_len = sizeof(client_addr);
 initialize_catalog();
 if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) //dimiourgia socket
 {
  perror("Socket failed");
  exit(EXIT_FAILURE);
 }
 server_addr.sin_family = AF_INET;
 server_addr.sin_addr.s_addr = INADDR_ANY; //rythmish dieythinshs server
 server_addr.sin_port = htons(PORT);
 if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) //desmeush socket
 {
  perror("Bind failed");
  close(server_fd);
  exit(EXIT_FAILURE);
 }
 if (listen(server_fd, MAXCLIENTS) < 0) //enarksi akroashs sundesewn
 {
  perror("Listen failed");
  close(server_fd);
  exit(EXIT_FAILURE);
 }
 printf("Server is running on port %d...\n", PORT);
 while ((client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) >= 0) //vroxos gia apodoxh sundesewn
 {
  pid_t pid = fork();
  if (pid < 0) 
  {
   perror("Fork failed");
   close(client_socket);
  } 
  else if (pid == 0) 
  {
  // Child process handles the client
   close(server_fd); // kleisimo tou server socket sth diergasia paidiou
   handle_client(client_socket);
  } 
  else 
  {
  // Parent process sinexizei na dexetai sindeseis
   close(client_socket);
  }
 }
 if (client_socket < 0) //elegxos apotyxias apodoxhs sundeshs
 { 
  perror("Accept failed");
  close(server_fd);
 }
 return 0;
}
