import socket
import select
import network
#from phew import server, connect_to_wifi

def start_server():
    # Create a TCP/IP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 12345))
    server_socket.listen(5)
    print("Server listening on port 12345")

    # List of sockets for select to monitor
    sockets_list = [server_socket]
    
    # Dictionary to map client sockets to addresses
    clients = {}  

    while True:
        # Wait for any socket to be ready for processing
        read_sockets, _, exception_sockets = select.select(sockets_list, [], sockets_list)

        for notified_socket in read_sockets:
            # If notified socket is the server socket, accept new connection
            if notified_socket == server_socket:
                client_socket, client_address = server_socket.accept()
                sockets_list.append(client_socket)
                print(f"Accepted new connection from {client_address}")
                clients[client_socket] = client_address
            else:
                # Receive data from existing connection
                data = notified_socket.recv(1024)
                if data:
                    print(f"Received: {data.decode()} from {clients[notified_socket]}")
                    notified_socket.send(data)  # Echo back the received data
                else:
                    # Handle client disconnect
                    print(f"Connection closed from {clients[notified_socket]}")
                    sockets_list.remove(notified_socket)
                    notified_socket.close()

        for notified_socket in exception_sockets:
            # Handle exceptional conditions
            sockets_list.remove(notified_socket)
            notified_socket.close()

if __name__ == "__main__":
    #Setup Wifi
    SSID = "YOUR SSID"
    PASS = "YOUR PASS"

    #ip = connect_to_wifi(SSID, PASS)
    #print("connected to IP ", ip)
    
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(SSID,PASS)
    while wlan.isconnected() == False:
        print('Waiting for connection...')
        sleep(1)
    print(wlan.ifconfig())
    
    
    
    start_server()