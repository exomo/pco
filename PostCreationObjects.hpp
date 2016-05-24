/**************************************************************************
 *  Copyright 2016 Kai Bublitz (exomo)                                    *
 *                                                                        *
 *  This file is part of the PostCreationObjects library (PCO).                                           *
 *                                                                        *
 *  PCO is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  PCO is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with PCO.  If not, see <http://www.gnu.org/licenses/>.        *
 *                                                                        *
 **************************************************************************/

/**
    PostCreationObjects are Objects to easily handle window controls
    after they have been created. You can create the window with all
    the controls as usual (e.g. using resource files) and create the
    objects from the window handles of the controls. Then you can use
    your new objects to perform various operations on the controls

    Objects:
        - Button
        - Label
        - Edit
        - Memo (Multiline-Edit)
        - ListBox
        - ComboBox
        - CheckBox
        - ProgressBar
        - SpinEdit
**/

#ifndef __POSTCREATIONOBJECTS_HPP
#define __POSTCREATIONOBJECTS_HPP

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <sstream>
#include <vector>

/* Workaround for mingw: The winapi that comes with mingw is missing some constants */
#if _WIN32_WINNT >= 501
#ifndef PBS_MARQUEE
    #define PBS_MARQUEE 0x08
    #define PBM_SETMARQUEE (WM_USER+10)
#endif
#endif

namespace pco
{

// super class DlgObject
template <typename chartype>
class DlgObject {
    protected:
        HWND hwnd;

    public:
        HWND getHwnd() const;
        // implicit cast to HWND
        operator HWND(){return hwnd;}

        virtual void show();
        virtual void hide();
        virtual void enable();
        virtual void disable();
        virtual void move(int x, int y, int width, int height, bool repaint = true);
        virtual void setText(const std::basic_string<chartype>& text);
        virtual std::basic_string<chartype> getText();
};

template <typename chartype>
class TForm : public DlgObject<chartype> {
    public:
        TForm();
        TForm(HWND window);
        TForm(const TForm& oldForm);
        void Create(HWND parent=0);
        void Destroy();
        bool operator! () {return (DlgObject<chartype>::hwnd == 0); }
        bool createNotifyIcon(unsigned int id, unsigned int msg, HICON icon, std::basic_string<chartype> text);
        bool deleteNotifyIcon(unsigned int id);
        bool updateNotifyIconText(unsigned int id, std::basic_string<chartype> text);
};

template <typename chartype>
class TButton : public DlgObject<chartype> {

    public:
        TButton();
        TButton(HWND window);
        TButton(const TButton&);
};

template <typename chartype>
class TLabel : public DlgObject<chartype> {

    public:
        TLabel();
        TLabel(HWND window);
        TLabel(const TLabel&);
        template <typename numtype>
        void setNum(numtype i);
};

template <typename chartype>
class TSpinEdit : public DlgObject<chartype> {
    public:
        TSpinEdit();
        TSpinEdit(HWND window);
        TSpinEdit(const TSpinEdit&);
        void setValue(int value);
        int getValue();
        void setRange(int min, int max);
};

template <typename chartype>
class TEdit : public DlgObject<chartype> {
    public:
        TEdit();
        TEdit(HWND window);
        TEdit(const TEdit&);
        template <typename numtype>
        void setNum(numtype i);
        void appendText(const std::basic_string<chartype>& text);
        int getInt();
        void setLimitText(int length);
        void clear();
        TSpinEdit<chartype> addSpinControl();
};

template <typename chartype>
class TMemo : public DlgObject<chartype> {

    public:
        TMemo();
        TMemo(HWND window);
        TMemo(const TMemo&);
        void appendText(const std::basic_string<chartype>& text);
        void addLine(const std::basic_string<chartype>& line, bool scroll=false);
        unsigned int countLines();
        void clear();
};

template <typename chartype>
class TListBox : public DlgObject<chartype> {

    public:
        TListBox();
        TListBox(HWND window);
        TListBox(const TListBox&);

        bool clear();
        bool addItem(const std::basic_string<chartype>& item);
        bool addItem(void* itemData);
        bool addItem(const std::basic_string<chartype>& item, void* itemData);
        bool deleteItem(int pos);
        std::basic_string<chartype> getItemText(int pos);
        void* getItemData(int);
        int getSel();
};

template <typename chartype>
class TComboBox : public DlgObject<chartype> {

