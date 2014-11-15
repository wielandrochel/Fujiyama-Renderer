// Copyright (c) 2011-2014 Hiroshi Tsubokawa
// See LICENSE and README

#include "fj_socket.h"

namespace fj {

const int DEFAULT_PORT = 50505;

inline static struct sockaddr *get_sockaddr(struct sockaddr_in * addr_in)
{
  return reinterpret_cast<struct sockaddr *>(addr_in);
}

Socket::Socket() :
    fd_(FJ_INVALID_SOCKET), len_(sizeof(address_)), address_()
{
}

Socket::~Socket()
{
  Close();
}

socket_id Socket::Open()
{
  if (IsOpen()) {
    return fd_;
  }

  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ == FJ_INVALID_SOCKET) {
    return SOCKET_ID_INVALID;
  }

  address_.sin_family = AF_INET;
  SetAddress("");
  SetPort(DEFAULT_PORT);

  return fd_;
}

bool Socket::IsOpen() const
{
  return fd_ > 2; // stderr
}

void Socket::Close()
{
  if (IsOpen()) {
    fj_close_socket(fd_);
  }
}

void Socket::Shutdown()
{
  if (IsOpen()) {
    shutdown(fd_, FJ_SHUTDOWN_READ_WRITE);
  }
}

void Socket::ShutdownRead()
{
  if (IsOpen()) {
    shutdown(fd_, FJ_SHUTDOWN_READ);
  }
}

void Socket::ShutdownWrite()
{
  if (IsOpen()) {
    shutdown(fd_, FJ_SHUTDOWN_WRITE);
  }
}

int Socket::GetFileDescriptor() const
{
  return fd_;
}

int Socket::SetNoDelay()
{
  int enable = 1;
  const int result = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY,
      reinterpret_cast<char *>(&enable), sizeof(enable));

  if (result == FJ_SOCKET_ERROR) {
    return -1;
  }

  return 0;
}

void Socket::SetAddress(const std::string &address)
{
  if (address == "") {
    address_.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    address_.sin_addr.s_addr = inet_addr(address.c_str());
  }
}

void Socket::SetPort(int port)
{
  address_.sin_port = htons(port);
}

int Socket::Connect()
{
  const int fd = Open();
  if (fd == -1) {
    return -1;
  }

  len_ = sizeof(address_);
  const int result = connect(fd_, get_sockaddr(&address_), len_);
  if (result == FJ_SOCKET_ERROR) {
    return -1;
  }

  return fd_;
}

int Socket::Bind()
{
  const int fd = Open();
  if (fd == -1) {
    return -1;
  }

  len_ = sizeof(address_);
  const int result = bind(fd_, get_sockaddr(&address_), len_);
  if (result == FJ_SOCKET_ERROR) {
    return -1;
  }

  return fd_;
}

int Socket::Listen()
{
  const int result = listen(fd_, SOMAXCONN);
  if (result == FJ_SOCKET_ERROR) {
    return -1;
  }

  return 0;
}

socket_id Socket::Accept(Socket &accepted)
{
  accepted.fd_ = accept(fd_, get_sockaddr(&accepted.address_), &accepted.len_);
  if (accepted.fd_ == FJ_INVALID_SOCKET) {
    return SOCKET_ID_INVALID;
  }

  return accepted.fd_;
}

socket_id Socket::AcceptOrTimeout(Socket &accepted, int sec, int micro_sec)
{
  struct timeval timeout;
  timeout.tv_sec = sec;
  timeout.tv_usec = micro_sec;

  fd_set read_mask;
  FD_ZERO(&read_mask);
  FD_SET(fd_, &read_mask);

  const int result = select(fd_ + 1, &read_mask, NULL, NULL, &timeout);
  if (result == FJ_SOCKET_ERROR) {
    // error
    return SOCKET_ID_INVALID;
  }
  else if (result == 0) {
    // time out
    return SOCKET_ID_TIMEOUT;
  }

  if (FD_ISSET(fd_, &read_mask)) {
    return Accept(accepted);
  } else {
    return SOCKET_ID_INVALID;
  }
}

int Socket::Receive(char *data, size_t count)
{
  return recv(fd_, data, count, MSG_WAITALL);
}

int Socket::Send(const char *data, size_t count)
{
  return send(fd_, data, count, 0);
}

} // namespace xxx