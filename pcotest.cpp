#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0400
#include <windows.h>
#include <winuser.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <fstream>
#include <iostream>
#include <exception>
#include <memory>

#include "PostCreationObjects.hpp"
#include "window.hpp"
#include "application.hpp"

#include <locale>

/**
 * @brief Custom Window class for our example window.
 *
 * By inheriting from pco::Window, we get basic message handling for the window. All that is left to do is overriding some methods
 * to define the behavior of the window. Usually you have to override at least OnCreate() and OnCommand().
 *
 * In this example the controls don't do anything usefull, they just change some controls to demonstrate the useage of PCO.
 */
class PCOTestWindow : public pco::Window
{
public:
    PCOTestWindow(HINSTANCE hThisInstance) : pco::Window(hThisInstance) {}

    /**
     * @brief As you can guess from the name, this method is called when the window gets created.
     *
     * Hidden behind some abstraction, we are handling the WM_CREATE message here, meaning that the window is just about to be created.
     * Here we set up our window, mainly we create and initialize child controls of our window. PCO does not (yet) help you with the
     * task of creating the controls using the somewhat daunting CreateWindow(Ex) functions. Rather the returned handles are assigned
     * to PCOs after you have created the controls manually (that's where the name comes from).
     *
     * @return true - Usually this method returns true to indicate that it was successful.
     * @return false - If you return false, this indicates an error and the window will not be created.
     */
    bool OnCreate() override
    {
        std::cout << "Creating a window: " << m_hwnd << std::endl;
        but1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Test"), WS_CHILD | WS_VISIBLE, 80, 150, 80, 25, m_hwnd, (HMENU)42, m_hThisInstance, NULL);
        but2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Copy Text"), WS_CHILD | WS_VISIBLE, 180, 150, 80, 25, m_hwnd, (HMENU)43, m_hThisInstance, NULL);

        label1 = CreateWindowEx(0, TEXT("STATIC"), TEXT("Change this text by pressing the buttons."), WS_CHILD | WS_VISIBLE, 10, 10, 310, 15, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
        edit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Type here"), WS_CHILD | WS_VISIBLE, 30, 30, 280, 25, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
        edit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Or here"), WS_CHILD | WS_VISIBLE, 30, 65, 280, 25, m_hwnd, (HMENU)0, m_hThisInstance, NULL);

        memo = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("So\r\nmany\r\nlines"), WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL, 340, 10, 140, 150, m_hwnd, (HMENU)0, m_hThisInstance, NULL);

        listBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), TEXT(""), WS_CHILD | WS_VISIBLE, 340, 170, 140, 150, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
        CreateWindowEx(0, TEXT("BUTTON"), TEXT("Add"), WS_CHILD | WS_VISIBLE, 340, 330, 60, 25, m_hwnd, (HMENU)45, m_hThisInstance, NULL);
        CreateWindowEx(0, TEXT("BUTTON"), TEXT("Get"), WS_CHILD | WS_VISIBLE, 410, 330, 60, 25, m_hwnd, (HMENU)46, m_hThisInstance, NULL);

