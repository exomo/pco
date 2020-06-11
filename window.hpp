#pragma once

#include <stdexcept>

#include <windows.h>
#include <winuser.h>
#include <uxtheme.h>
#include <commctrl.h>

#include <iostream>

namespace pco
{
    class WindowRegistrationException : public std::runtime_error
    {
    public:
        WindowRegistrationException() : std::runtime_error("Registration of window class failed") {}
    };

    class WindowCreationException : public std::runtime_error
    {
    public:
        WindowCreationException() : std::runtime_error("Could not create new window") {}
    };

    class Window
    {
    public:
        Window(HINSTANCE hThisInstance);

        ~Window();

        bool Initialize(int nCmdShow);

        void Destroy();

        void Show(int nCmdShow = SW_SHOW)
        {
            ShowWindow(m_hwnd, nCmdShow);
        }

    protected:
        virtual bool OnNcCreate();
        virtual bool OnCreate();
        virtual bool OnDestroy();
        virtual bool OnTimer(uint32_t timer);
        virtual bool OnCommand(uint32_t notificationCode, uint32_t controlId, HWND controlWindow);

    private:
        bool RegisterClass();
        bool Create();
        const std::string &GetWindowClassName();

        static LRESULT CALLBACK WindowProcedure(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

        static int s_registrationCounter;

    protected:
        HINSTANCE m_hThisInstance;
        HWND m_hwnd;

        // /*  Name of the class that was registered for this window, empty string means the class is not yet registered  */
        std::string m_windowClassName = "";
    };

} // namespace pco
