## Running the program Locally

To create executable named `fileserver`, run the following command:
`g++ -std=c++17 -o fileserver main.cpp -L<path_to_openssl>/lib -I<path_to_openssl>/include -lssl -lcrypto`
for macOS it might be: `/usr/local/opt/openssl@3`

- To execute the program, enter `./fileserver keyfile_name` in the cli.

- You can even run it using Docker
  1. docker build -t fileserver .
  2. docker run -it --name fileserver-container fileserver:latest /bin/bash