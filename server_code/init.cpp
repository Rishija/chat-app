#include "../Header.h"

void Bind(int &sockFD, sockaddr_in &servAddr) {

    const int temp = 1;
    if(setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) < 0)
        print_error("Set sock option Error");

    if(::bind(sockFD, (sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        print_error("Bind Error");
}

void Listen(int sockFD, int backlog) {

    if(listen(sockFD, backlog) < 0)
        print_error("Listen Error");
}


void start_server(int &sockFD, sockaddr_in &servAddr, int argc, const char * argv[]) {

    Socket(sockFD);
    bzero(&servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;

    if(argc > 2)
        inet_pton(AF_INET, argv[2], &(servAddr.sin_addr));
    else
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    servAddr.sin_port = htons(argc > 1 ? atoi(argv[1]) : PORT);

    Bind(sockFD, servAddr);

    Listen(sockFD, BACKLOG);

    cout << setw(atoi(getenv("COLUMNS"))/2 - 4) << green << bold << "SERVER ON" << regular << setw(0) << endl;
    cout << "IP: " << inet_ntoa(servAddr.sin_addr) << "\t";
    // cout << "Port: " << servAddr.sin_port<< "\t";
    cout << "Port: " << ntohs(servAddr.sin_port) << endl << endl;
}