        combo = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), TEXT(""), WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 30, 100, 180, 25, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
        combo.addItem(TEXT("Foo"));
        combo.addItem(TEXT("Bar"));
        combo.setText(TEXT("Horst"));

        radio1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 30, 15, 15, m_hwnd, (HMENU)1, m_hThisInstance, NULL);
        radio2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 65, 15, 15, m_hwnd, (HMENU)1, m_hThisInstance, NULL);
        radio3 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 100, 15, 15, m_hwnd, (HMENU)1, m_hThisInstance, NULL);
        radio3.setState(BST_CHECKED);

        PB = CreateWindowEx(WS_EX_CLIENTEDGE, PROGRESS_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 10, 200, 300, 15, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
        PB.setRange(0, 50);
        PB.setPos(0);
        PB.setText(TEXT("progress"));
        SetTimer(m_hwnd, ID_TIMER, 200, NULL);

        // Disable the theme for the progress bar control to allow changing the color
        // Controls with modern style can not change the color, old style controls can
        SetWindowTheme(PB, L"", L"");
        PBMarquee = CreateWindowEx(WS_EX_CLIENTEDGE, PROGRESS_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | PBS_MARQUEE, 10, 230, 300, 15, m_hwnd, (HMENU)0, m_hThisInstance, NULL);

        {
            pco::Edit numberEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_LEFT, 10, 250, 280, 25, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
            spin = CreateWindowEx(0, UPDOWN_CLASS, NULL, WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_SETBUDDYINT, 0, 250, 10, 25, m_hwnd, (HMENU)0, m_hThisInstance, NULL);
            spin.setRange(11, 77);
            // assigning a buddy edit to a spin control still has to be done manually
            SendMessage(spin, UDM_SETBUDDY, (WPARAM)numberEdit.getHwnd(), 0);
            spin.setValue(99);
        }

        CreateWindowEx(WS_EX_APPWINDOW, TEXT("BUTTON"), TEXT("Exit"), WS_CHILD | WS_VISIBLE, 200, 330, 80, 25, m_hwnd, (HMENU)50, m_hThisInstance, NULL);
        return true;
    }

    /**
     * @brief This method gets called when there is a timer event (WM_TIMER message).
     *
     * Each timer that was set up for our window (without an explicit TimerProc callback), will trigger an OnTimer
     * event when the timeout elapses.
     *
     * @return true - we have handled the event
     * @return false - we don't want to handle the event here
     */
    bool OnTimer(uint32_t timerId) override
    {
        // we only have one timer so there shouldn't be any messages for another timer, but just in case we
        // return false because we don't want to handle the message
        if (timerId != ID_TIMER)
        {
            return false;
        }

        // Here we do some animation of the progress bars, using the PCOs
        if (m_timerCount <= 0)
        {
            m_timerForward = true;
            PBMarquee.enableMarquee();
        }
        else if (m_timerCount >= 50)
        {
            m_timerForward = false;
            PBMarquee.disableMarquee();
        }
        if (m_timerForward)
        {
            ++m_timerCount;
        }
        else
        {
            --m_timerCount;
        }
        PB.setPos(m_timerCount);
        PB.setColor(200, 200 - 4 * m_timerCount, 4 * m_timerCount);
        return true;
    }

    /**
     * @brief Handle all the commands we get from the controls.
     *
     * Unfortunatelly, we have to deal with all the control messages here, without any support from the PCO library.
     * In a real application you probably want to define some constants for your control IDs somewhere, here in the example
     * we have just used numbers, so you have to trust the comments next to the 'case's to know which control is handled.
     * Also ther's nothing that stops you from putting the code that handles specific command events into separate functions,
     * just for this example we didn't do so.
     *
     * @param notificationCode Control-defined notification code - look it up in the WinAPI documentation for each control.
     * @param controlId ID of the control as specified in the CreateWindow call.
     * @param controlWindow Window handle of the control that triggered the command.
     * @return true - we have handled the event
     * @return false - we don't want to handle the event here
     */
    bool OnCommand(uint32_t notificationCode, uint32_t controlId, HWND controlWindow) override
    {
        switch (controlId)
        {
        case 42:                         // left button
            but1.setText(TEXT("漢語"));  // change button text (only works properly in UNICODE build)
            edit2.appendText(TEXT("!")); // append text to edit control
            if (radio1.isChecked())
                memo.addLine(edit1.getText(), true); // add line and scroll down
            if (radio2.isChecked())
                memo.addLine(edit2.getText(), true); // add line and scroll down
            if (radio3.isChecked())
                memo.addLine(combo.getText(), true); // add line and scroll down
            label1.setNum(memo.countLines());        // show number of lines of memo in label
            break;

        case 43: // right button
            but2.setText(edit2.getText());
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
            if (selectedIndex >= 0)
            {
                edit1.setText(listBox.getItemText(selectedIndex));
            }
            break;
        }

        case 50: // Exit button
            Destroy();
            break;
        }

        return true;
    }

private:
    // The PCOs for all the controls we create
    pco::Button but1, but2;
    pco::Label label1;
    pco::Edit edit1, edit2;
    pco::Memo memo;
    pco::ListBox listBox;
    pco::ComboBox combo;
    pco::RadioButton radio1, radio2, radio3;
    pco::ProgressBar PB, PBMarquee;
    pco::SpinEdit spin;

    // status of the progress bar animation (updated by timer event)
    int m_timerCount = 0;
    bool m_timerForward = true;

    // an arbitrary ID we chose to identify a timer
    const uint32_t ID_TIMER = 1234567;
};

/**
 * @brief The application is where the main message pump is run.
 *
 * If you wanted, you could just use the pco::Application without inheriting from it. But for good code style we
 * create an inherited class that defines what our application does. For example you could parse the command line
 * and react to different parameters, read settings, etc.
 * For this example it just creates a Window. Not that there are two steps required to create a window.
 *
 */
class PCOTestApplication : public pco::Application
{
public:
    PCOTestApplication(HINSTANCE hThisInstance,
                       HINSTANCE hPrevInstance,
                       LPSTR lpszArgument,
                       int nCmdShow) : pco::Application(hThisInstance, hPrevInstance, lpszArgument)
    {
        // Step 1: Create the window object. The window is not created until Initialize() is called.
        m_mainWindow = std::make_unique<PCOTestWindow>(hThisInstance);

        // Step 2: Initialize the window object, i.e. actually create the window. This two-step creation of the window is
        // necessary because we need a fully created object of the window class (Step 1) before CreateWindow is called (Step 2)
        // to be able to use runtime polymorphism in the event handler.
        // (Problem here is that CreateWindow(Ex) already sends WM_CREATE which we can't handle before the object is constructed)
        m_mainWindow->Initialize(nCmdShow);
    }

private:
    std::unique_ptr<PCOTestWindow> m_mainWindow;
};

/**
 * @brief The main function is pretty simple. Just create an object of your Application and call Run().
 *
 * @param hThisInstance
 * @param hPrevInstance
 * @param lpszArgument
 * @param nCmdShow
 * @return int WinMain
 */
int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)

{
    try
    {
        PCOTestApplication app(hThisInstance, hPrevInstance, lpszArgument, nCmdShow);
        return app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Application is terminated due to an exception: " << e.what() << '\n';
        return -1;
    }
}
