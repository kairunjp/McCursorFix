#include <Windows.h>
#include <iostream>

POINT lastCursorPos;

int padding = 50;

bool IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&ntAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}


bool IsTargetWindowInFocus(HWND targetWindow) {
    return GetForegroundWindow() == targetWindow;
}

/*void ConstrainCursorWithinWindow(HWND targetWindow) {
    RECT windowRect;
    GetWindowRect(targetWindow, &windowRect);

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    if (cursorPos.x - padding < windowRect.left || cursorPos.x + padding > windowRect.right ||
        cursorPos.y - padding < windowRect.top || cursorPos.y + padding > windowRect.bottom) {
        SetCursorPos(lastCursorPos.x, lastCursorPos.y);
    }
    else {
        lastCursorPos = cursorPos;
    }
}*/

void ConstrainCursorWithinWindow(HWND targetWindow) {
    RECT windowRect;
    GetWindowRect(targetWindow, &windowRect);

    int centerX = (windowRect.left + windowRect.right) / 2;
    int centerY = (windowRect.top + windowRect.bottom) / 2;

    ClipCursor(&windowRect);

    SetCursorPos(centerX, centerY);
}


int main() {
    printf("マイクラのマウス逃げちゃうやつ治すやつツール (日本語病気)")
    if (!IsRunningAsAdmin()) {
        std::cout << "このアプリケーションは管理者権限で実行する必要があります。" << std::endl;
        return 1;
    }

    HWND targetWindow = FindWindowA(nullptr, "Minecraft");
    if (!targetWindow) {
        printf("マイクラが見つかりませんでした\n");
        return 1;
    }
    else printf("マイクラが見つかりました\n");


    // GetCursorPos(&lastCursorPos);

    while (true) {
        CURSORINFO cursorInfo;
        cursorInfo.cbSize = sizeof(CURSORINFO);
        if (GetCursorInfo(&cursorInfo)) {
            if (cursorInfo.flags != CURSOR_SHOWING) {
                if (IsTargetWindowInFocus(targetWindow)) {
                    ConstrainCursorWithinWindow(targetWindow);
                }
                else {
                    ClipCursor(nullptr);
                }
            }
            else {
                ClipCursor(nullptr);
            }
        }
        Sleep(5);
    }

    return 0;
}
