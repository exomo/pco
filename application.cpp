#include "application.hpp"

namespace pco
{

    Application::Application(HINSTANCE hThisInstance,
                             HINSTANCE hPrevInstance,
                             LPSTR lpszArgument)
        : m_hThisInstance(hThisInstance), m_hPrevInstance(hPrevInstance)
    {
        Initialize();
    }

    Application::~Application()
    {
    }

    bool Application::Initialize()
    {
        return true;
    }

    int Application::Run()
    {
        return MessageLoop();
    }

    WPARAM Application::MessageLoop()
    {
        MSG messages; /* Here messages to the application are saved */

        /* Run the message loop. It will run until GetMessage() returns 0 */
        while (GetMessage(&messages, NULL, 0, 0))
        {
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }

        /* The program return-value is 0 - The value that PostQuitMessage() gave */
        return messages.wParam;
    }

} // namespace pco
