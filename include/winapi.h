#ifndef WINAPI_H
#define WINAPI_H

#include "stat.h"
#include <Windows.h> // Advapi32.dll

struct LogonStatus {
    char* user;
    char *pass;
    bool success;
    DWORD rvalue;
    char* msg;
};

inline LogonStatus* verbose_logon(char* user, char* pass, char* domain) {

    stat_tries++;

    // Return value
    LogonStatus *status = new LogonStatus;
    status->user = user;
    status->pass = pass;
    status->rvalue = 1;
    status->success = true;
    status->msg = MAKE_STR;

    HANDLE handle;
    if (!LogonUserA(
            user,
            domain,
            pass,
            LOGON32_LOGON_NETWORK,
            LOGON32_PROVIDER_DEFAULT,
            &handle)) {
        status->rvalue = GetLastError();
        status->success = false;

        FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                status->rvalue,
                0,
                (char*) &status->msg,
                0,
                NULL
                );
    }

    CloseHandle(handle);
    LocalFree(handle);
    return status;
}

inline bool fast_logon(char* user, char* pass, char* domain) {

    stat_tries++;

    HANDLE handle;
    const bool success = LogonUserA(
            user,
            domain,
            pass,
            LOGON32_LOGON_NETWORK,
            LOGON32_PROVIDER_DEFAULT,
            &handle);
    CloseHandle(handle);
    LocalFree(handle);
    return success;
}

#endif
