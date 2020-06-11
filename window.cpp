#include "window.hpp"

namespace pco
{
    Window::Window(HINSTANCE hThisInstance)
        : m_hThisInstance(hThisInstance)
    {
    }

    Window::~Window() {}

    bool Window::OnCreate()
    {
        std::cout << "OnCreate default implementation is called, did you forget to override it?\n";
        return true;
    }

    bool Window::OnDestroy()
    {
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        return true;
    }

    bool Window::OnTimer(uint32_t timer)
    {
        // The default implementation does not process this message, so we return FALSE
        return false;
    }

    bool Window::OnCommand(uint32_t notificationCode, uint32_t controlId, HWND controlWindow)
    {
        std::cout << "OnCommand default implementation is called, did you forget to override it?\n";
        return true;
    }

    bool Window::OnNcCreate()
    {
        // The default implementation does not do anything, return TRUE to indicate success
        return true;
    }

    LRESULT Window::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message) /* handle the messages */
        {
        case WM_CREATE:
            InitCommonControls();
            if (OnCreate())
            {
                return 0;
            }
            else
            {
                return -1;
            }

        case WM_DESTROY:
            return OnDestroy();
            break;

        case WM_TIMER:
            return OnTimer(static_cast<uint32_t>(wParam));

        case WM_COMMAND:
            return OnCommand(static_cast<uint32_t>(HIWORD(wParam)), static_cast<uint32_t>(LOWORD(wParam)), reinterpret_cast<HWND>(lParam));

        default: /* for messages that we don't deal with */
            return DefWindowProc(m_hwnd, message, wParam, lParam);
        }

        return TRUE;
    }

    bool Window::Initialize(
        int nCmdShow)
    {
        if (!RegisterClass())
        {
            throw WindowRegistrationException();
        }
        if (!Create())
        {
            throw WindowCreationException();
        }

        /* Make the window visible on the screen */
        Show(nCmdShow);

        return true;
    }

    bool Window::Create()
    {
        /* The class is registered, let's create the program*/
        m_hwnd = CreateWindowEx(
            0,                                /* Extended possibilites for variation */
            GetWindowClassName().c_str(),     /* Classname */
            TEXT("PCO Demonstration Window"), /* Title Text */
            WS_OVERLAPPEDWINDOW,              /* default window */
            CW_USEDEFAULT,                    /* Windows decides the position */
            CW_USEDEFAULT,                    /* where the window ends up on the screen */
            500,                              /* The programs width */
            400,                              /* and height in pixels */
            HWND_DESKTOP,                     /* The window is a child-window to desktop */
            NULL,                             /* No menu */
            m_hThisInstance,                  /* Program Instance handler */
            this                              /* No Window Creation data */
        );

        return m_hwnd != NULL;
    }

    void Window::Destroy()
    {
        DestroyWindow(m_hwnd);
    }

    LRESULT CALLBACK Window::WindowProcedure(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Window *pThis;

        if (message == WM_NCCREATE)
        {
            pThis = static_cast<Window *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

            SetLastError(0);
            if (!SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
            {
                if (GetLastError() != 0)
                    return FALSE;
            }
            // Set our internal hwnd here. If we wait until CreateWindowEx returns it, it's already tool late for
            // the message handler OnCreate().
            pThis->m_hwnd = wnd;
            return pThis->OnNcCreate();
        }
        else
        {
            pThis = reinterpret_cast<Window *>(GetWindowLongPtr(wnd, GWLP_USERDATA));
        }

        if (pThis)
        {
            return pThis->HandleMessage(message, wParam, lParam);
        }
        else
        {
            return DefWindowProc(wnd, message, wParam, lParam);
        }
    }

    bool Window::RegisterClass()
    {
        WNDCLASSEX wincl; /* Data structure for the window class */

        /* The Window structure */
        wincl.hInstance = m_hThisInstance;
        wincl.lpszClassName = GetWindowClassName().c_str();
        wincl.lpfnWndProc = Window::WindowProcedure; /* This function is called by windows */
        wincl.style = CS_DBLCLKS;                    /* Catch double-clicks */
        wincl.cbSize = sizeof(WNDCLASSEX);

        /* Use default icon and mouse-pointer */
        wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
        wincl.lpszMenuName = NULL; /* No menu */
        wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
        wincl.cbWndExtra = 0;      /* structure or the window instance */
        /* Use Windows's default color as the background of the window */
        wincl.hbrBackground = (HBRUSH)COLOR_WINDOW;

        /* Register the window class, and if it fails quit the program */
        if (!RegisterClassEx(&wincl))
            return false;

        return true;
    }

    const std::string &Window::GetWindowClassName()
    {
        if (m_windowClassName.empty())
        {
            m_windowClassName = std::string("CustomWindow") + std::to_string(++s_registrationCounter);
        }
        return m_windowClassName;
    }

    // static counter used to generate unique window class names
    int Window::s_registrationCounter = 0;

} // namespace pco
