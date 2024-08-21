#include <iostream>
#include <fstream>
#include <string.h>
#include "sha1.h"
#include "chat.h"

void throwWhere(const std::string& text) {
    std::cout << "Throw text: " << text.c_str() << std::endl;
    throw 1;
}

void RegUsers(Chat chat)
{
    ifstream file;
    try
    {
        file.open("text.txt");
        // Base case
        if (file.fail())
        {
            throwWhere("in RegUsers |ERROR: Can not open the file!");
            exit(0);
        }
    }
    catch (...) {
    }
    // Reading file
    string login, password;
    
    while (!file.eof()) {
        // Get data
        file >> login >> password;
        // Converting string to char array 
        const char* _login = login.c_str();
        const char* _password = password.c_str();
        // Reg
        chat.reg((char*)_login, (char*)_password, sizeof(password));
    }
    file.close();
}


int main()
{

    bool flag = false;
    setlocale(LC_CTYPE, "rus");
    try {
        Chat chat;

        RegUsers(chat);
        chat.unreg((char*)"userUnreg");
        // Succesful login attempt
        if (chat.login((char*)"user1", (char*)"as", sizeof("user1")))  throwWhere("in c.login() user1 |ERROR: Fail login attempt!");
        // Fail login attempt
        if (!chat.login((char*)"user2", (char*)"a1s", sizeof("user2")))  throwWhere("in c.login() user2 |ERROR: Fail login attempt!");
        
        flag = true;
    }
    catch (...) {
        flag = false;
    }

    return 0;
}