    public:
        TComboBox();
        TComboBox(HWND window);
        TComboBox(const TComboBox&);
        void addItem(const std::basic_string<chartype>& item);
        void deleteItem(int);
        std::basic_string<chartype> getItemText(int);
        int getSelection();
};

template <typename chartype>
class TCheckbox : public DlgObject<chartype> {

    public:
        TCheckbox();
        TCheckbox(HWND window);
        TCheckbox(const TCheckbox&);
        void setState(int);
        int getState();
};

template <typename chartype>
class TRadioButton : public DlgObject<chartype> {

    public:
        TRadioButton();
        TRadioButton(HWND window);
        TRadioButton(const TRadioButton&);
        void setState(int);
        int getState();
        bool isChecked();
};

template <typename chartype>
class TProgressBar : public DlgObject<chartype> {
    public:
        TProgressBar();
        TProgressBar(HWND window);
        TProgressBar(const TProgressBar& oldProg);
        void setRange(int min, int max);
        void setRange(int max = 100) {setRange(0, max);};
        void setPos(int);
        void deltaPos(int);
        void setStep(int);
        void stepIt();
        void setColor(int r, int g, int b);
        void setBgColor(int r, int g, int b);
        void enableMarquee();
        void disableMarquee();
        static TProgressBar<chartype> create(HWND parent, int x, int y, int width, int height, int min, int max);
};

template <typename chartype>
class TFrame : public DlgObject<chartype> {
    public:
        TFrame();
        TFrame(HWND window);
        TFrame(const TFrame& oldFrame);
        void addControl(const DlgObject<chartype> &obj);
        void removeControl(const DlgObject<chartype> &obj);
        void show();
        void hide();
        void enable();
        void disable();

