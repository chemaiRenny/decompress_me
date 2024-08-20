'''
$python solution.py
'''

import sys, socket, gzip

HOST = "51.11.50.70"
PORT = 5050

ADDR = (HOST, PORT)

#tcp socket
print("[+] opening socket...")
sock_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

print("[+] connecting to %s on port %d"%ADDR)
sock_fd.connect(ADDR)

print("[+] Receiving data from server")
data_size = 349
count = 0
data = b""
while count < data_size:
    chunk = sock_fd.recv(1024)
    if not chunk:
        print("[-] Error receiving data from server")
        sys.exit()

    print("[+] received %d bytes from server"%len(chunk))
    data += chunk
    count += len(chunk)

data = data.decode("UTF-8")

print("\n++++++++++ RECEIVED DATA +++++++++")
print(data)
print("---------- RECEIVED DATA ---------\n")

hex_string = data.split("\n")[-2]
print("[+] Hex: %s"%hex_string)

print("[+] decoding hex to bytes")
byte_array = bytes.fromhex(hex_string)

print("[+] decompressing bytes")
decompress_str = gzip.decompress(byte_array).decode("UTF-8")
print("[+] decompressed string:%s"%decompress_str)

print("[+] Sending response to server")
decompress_str += "\n"
sock_fd.send(decompress_str.encode())

print("[+] Receiving data from server")
data_size = 349
data = b""

while True:
    chunk = sock_fd.recv(1024)
    if not chunk:
        print("[+] Server closed connection")
        break
    print("[+] received %d bytes from server"%len(chunk))
    data += chunk

data = data.decode("UTF-8")

print("\n++++++++++ RECEIVED DATA +++++++++")
print(data)
print("---------- RECEIVED DATA ---------\n")


sock_fd.close()