import  socket
import  threading

server_addr = "127.0.0.1"
server_port = 11111


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((server_addr, server_port))

server.listen(100)
print "listen: %s:%d" %  (server_addr,server_port)



def handle_client(sock_client):
    request = sock_client.recv(1024)
    print "Recv: %s" % request

    sock_client.send("ACK!")
    sock_client.close()


while True:
    client, addr = server.accept()
    print "accept:\nconnection from: %s:%d" % (addr[0], addr[1])

    client_handler = threading.Thread(target=handle_client, args=(client, ))
    client_handler.start()


