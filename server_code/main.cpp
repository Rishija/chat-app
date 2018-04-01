#include "../Header.h"

int main(int argc, const char * argv[]) {
    
    int sockfd;
    Socket(sockfd);
    
    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    
    servAddr.sin_family = AF_INET;
    
    if(argc > 2)
        inet_pton(AF_INET, argv[2], &(servAddr.sin_addr));
    else
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    servAddr.sin_port = htons(argc > 1 ? atoi(argv[1]) : PORT);
    
    Bind(sockfd, servAddr);
    
    Listen(sockfd, BACKLOG);
    
    cout << setw(atoi(getenv("COLUMNS"))/2 - 4) << green << bold << "SERVER ON" << regular << setw(0) << endl;
    cout << "IP: " << inet_ntoa(servAddr.sin_addr) << "\t";
    cout << "Port: " << ntohs(servAddr.sin_port) << endl;
    
    return 0;
}
