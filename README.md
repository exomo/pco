# pco
PostCreationObjects Library is a small header only library to make use of WinApi more comfortable.

## What is it for ##
Hardly anybody wants to use Windows API functions in their code to create and work with window controls. But there might be cases where using a full blown Windowing framework (like Qt) is just too much. What I always hated most when doing "windows" with just the WinAPI was writing the code to interact with the controls. Not only does this mean writing similar code over and over again, but it also is very error prone because you have to take care of buffers for text and you have to remember all those stupid 'WM_...', 'EM_', 'LB_' and whatever constants or look them up all the time. That's why I created this little library. It takes care of some of the common interactions with window controls in a more convenient way. And nothing else.

## How to use ##

### Setting up PCO ###
PCO is a single header file. Just include 'PostCreationObjects.hpp' into your source file and you're done.

### ASCII or Unicode ###
The PCO library can be used for whatever encoding your program uses. If 'UNICODE' is defined in you project, the pco classes expect and return std::wstring, else it is just std::string. 

### Basic usage ###
The library provides a class for each of the common control elements. The objects can be created from a window handle and can be casted to window handle implicitly. For most cases you can use an object of one of the PCO classes wherever you would usually use a window handle.

    // Obtain a control's handle and assign it to a button object
    pco::Button myButton = GetDlgItem(hwnd, BUTTON_ID);
    
The objects provide control specific methods to interact with the control. For example, all of the controls have a 'GetText()' method (although for some controls the text is never visible).

    // Get the text of the button
    std::string text = myButton.getText();
    // Set a new button text
    myButton.setText("Click me");
    
For more example take a look into 'pcotest.cpp'. The pco classes are not documented at the moment, but most of the methods should be pretty self-explaining.