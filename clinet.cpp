


#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <thread> 

using namespace std;

void receive_messages(int socket_fd) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int revf = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (revf > 0) {
            buffer[revf] = '\0';
            cout << "\n[Server]: " << buffer << "\n";
            cout << "[You]: " << flush; 
        } else {
            cout << "\n[System] Disconnected from server.\n";
            break;
        }
    }
}

int main() {
    int a = socket(AF_INET, SOCK_STREAM, 0);
  

    struct sockaddr_in envelop;
    envelop.sin_family = AF_INET;
    envelop.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &envelop.sin_addr);
    
    if (connect(a, (sockaddr*)&envelop, sizeof(envelop)) < 0) {
        cout << "Connection failed\n";
        return 1;
    }
    cout << "Connected to server! Start typing below.\n";
    thread reader(receive_messages, a);
    reader.detach();
    string user_input;
    while (true) {
        cout << "[You]: ";
        getline(cin, user_input);

        if (user_input == "exit") {
            break;
        }

        if (!user_input.empty()) {
            send(a, user_input.c_str(), user_input.length(), 0);
        }
    }

    close(a);
    return 0;
}