    private:
        std::vector< DlgObject<chartype> > groupControls;
};


/* implementation of the superclass */
template<typename chartype>
HWND DlgObject<chartype>::getHwnd() const
{
    return hwnd;
}

template<typename chartype>
void DlgObject<chartype>::show()
{
    ShowWindow(hwnd, SW_SHOW);
}

template<typename chartype>
void DlgObject<chartype>::hide()
{
    ShowWindow(hwnd, SW_HIDE);
}

template<typename chartype>
void DlgObject<chartype>::move(int x, int y, int width, int height, bool repaint)
{
    MoveWindow(hwnd, x, y, width, height, repaint);
}

template<typename chartype>
void DlgObject<chartype>::enable()
{
    EnableWindow(hwnd, TRUE);
}

template<typename chartype>
void DlgObject<chartype>::disable()
{
    EnableWindow(hwnd, FALSE);
}

template<typename chartype>
void DlgObject<chartype>::setText(const std::basic_string<chartype>& text)
{
    SetWindowText(hwnd, text.c_str());
}

template<typename chartype>
std::basic_string<chartype> DlgObject<chartype>::getText()
{
    int len = GetWindowTextLength(hwnd);
    chartype* buffer = new chartype[len+1];
    int ret = GetWindowText(hwnd, buffer, len+1);
    std::basic_string<chartype> text(buffer);
    delete[] buffer;
    if (ret == len) return text;
    else
    {
        return std::basic_string<chartype>();
    }
}

/* implementation of the TButton class */
template<typename chartype>
TButton<chartype>::TButton()
{
    DlgObject<chartype>::hwnd = NULL;
}

template<typename chartype>
TButton<chartype>::TButton(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template<typename chartype>
TButton<chartype>::TButton(const TButton& oldBut)
{
    DlgObject<chartype>::hwnd = oldBut.hwnd;
}

/* implementation of Label class */
template<typename chartype>
TLabel<chartype>::TLabel()
{
    DlgObject<chartype>::hwnd = 0;
}

template<typename chartype>
TLabel<chartype>::TLabel(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template<typename chartype>
TLabel<chartype>::TLabel(const TLabel& oldLbl)
{
    DlgObject<chartype>::hwnd = oldLbl.hwnd;
}

template<typename chartype>
template<typename numtype>
void TLabel<chartype>::setNum(numtype value)
{
    std::basic_ostringstream<chartype> oss;
    oss << value;
    SetWindowText(DlgObject<chartype>::hwnd, oss.str().c_str());
}

/* implementation of TEdit class */
template<typename chartype>
TEdit<chartype>::TEdit()
{
    DlgObject<chartype>::hwnd = NULL;
}

template<typename chartype>
TEdit<chartype>::TEdit(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template<typename chartype>
TEdit<chartype>::TEdit(const TEdit& oldBut)
{
    DlgObject<chartype>::hwnd = oldBut.hwnd;
}

template<typename chartype>
template<typename numtype>
void TEdit<chartype>::setNum(numtype value)
{
    std::basic_ostringstream<chartype> oss;
    oss << value;
    SetWindowText(DlgObject<chartype>::hwnd, oss.str().c_str());
}

template<typename chartype>
void TEdit<chartype>::appendText(const std::basic_string<chartype>& text)
{
    SetWindowText(DlgObject<chartype>::hwnd, (DlgObject<chartype>::getText()+text).c_str());
}

template<typename chartype>
int TEdit<chartype>::getInt()
{
    // std::basic_istringstream<chartype> iss(getText());

    int len = GetWindowTextLength(DlgObject<chartype>::hwnd);
    char* buffer = new char[len+1];
    int ret = GetWindowText(DlgObject<chartype>::hwnd, buffer, len+1);
    int num = atoi(buffer);
    if (ret == len) return num;
    else return 0;
}

template<typename chartype>
void TEdit<chartype>::setLimitText(int length)
{
    SendMessage(DlgObject<chartype>::hwnd, EM_SETLIMITTEXT, (LPARAM) length, 0);
}

template<typename chartype>
void TEdit<chartype>::clear()
{
    SetWindowText(DlgObject<chartype>::hwnd, TEXT(""));
}

/** atttaches a spin control to the edit box (edit box becomes buddy)
 *  the parent of the spin control is the edit's parent
 */
template<typename chartype>
TSpinEdit<chartype> TEdit<chartype>::addSpinControl()
{
    HWND spinner = CreateWindowEx(0, UPDOWN_CLASS, NULL, WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_SETBUDDYINT, 0, 0, 0, 0, GetParent(DlgObject<chartype>::hwnd), (HMENU) 0, 0, NULL);
    SendMessage(spinner, UDM_SETBUDDY, (WPARAM) DlgObject<chartype>::hwnd, 0);
    return TSpinEdit<chartype>(spinner);
}

/* implementation of TMemo class */
template<typename chartype>
TMemo<chartype>::TMemo()
{
    DlgObject<chartype>::hwnd = NULL;
}

template<typename chartype>
TMemo<chartype>::TMemo(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template<typename chartype>
TMemo<chartype>::TMemo(const TMemo& oldMem)
{
    DlgObject<chartype>::hwnd = oldMem.DlgObject<chartype>::hwnd;
}

template<typename chartype>
void TMemo<chartype>::appendText(const std::basic_string<chartype>& text)
{
    SetWindowText(DlgObject<chartype>::hwnd, (DlgObject<chartype>::getText()+text).c_str());
}

template<typename chartype>
void TMemo<chartype>::addLine(const std::basic_string<chartype>& line, bool scroll)
{
    std::basic_string<chartype> text = DlgObject<chartype>::getText();
    if (text.length() > 0) text += TEXT("\x0D\x0A");
    text += line;
    SetWindowText(DlgObject<chartype>::hwnd, text.c_str());

    if(scroll)
    {
        unsigned int lineCount = countLines();
        SendMessage(DlgObject<chartype>::hwnd, EM_LINESCROLL, 0, lineCount);
    }
}

template<typename chartype>
unsigned int TMemo<chartype>::countLines()
{
    return SendMessage(DlgObject<chartype>::hwnd, EM_GETLINECOUNT,0,0);
}

template<typename chartype>
void TMemo<chartype>::clear()
{
    SetWindowText(DlgObject<chartype>::hwnd, TEXT(""));
}

/* implementation of the LisBox class */
template<typename chartype>
TListBox<chartype>::TListBox()
{
    DlgObject<chartype>::hwnd = NULL;
}

template<typename chartype>
TListBox<chartype>::TListBox(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template<typename chartype>
TListBox<chartype>::TListBox(const TListBox& oldBut)
{
    DlgObject<chartype>::hwnd = oldBut.hwnd;
}

template<typename chartype>
bool TListBox<chartype>::clear()
{
    SendMessage(DlgObject<chartype>::hwnd, LB_RESETCONTENT, 0, 0);
    return true;
}

template<typename chartype>
bool TListBox<chartype>::addItem(const std::basic_string<chartype>& newItem)
{
    SendMessage(DlgObject<chartype>::hwnd, LB_ADDSTRING, 0, (LPARAM) newItem.c_str());
    return true;
}

template<typename chartype>
bool TListBox<chartype>::addItem(const std::basic_string<chartype>& newItem, void* itemData)
{
    int index = SendMessage(DlgObject<chartype>::hwnd, LB_ADDSTRING, 0, (LPARAM) newItem.c_str());
    if(index == LB_ERR || index == LB_ERRSPACE) return false;
    SendMessage(DlgObject<chartype>::hwnd, LB_SETITEMDATA, index, (LPARAM) itemData);
    return true;
}

template<typename chartype>
bool TListBox<chartype>::addItem(void* itemData)
{
    int index = SendMessage(DlgObject<chartype>::hwnd, LB_ADDSTRING, 0, (LPARAM) itemData);
    if(index == LB_ERR || index == LB_ERRSPACE) return false;
    //SendMessage(DlgObject<chartype>::hwnd, LB_SETITEMDATA, index, (LPARAM) itemData);
    return true;
}

template<typename chartype>
bool TListBox<chartype>::deleteItem(int pos)
{
    SendMessage(DlgObject<chartype>::hwnd, LB_DELETESTRING, (WPARAM) pos, 0);
    return true;
}

template<typename chartype>
std::basic_string<chartype> TListBox<chartype>::getItemText(int pos)
{
    int len = SendMessage(DlgObject<chartype>::hwnd, LB_GETTEXTLEN, pos, 0);
    chartype* buffer = new chartype[len+1];
    int ret = SendMessage(DlgObject<chartype>::hwnd, LB_GETTEXT, pos, (LPARAM) buffer);
    std::basic_string<chartype> text(buffer);
    delete[] buffer;
    if (ret == len) return text;
    else
    {
        return std::basic_string<chartype>();
    }
    return buffer;
}

template<typename chartype>
void* TListBox<chartype>::getItemData(int pos)
{
    return (void*) SendMessage(DlgObject<chartype>::hwnd, LB_GETITEMDATA, pos, 0);
}


template<typename chartype>
int TListBox<chartype>::getSel()
{
    return (int) SendMessage(DlgObject<chartype>::hwnd, LB_GETCURSEL,0,0);
}

/* implementation of TComboBox class */
template<typename chartype>
TComboBox<chartype>::TComboBox()
{
    DlgObject<chartype>::hwnd = NULL;
}

template<typename chartype>
TComboBox<chartype>::TComboBox(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template<typename chartype>
TComboBox<chartype>::TComboBox(const TComboBox& oldComb)
{
    DlgObject<chartype>::hwnd = oldComb.hwnd;
}

template <typename chartype>
void TComboBox<chartype>::addItem(const std::basic_string<chartype>& item)
{
    SendMessage(DlgObject<chartype>::hwnd, CB_ADDSTRING, 0, (LPARAM) item.c_str());
}

template <typename chartype>
void TComboBox<chartype>::deleteItem(int pos)
{
    SendMessage(DlgObject<chartype>::hwnd, CB_DELETESTRING, pos, 0);
}

template <typename chartype>
std::basic_string<chartype> TComboBox<chartype>::getItemText(int pos)
{
    int len = SendMessage(DlgObject<chartype>::hwnd, CB_GETLBTEXTLEN, pos, 0);
    chartype* buffer = new chartype[len+1];
    int ret = SendMessage(DlgObject<chartype>::hwnd, CB_GETLBTEXT, pos, (LPARAM) buffer);
    std::basic_string<chartype> text(buffer);
    delete[] buffer;
    if (ret == len) return text;
    else
    {
        return std::basic_string<chartype>();
    }
    return buffer;
}

template <typename chartype>
int TComboBox<chartype>::getSelection()
{
    return SendMessage(DlgObject<chartype>::hwnd, CB_GETCURSEL, 0,0);
}

/* implementation of TCheckbox class */
template <typename chartype>
TCheckbox<chartype>::TCheckbox()
{
    DlgObject<chartype>::hwnd = NULL;
}

template <typename chartype>
TCheckbox<chartype>::TCheckbox(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template <typename chartype>
TCheckbox<chartype>::TCheckbox(const TCheckbox& oldCheck)
{
    DlgObject<chartype>::hwnd = oldCheck.hwnd;
}

template <typename chartype>
int TCheckbox<chartype>::getState()
{
    return SendMessage(DlgObject<chartype>::hwnd, BM_GETCHECK,0,0);
}

template <typename chartype>
void TCheckbox<chartype>::setState(int newState)
{
    SendMessage(DlgObject<chartype>::hwnd, BM_SETCHECK, (WPARAM) newState, 0);
}

/* implementation of TRadioButton class */
template <typename chartype>
TRadioButton<chartype>::TRadioButton()
{
    DlgObject<chartype>::hwnd = NULL;
}

template <typename chartype>
TRadioButton<chartype>::TRadioButton(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template <typename chartype>
TRadioButton<chartype>::TRadioButton(const TRadioButton& oldRadio)
{
    DlgObject<chartype>::hwnd = oldRadio.hwnd;
}

template <typename chartype>
int TRadioButton<chartype>::getState()
{
    return SendMessage(DlgObject<chartype>::hwnd, BM_GETCHECK,0,0);
}

template <typename chartype>
void TRadioButton<chartype>::setState(int newState)
{
    SendMessage(DlgObject<chartype>::hwnd, BM_SETCHECK, (WPARAM) newState, 0);
}

template <typename chartype>
bool TRadioButton<chartype>::isChecked()
{
    return SendMessage(DlgObject<chartype>::hwnd, BM_GETCHECK,0,0) == BST_CHECKED;
}


/* implementation of TProgressBar class */
template <typename chartype>
TProgressBar<chartype>::TProgressBar()
{
    DlgObject<chartype>::hwnd = NULL;
}

template <typename chartype>
TProgressBar<chartype>::TProgressBar(HWND window)
{
     DlgObject<chartype>::hwnd = window;
}

template <typename chartype>
TProgressBar<chartype>::TProgressBar(const TProgressBar& oldProg)
 {
     DlgObject<chartype>::hwnd = oldProg.hwnd;
 }

template <typename chartype>
void TProgressBar<chartype>::setRange(int min, int max)
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_SETRANGE, 0, MAKELPARAM(min, max));
}

template <typename chartype>
void TProgressBar<chartype>::setPos(int pos)
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_SETPOS, (WPARAM) pos, 0);
}

template <typename chartype>
void TProgressBar<chartype>::deltaPos(int pos)
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_DELTAPOS, (WPARAM) pos, 0);
}

template <typename chartype>
void TProgressBar<chartype>::setStep(int step)
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_SETSTEP, (WPARAM) step, 0);
}

template <typename chartype>
void TProgressBar<chartype>::stepIt()
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_STEPIT, 0, 0);
}

