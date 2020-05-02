#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <Windows.h>
#include <stdio.h>
#include <winsock.h>
#include <iostream>

// 协议中加密部分使用的是base64方法
char ConvertToBase64(char);
void EncodeBase64(char*, char*, int);
bool SendMail(std::string, std::string, std::string, const char*);
int  OpenSocket(struct sockaddr*);

#endif // SENDMAIL_H
