from io import BytesIO  
import gzip

flag = "NOVEK{S1MPL1C1TY_1S_THE_SOUL_OF_EFF1C1ENCY}"

secret = "The best performance improvement is the transition from the nonworking state to the working state."

out = BytesIO()
with gzip.GzipFile(fileobj=out, mode="w") as f:
	f.write(secret.encode())

compressed_secret = out.getvalue()

desc = '''One of our engineers has been trying to decompress
a gzip string for a month now, can you help him? 
Below is the hex encoded compressed string\n'''

print("Hello Padawan!\n")
print(desc)
print(compressed_secret.hex())

decompress_secret = input("Secret:")
if decompress_secret  == secret:
    print("\nNice work padawan, here is you flag: %s"%flag)
else:
    print("\nUmmh i dont think that's it")
