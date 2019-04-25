// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright 2018 Sacha Delanoue

#include "prologin.hh"
#include <netinet/in.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

extern "C" const char* dump_state_json();

static int conn = 0;

static std::string receive()
{
    char buffer[1024] = {0};
    while (true)
    {
        int len = read(conn, buffer, 1024);
        if (len == 0)
            continue;
        int offset = 0;
        while (int(buffer[offset]) < 33 and offset < 1024)
            ++offset;
        if (offset == 1024)
            continue;
        return std::string(buffer + offset);
    }
}

void partie_init()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (!socket_fd)
        exit(1);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(0);

    if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        exit(2);
    if (listen(socket_fd, 1) < 0)
        exit(3);

    socklen_t len = sizeof(address);
    int port = 0;
    if (getsockname(socket_fd, (struct sockaddr*)&address, &len) == -1)
        exit(4);
    else
        port = ntohs(address.sin_port);
    if (!fork())
    {
        auto s = std::to_string(port);
        execl("/usr/bin/prologin2019-gui", "/usr/bin/prologin2019-gui",
              s.c_str(), std::to_string(moi()).c_str(), (char*)nullptr);
        exit(5);
    }
    conn = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&len);
    if (conn < 0)
        exit(6);
    const char* dump = dump_state_json();
    send(conn, dump, strlen(dump), 0);
    while (receive() != "NEXT")
        ;
}

static direction to_direction(const std::string& input)
{
    int value = std::stoi(input);
    // If you think this does not makes sense it is because I inverted the x
    // and y axis. It is no big deal I make the conversion here
    direction convert[4] = {OUEST, SUD, EST, NORD};
    return convert[value];
}

void jouer_tour()
{
    const char* dump = dump_state_json();
    send(conn, dump, strlen(dump), 0);
    while (true)
    {
        std::string command = receive();
        if (command == "NEXT")
            return;
        std::istringstream ss(command);
        std::vector<std::string> words;
        while (!ss.eof())
        {
            std::string word;
            std::getline(ss, word, ' ');
            if (!word.empty())
                words.push_back(word);
        }
        if (words[0] == "MOVE")
            deplacer(std::stoi(words[1]), to_direction(words[2]));
        else if (words[0] == "DROP")
            glisser(std::stoi(words[1]), to_direction(words[2]));
        else if (words[0] == "MINE")
            pousser(std::stoi(words[1]), to_direction(words[2]));
        //else if (words[0] == "PULL")
        //else if (words[0] == "SET_A_ROPE")
        else if (words[0] == "UNDO")
            annuler();
    }
}

void partie_fin()
{
}
