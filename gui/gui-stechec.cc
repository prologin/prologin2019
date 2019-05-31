// SPDX-Licence-Identifier: GPL-3.0-or-later

#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "prologin.hh"

extern "C" const char* dump_state_json();

static int status_connect = 0;

static std::string receive()
{
  char buffer[1024] = {0};
  while (true)
  {
    int len = read(status_connect, buffer, 1024);
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
  socklen_t len = sizeof(address);

  if (bind(socket_fd, (struct sockaddr*)&address, len) < 0)
    exit(2);
  if (listen(socket_fd, 1) < 0)
    exit(3);

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
  
  status_connect = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&len);
  if (status_connect < 0)
    exit(6);

  const char* dump = dump_state_json();
  send(status_connect, dump, strlen(dump), 0);
  while (receive() != "NEXT")
    continue;
}

static direction to_direction(const std::string& input)
{
  int value = std::stoi(input);
  direction convert[4] = {BAS, HAUT, GAUCHE, DROITE};
  return convert[value];
}

void jouer_tour()
{
  const char* dump = dump_state_json();
  send(status_connect, dump, strlen(dump), 0);
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
      lacher(std::stoi(words[1]));
    else if (words[0] == "GRAB")
      agripper(std::stoi(words[1]));
    else if (words[0] == "MINE")
      miner(std::stoi(words[1]), to_direction(words[2]));
    else if (words[0] == "PULL")
      tirer(std::stoi(words[1]), to_direction(words[2]), to_direction(words[3]));
    else if (words[0] == "SET_ROPE")
      poser_corde(std::stoi(words[1]), to_direction(words[2]));
    else if (words[0] == "CANCEL")
      annuler();
  }

}

void partie_fin()
{}
