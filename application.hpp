#pragma once

#include <windows.h>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <memory>

namespace pco
{

    class Application
    {
    public:
        Application(HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument);

        virtual ~Application();

        virtual bool Initialize();

        int Run();

    private:
        WPARAM MessageLoop();

        HINSTANCE m_hThisInstance;
        HINSTANCE m_hPrevInstance;
    };

} // namespace pco