template <typename chartype>
void TProgressBar<chartype>::setColor(int r, int g, int b)
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_SETBARCOLOR, 0, RGB(r,g,b));
}

template <typename chartype>
void TProgressBar<chartype>::setBgColor(int r, int g, int b)
{
    SendMessage(DlgObject<chartype>::hwnd, PBM_SETBKCOLOR, 0, RGB(r,g,b));
}

template <typename chartype>
void TProgressBar<chartype>::enableMarquee()
{
    SendMessage(DlgObject<chartype>::hwnd,(UINT) PBM_SETMARQUEE,(WPARAM) 1,(LPARAM)NULL);
}

template <typename chartype>
void TProgressBar<chartype>::disableMarquee()
{
    SendMessage(DlgObject<chartype>::hwnd,(UINT) PBM_SETMARQUEE,(WPARAM) 0,(LPARAM)NULL);
}

/* implementation of SpinEdit class */
template <typename chartype>
TSpinEdit<chartype>::TSpinEdit()
{

}

template <typename chartype>
TSpinEdit<chartype>::TSpinEdit(HWND window)
{
    DlgObject<chartype>::hwnd = window;
}

template <typename chartype>
TSpinEdit<chartype>::TSpinEdit(const TSpinEdit &oldSpin)
{
    DlgObject<chartype>::hwnd = oldSpin.hwnd;
}

