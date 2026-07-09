# My First TCP Socket Server in C++

This is a raw, low-level network socket server written in C++. I built this to get my hands dirty with backend networking layers and to see how data actually moves through an operating system's network stack without relying on modern framework magic.

Heads Up: This is purely a learning project. It is an experimental sandbox, meaning it is definitely not production-ready, it has hardcoded values, and you will probably find some bugs in it while I am still figuring things out.

---

## How It Works Under the Hood

* Simultaneous Two-Way Traffic (Full-Duplex): It uses standard stream sockets (SOCK_STREAM), meaning both the server and the client can send and receive data at the exact same time without locking each other out.
* Clearing Up the Port: If you have ever tried restarting a network script, you know the OS likes to lock the port for a few minutes (the TIME_WAIT cooldown). I used setsockopt() with the SO_REUSEADDR flag to force the OS to free up the port instantly so the server can reboot without throwing an "Address already in use" error.
* Hardcoded for Local Testing:
  * Locked to port 8080.
  * Tied directly to the loopback IP (127.0.0.1), meaning it runs locally on your own machine right out of the box.

### Talking to Other Computers
Even though it is set up for local testing right now, the logic is ready for cross-device networking. If you want to use this code to connect two separate computers over the same Wi-Fi network:
1. Find the actual local network IP of the hosting computer (something like 192.168.1.5).
2. Swap out 127.0.0.1 in the code with that new hosting IP.
3. Make sure your firewall is not blocking traffic on port 8080, and you are good to go.

---

## How to Run It

You need a standard C++ compiler like g++ on Linux, Mac, or Windows WSL.

```bash
# Compile the C++ code

# Compile the client code
g++ client.cpp -o my_client

# Run the client to connect to the server
./my_client
g++ server.cpp -o my_server

# Run the server
./my_server
