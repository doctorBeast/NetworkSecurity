import  socket

target_addr = "127.0.0.1"
target_port = "80"

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.sendto("AAAAA", (target_addr,target_port))

data, addr = client.recvfrom(4096)

print data
