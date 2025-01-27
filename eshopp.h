struct Product {
    char description[50];
    float price;
    int item_count;
    int total_requests;
    int total_sold;
};

void initialize_catalog();
void handle_client(int client_socket);
