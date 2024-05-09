import os

key = "h4cker_v4illant_ri3n_d1mposs1ble"
path = input("Enter the path to the directory: ")
for filename in os.listdir(path):
    if filename.endswith(".crypt"):
        filePath = os.path.join(path, filename)
        decryptedFilePath = os.path.join(path, filename[:-6])
        with open(filePath, "rb") as cryptedFile, open(decryptedFilePath, "wb") as decryptedFile:
            i = 0
            while True:
                buffer = cryptedFile.read(1)
                if not buffer:
                    break
                decryptedFile.write(bytes([buffer[0] ^ ord(key[i])]))
                i = (i + 1) % len(key)
            os.remove(filePath)