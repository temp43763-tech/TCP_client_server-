
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
using namespace std;

mutex log_mutex;
void readerthread(int client_socket)
{
    
    while (true)
    {
        /* code */
        
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read < 0)
        {
            cout << " nothing said from the client \n";
            break;
        }
        else if (bytes_read > 0)
        {
            lock_guard<mutex> ol(log_mutex);
            cout << "[client] " << buffer << endl;
        }
    }
}
void workerthread(int client_socket)
{

    while (true)
    {
        /* code */
        thread t2(readerthread, client_socket);
        t2.detach();

        string server_msg;
        getline(cin, server_msg);

        cout << "[server] ";
        send(client_socket, server_msg.c_str(), server_msg.length(), 0);
        cout << endl;
    }
}

int main()
{

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in envelop;
    envelop.sin_family = AF_INET;
    envelop.sin_port = htons(8080);
    envelop.sin_addr.s_addr = INADDR_ANY;
    int enable_switch = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable_switch, sizeof(enable_switch));

    int bind_to_port = bind(server_socket, (sockaddr *)&envelop, sizeof(envelop));

    if (bind_to_port < 0)
    {
        cout << "can not bind to port \n";
        close(server_socket);
        return 1;
    }
    cout << " connected to port \n";

    if (listen(server_socket, 3) < 0)
    {
        cout << " can not listen to client \n ";
        close(server_socket);
        return 1;
    }
    cout << "listening....\n";
    struct sockaddr_in client_info;
    socklen_t len = sizeof(client_info);

    while (true)
    {
        /* code */

        int client_socket = accept(server_socket, (sockaddr *)&client_info, &len);

        if (client_socket < 0)
        {
            cout << "client socket failed\n";
            continue;
        }
        thread t1(workerthread, client_socket);
        t1.detach();
    }

    return 0;
};