template <typename chartype>
void TSpinEdit<chartype>::setValue(int val)
{
    SendMessage(DlgObject<chartype>::hwnd, UDM_SETPOS32, 0, val );
}

template <typename chartype>
int TSpinEdit<chartype>::getValue()
{
    return SendMessage(DlgObject<chartype>::hwnd, UDM_GETPOS32, 0, (LPARAM) NULL );
}

template <typename chartype>
void TSpinEdit<chartype>::setRange(int min, int max)
{
    SendMessage(DlgObject<chartype>::hwnd, UDM_SETRANGE32, min, max );
}

/* implementation of Form class */
template <typename chartype>
TForm<chartype>::TForm()
{
    DlgObject<chartype>::hwnd = NULL;
}

template <typename chartype>
TForm<chartype>::TForm(HWND window)
{
     DlgObject<chartype>::hwnd = window;
}

template <typename chartype>
TForm<chartype>::TForm(const TForm& oldForm)
{
    DlgObject<chartype>::hwnd = oldForm.hwnd;
}

template <typename chartype>
void TForm<chartype>::Destroy()
{
    DestroyWindow(DlgObject<chartype>::hwnd);
    DlgObject<chartype>::hwnd = NULL;
}

template <typename chartype>
bool TForm<chartype>::createNotifyIcon(unsigned int id, unsigned int msg, HICON icon, std::basic_string<chartype> text)
{
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID = id;
    nid.uCallbackMessage = msg;
    nid.hWnd=DlgObject<chartype>::hwnd;
    nid.hIcon = icon;
    strcpy(nid.szTip, text.c_str());
    //strcpy(nid.szInfo, "INfo");
    //strcpy(nid.szInfoTitle, "Shutdown");
    //nid.uTimeout = 1;
    //nid.dwInfoFlags = NIF_INFO;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // | NIF_INFO;
    return Shell_NotifyIcon(NIM_ADD, &nid);
}

