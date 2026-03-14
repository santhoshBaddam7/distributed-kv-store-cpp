import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    s.connect(("localhost", 6380))
    print("Connected to server")
except Exception as e:
    print("Connection failed:", e)
    exit()

while True:
    cmd = input("Enter command: ")

    if cmd.lower() == "exit":
        break

    s.send((cmd + "\n").encode())

    data = s.recv(1024)

    print("Server:", data.decode())

s.close()