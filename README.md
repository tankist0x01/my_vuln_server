# my_vuln_server
Just another damn vulnerable c server
### Build
We need to disable some security mechanisms to demonstrate basic exploitation
```
gcc -o my_vuln_server my_vuln_server.c -fno-stack-protector -z execstack -no-pie
```