template <typename chartype>
bool TForm<chartype>::deleteNotifyIcon(unsigned int id)
{
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID = id;
    nid.hWnd=DlgObject<chartype>::hwnd;
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

template <typename chartype>
bool TForm<chartype>::updateNotifyIconText(unsigned int id, std::basic_string<chartype> text)
{
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.uID = id;
    nid.hWnd=DlgObject<chartype>::hwnd;
    strcpy(nid.szTip, text.c_str());
    nid.uFlags = NIF_TIP;
    return Shell_NotifyIcon(NIM_MODIFY, &nid);
}

/* implementation of Frame class */
template <typename chartype>
TFrame<chartype>::TFrame()
{
    DlgObject<chartype>::hwnd = NULL;
}

template <typename chartype>
TFrame<chartype>::TFrame(HWND window)
{
     DlgObject<chartype>::hwnd = window;
}

template <typename chartype>
TFrame<chartype>::TFrame(const TFrame& oldFrame)
{
    DlgObject<chartype>::hwnd = oldFrame.hwnd;
}

template <typename chartype>
void TFrame<chartype>::addControl(const DlgObject<chartype> &obj)
{
    groupControls.push_back(obj);
}

template <typename chartype>
void TFrame<chartype>::removeControl(const DlgObject<chartype> &obj)
{
    groupControls.push_back(obj);
}

// override show()
template <typename chartype>
void TFrame<chartype>::show()
{
    DlgObject<chartype>::show();
    for(auto control : groupControls)
    {
        control.show();
    }
}

// override hide()
template <typename chartype>
void TFrame<chartype>::hide()
{
    DlgObject<chartype>::hide();
    for(auto control : groupControls)
    {
        control.hide();
    }
}

// override enable()
template <typename chartype>
void TFrame<chartype>::enable()
{
    DlgObject<chartype>::enable();
    for(auto control : groupControls)
    {
        control.enable();
    }
}

// override disable()
template <typename chartype>
void TFrame<chartype>::disable()
{
    DlgObject<chartype>::disable();
    for(auto control : groupControls)
    {
        control.disable();
    }
}

/* define types */
#ifdef UNICODE
typedef TButton<wchar_t> Button;
typedef TLabel<wchar_t> Label;
typedef TEdit<wchar_t> Edit;
typedef TMemo<wchar_t> Memo;
typedef TListBox<wchar_t> ListBox;
typedef TComboBox<wchar_t> ComboBox;
typedef TCheckbox<wchar_t> Checkbox;
typedef TRadioButton<wchar_t> RadioButton;
typedef TProgressBar<wchar_t> ProgressBar;
typedef TSpinEdit<wchar_t> SpinEdit;
typedef TForm<wchar_t> Form;
typedef TFrame<wchar_t> Frame;
#else
typedef TButton<char> Button;
typedef TLabel<char> Label;
typedef TEdit<char> Edit;
typedef TMemo<char> Memo;
typedef TListBox<char> ListBox;
typedef TComboBox<char> ComboBox;
typedef TCheckbox<char> Checkbox;
typedef TRadioButton<char> RadioButton;
typedef TProgressBar<char> ProgressBar;
typedef TSpinEdit<char> SpinEdit;
typedef TForm<char> Form;
typedef TFrame<char> Frame;

#endif

}

#endif
