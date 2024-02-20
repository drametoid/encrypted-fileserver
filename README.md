# encrypted-fileserver
Building an encrypted fileserver for learnings on Secure System Design

# Running Locally
For now, use your local C++ toolchain (g++) in my case:
`g++ -std=c++17 -o fileserver main.cpp -L/usr/local/opt/openssl@3/lib -I/usr/local/opt/openssl@3/include -lssl -lcrypto`
Replace `/usr/local/opt/openssl@3` with the path of your devel openssl package in your system.