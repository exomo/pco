#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0400
#include <windows.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <fstream>
#include <iostream>
#include "PostCreationObjects.hpp"

#define ID_TIMER    1234567

#include <locale>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = TEXT("pcotest");

HINSTANCE hInst;


pco::Button but1, but2;
pco::Label label1;
pco::Edit edit1, edit2;
pco::Memo memo;
pco::ListBox listBox;
pco::ComboBox combo;
pco::RadioButton radio1, radio2, radio3;
pco::ProgressBar PB, PBMarquee;
pco::SpinEdit spin;
pco::Form hwnd;               /* This is the handle for our window */

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    hInst = hThisInstance;

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           TEXT("PCO Demonstration Window"), /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           400,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    SetWindowTheme(hwnd, L"Explorer", L"");

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int i = 0;
    static bool flag = true;
    pco::Form hwnd(wnd);

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            InitCommonControls();

            but1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Test"), WS_CHILD | WS_VISIBLE, 80, 150, 80, 25, hwnd, (HMENU) 42, hInst, NULL);
            but2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Copy Text"), WS_CHILD | WS_VISIBLE, 180, 150, 80, 25, hwnd, (HMENU) 43, hInst, NULL);

            label1 = CreateWindowEx(0, TEXT("STATIC"), TEXT("Change this text by pressing the buttons."), WS_CHILD | WS_VISIBLE, 10, 10, 310, 15, hwnd, (HMENU) 0, hInst, NULL);
            edit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Type here"), WS_CHILD | WS_VISIBLE, 30, 30, 280, 25, hwnd, (HMENU) 0, hInst, NULL);
            edit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Or here"), WS_CHILD | WS_VISIBLE, 30, 65, 280, 25, hwnd, (HMENU) 0, hInst, NULL);

            memo = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("So\r\nmany\r\nlines"), WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL , 340, 10, 140, 150, hwnd, (HMENU) 0, hInst, NULL);

            listBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), TEXT(""), WS_CHILD | WS_VISIBLE, 340, 170, 140, 150, hwnd, (HMENU) 0, hInst, NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Add"), WS_CHILD | WS_VISIBLE, 340, 330, 60, 25, hwnd, (HMENU) 45, hInst, NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Get"), WS_CHILD | WS_VISIBLE, 410, 330, 60, 25, hwnd, (HMENU) 46, hInst, NULL);

            combo = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), TEXT(""), WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 30, 100, 180, 25, hwnd, (HMENU) 0, hInst, NULL);
            combo.addItem(TEXT("Foo"));
            combo.addItem(TEXT("Bar"));
            combo.setText(TEXT("Horst"));

            radio1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 30, 15, 15, hwnd, (HMENU) 1, hInst, NULL);
            radio2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 65, 15, 15, hwnd, (HMENU) 1, hInst, NULL);
            radio3 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 100, 15, 15, hwnd, (HMENU) 1, hInst, NULL);
            radio3.setState(BST_CHECKED);

            PB = CreateWindowEx(WS_EX_CLIENTEDGE, PROGRESS_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 10, 200, 300, 15, hwnd, (HMENU) 0, hInst, NULL);
            PB.setRange(0, 50);
            PB.setPos(0);
            PB.setText(TEXT("progress"));
            SetTimer(hwnd, ID_TIMER, 200, NULL);

            // Disable the theme for the progress bar control to allow changing the color
            // Controls with modern style can not change the color, old style controls can
            SetWindowTheme(PB, L"", L"");

            PBMarquee = CreateWindowEx(WS_EX_CLIENTEDGE, PROGRESS_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | PBS_MARQUEE, 10, 230, 300, 15, hwnd, (HMENU) 0, hInst, NULL);

            {
                pco::Edit numberEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_LEFT, 10, 250, 280, 25, hwnd, (HMENU) 0, hInst, NULL);
                spin = CreateWindowEx(0, UPDOWN_CLASS, NULL, WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_SETBUDDYINT, 0, 250, 10, 25, hwnd, (HMENU) 0, hInst, NULL);
                spin.setRange(11, 77);
                //SendMessage(spin, UDM_SETRANGE32, 0, 100);
                SendMessage(spin, UDM_SETBUDDY, (WPARAM) numberEdit.getHwnd(), 0);
                spin.setValue(99);
            }

            CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("Exit"), WS_CHILD | WS_VISIBLE, 200, 330, 80, 25, hwnd, (HMENU) 50, hInst, NULL);

            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        case WM_TIMER:

            if(i<=0)
            {
                flag = true;
                PBMarquee.enableMarquee();
            }
                else if(i >= 50)
            {
                flag = false;
                PBMarquee.disableMarquee();
            }
            if(flag)
            {
                ++i;
            }
            else
            {
                --i;
            }
            PB.setPos(i);
            PB.setColor(200, 200 - 4*i, 4*i);
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case 42: // left button
                    but1.setText( TEXT("漢語"));          // change button text
                    edit2.appendText(TEXT("!"));         // append text to edit control
                    if(radio1.isChecked()) memo.addLine(edit1.getText(), true); // add line and scroll down
                    if(radio2.isChecked()) memo.addLine(edit2.getText(), true); // add line and scroll down
                    if(radio3.isChecked()) memo.addLine(combo.getText(), true); // add line and scroll down
                    label1.setNum( memo.countLines());   // show number of lines of memo in label
                    break;
                case 43: // right button
                    but2.setText( edit2.getText() );
                    label1.setNum(42.1);
                    label1.setText(label1.getText() + TEXT(" Hello ") + edit1.getText());
                    edit2.setNum(spin.getValue());
                    break;

                case 45: // listbox add button
                    listBox.addItem(edit1.getText());
                    break;

                case 46: // listbox get button
                    {
                        int selectedIndex = listBox.getSel();
                        if(selectedIndex>=0)
                        {
                            edit1.setText(listBox.getItemText(selectedIndex));
                        }
                    }
                    break;

                case 50:
                    hwnd.Destroy();
                    break;
            }

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
