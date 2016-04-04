#include <windows.h>
#include <cmath> // Only needed to round minute decimals so we don't deal with seconds
#include <string>
#define IDI_DISPLAYICON 1
#define ID_FILE_ABOUT 9001
#define ID_FILE_EXIT 9002
#define ID_RADIO_ONEANDAHALF 9003
#define ID_RADIO_DOUBLE 9004
#define ID_BUTTON_CALCULATE 9005
#define ID_COMBO_BEGANHOURS 9006
#define ID_COMBO_BEGANMINUTES 9007
#define ID_COMBO_TIMEPERIOD 9008
#define ID_COMBO_CLASSHOURS 9009
#define ID_COMBO_CLASSMINUTES 9010
#define ID_EDIT_RESULT 9011
#define ID_FILE_HELP 9012

const char g_szClassName[] = "TimeCalculator"; // The name of our window class

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
std::string CalculateTestTime(int &hourIndex, int &minuteIndex, int &periodIndex, int &extensionIndex, int &classHourIndex, int &classMinuteIndex); // We'll pass by reference since these values are reset every time the user clicks "Calculate"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    
    // Step 1: Registering The Window Class
    wc.cbSize = sizeof(WNDCLASSEX); // Size of the structure
    wc.style = 0; // Class Styles (CS_*) not to be confused with Window Styles (WS_*)
    wc.lpfnWndProc = WndProc; // Pointer to the window procedure for this window class
    wc.cbClsExtra = 0; // Amount of extra data allocated for this class in memory. Usaully 0
    wc.cbWndExtra = 0; // Amount of extra data allocated per window of this type. Usaully 0
    wc.hInstance = hInstance; // Handle to application instance (that we got in the first parameter of WinMain())
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DISPLAYICON)); // Large (usaully 32 x 32) icon shown when the user presses Alt+Tab
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor that will be displayed over our window
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // Background brush to set the color of our window
    wc.lpszMenuName = NULL; // Name of a menu resource to use for the windows with this class
    wc.lpszClassName = g_szClassName; // Name to identify the class with
    wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DISPLAYICON), IMAGE_ICON, 16, 16, 0); // Small (usaully 16 x 16) icon to show in the taskbar and in the top left corner of the window
    
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    // Step 2: Creating The Window
    hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, // Extended windows style
    g_szClassName, // Tells the system what kind of window to create
    "Test Time Calculator", // Text displayed in the caption or title bar on our window
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // Window style parameter
    CW_USEDEFAULT, // X-coordinate for window
    CW_USEDEFAULT, // Y-coordinate for window
    500, 400, // Width and Height of window
    NULL, NULL, hInstance, NULL); // Parent window handle, Menu handle, Application instance handle, Pointer to window creation data
    
    if (hwnd == NULL) { // Check to see if window creation failed by comparing our CreateWindowEx handle to null
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    
    ShowWindow(hwnd, nCmdShow); // After creating and checking to see if we have a valid handle to the window show the window  using the last parameter in 
    UpdateWindow(hwnd); // Update it to ensure that it has properly redrawn its self on the screen, also immediately sends WM_PAINT after updating
    
    // Step 3: The Message Loop
    // The heart of the program
    while (GetMessage(&Msg, NULL, 0, 0) > 0) { // Gets a message from your application's message queue. Anytime the user moves the mouse, types on the keyboard, clicks on your windows menu, or does any number of other things, messages are genereated, if there is no message waits until there is a message and then returns it to you
        if (!IsDialogMessage(hwnd, &Msg)) { // Check to see if the message is a dialogue message and process it (i.e. Tab Stops)
            TranslateMessage(&Msg); // Does some additional processing on keyboard events like generating WM_KEYDOWN messages
            DispatchMessage(&Msg); // Sends the message out to the window that the message was sent to
        }
    }
    
    return 0;
}

// Step 4: The Window Procedure
// The brain
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { // The window procedure is called for each message, hwnd is the handle of the specfic window
    switch(msg) {
        case WM_CLOSE: // Sent when user hits the 'x' button or presses Alt-F4
            DestroyWindow(hwnd);
        break;
        
        case WM_DESTROY: // DestroyWindow sends a WM_DESTROY message to the window getting destroyed and then destroys any remaining child windows before removing our windows
            PostQuitMessage(0); // Posts the WM_QUIT message to the message loop. We never recieve this message because because it causes GetMessage() to return false
        break; // and as you see in our message loop code when that happens we stop processing messages and return the final result code, the wParam of WM_QUIT which happens to be
        // the value we passed into PostQuitMessage()
        case WM_CREATE: { // Sent when an application requests a window be created by calling CreateWindowEx() or CreateWindow()
            HMENU hMenu, hSubMenu;
            
            HDC myWindc = GetDC(hwnd); // Get the current window device context to get the logical y pixels per inch
            long lfHeight = -MulDiv(12, GetDeviceCaps(myWindc, LOGPIXELSY), 72); // GetDeviceCaps will get the logical y pixels per inch, MulDiv multiplies the first two numbers then divides by the third (32 bit * 32 bit = 64 bit / 32 bit)
            
            HFONT myFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas"); // Make times new roman font at 12 point with clear type quality
            
            SelectObject(myWindc, myFont);
            SendMessage(hwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            hMenu = CreateMenu();
            
            hSubMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "&File");
            AppendMenu(hSubMenu, MF_STRING, ID_FILE_ABOUT, "&About");
            AppendMenu(hSubMenu, MF_STRING, ID_FILE_HELP, "&Help");
            AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit"); // The & before a letter highlights it when the user presses alt in the menu, this is known as a menu access key
            
            SetMenu(hwnd, hMenu);
            
            // Combobox portion of code
            // First make the combobox to handle the hour box
            HWND comboBoxHourhwnd;
            comboBoxHourhwnd = CreateWindow("COMBOBOX", 
                                    TEXT(""),
                                    CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_SORT | WS_TABSTOP | WS_GROUP,
                                    275, 10,
                                    50, 350,
                                    hwnd, (HMENU) ID_COMBO_BEGANHOURS, NULL, NULL);
            
            SendMessage(comboBoxHourhwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            // Add values to the hourTime combobox
            TCHAR hourTime[12] [3] = {TEXT("01"), TEXT("02"), TEXT("03"), TEXT("04"), TEXT("05"), TEXT("06"),
            TEXT("07"), TEXT("08"), TEXT("09"),  TEXT("10"), TEXT("11"), TEXT("12")
            };
            
            for (int i = 0; i < 12; i++) {
                SendMessage(comboBoxHourhwnd, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) hourTime[i]);
            }
            // Change the default selection to index 11
            SendMessage(comboBoxHourhwnd, (UINT) CB_SETCURSEL, (WPARAM) 11, (LPARAM) 0);
            
            ShowWindow(comboBoxHourhwnd, SW_SHOWDEFAULT);
            UpdateWindow(comboBoxHourhwnd);
            // End hour box
            // Now make the combobx for seconds
            HWND comboBoxMinutehwnd;
            comboBoxMinutehwnd = CreateWindow("COMBOBOX",
                                              TEXT(""),
                                              CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_SORT | WS_VSCROLL | WS_TABSTOP,
                                              345, 10,
                                              50, 500,
                                              hwnd, (HMENU) ID_COMBO_BEGANMINUTES, NULL, NULL);
                                              
            SendMessage(comboBoxMinutehwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            // Add values to the seconds combobox
            TCHAR minuteTime[60] [3] = {TEXT("00"), TEXT("01"), TEXT("02"), TEXT("03"), TEXT("04"), TEXT("05"), TEXT("06"), TEXT("07"), TEXT("08"), TEXT("09"),
            TEXT("10"), TEXT("11"), TEXT("12"), TEXT("13"), TEXT("14"), TEXT("15"), TEXT("16"), TEXT("17"), TEXT("18"), TEXT("19"), TEXT("20"), TEXT("21"), TEXT("22"), TEXT("23"),
            TEXT("24"), TEXT("25"), TEXT("26"), TEXT("27"), TEXT("28"), TEXT("29"), TEXT("30"), TEXT("31"), TEXT("32"), TEXT("33"), TEXT("34"), TEXT("35"), TEXT("36"), TEXT("37"),
            TEXT("38"), TEXT("39"), TEXT("40"), TEXT("41"), TEXT("42"), TEXT("43"), TEXT("44"), TEXT("45"), TEXT("46"), TEXT("47"), TEXT("48"), TEXT("49"), TEXT("50"), TEXT("51"),
            TEXT("52"), TEXT("53"), TEXT("54"), TEXT("55"), TEXT("56"), TEXT("57"), TEXT("58"), TEXT("59")
            };
            
            for (int i = 0; i < 60; i++) {
                SendMessage(comboBoxMinutehwnd, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) minuteTime[i]);
            }
            // Change the default selection to index 0
            SendMessage(comboBoxMinutehwnd, (UINT) CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);
            
            ShowWindow(comboBoxMinutehwnd, SW_SHOWDEFAULT);
            UpdateWindow(comboBoxMinutehwnd);
            // End seconds combobox
            // Start time period combobox
            HWND comboboxTimePeriodhwnd;
            comboboxTimePeriodhwnd = CreateWindow("COMBOBOX",
                                                   TEXT(""),
                                                   CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_SORT | WS_TABSTOP,
                                                   405, 10,
                                                   50, 150,
                                                   hwnd, (HMENU) ID_COMBO_TIMEPERIOD, NULL, NULL);
                                                   
            SendMessage(comboboxTimePeriodhwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            // Add values to the time period combobox
            TCHAR timePeriod[2] [3] = {TEXT("AM"), TEXT("PM")};
            
            for (int i = 0; i < 2; i++) {
                SendMessage(comboboxTimePeriodhwnd, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) timePeriod[i]);
            }
            // Change the default selection to index 1
            SendMessage(comboboxTimePeriodhwnd, (UINT) CB_SETCURSEL, (WPARAM) 1, (LPARAM) 0);
            
            ShowWindow(comboboxTimePeriodhwnd, SW_SHOWDEFAULT);
            UpdateWindow(comboboxTimePeriodhwnd);
            // End time period combobox
            // End Combobox portion
            // Start two radio buttons and text with first radio button oneAndAHalfButtonhwnd
            HWND oneAndAHalfButtonhwnd;
            oneAndAHalfButtonhwnd = CreateWindow("Button", // Class name
                                                 TEXT("Time and a half (1.5X)"), // The window name, text appears in title bar or on control
                                                 WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP, // Combine multiple styles with |
                                                 275, 60, // X and Y coordinates of the window
                                                 215, 20, // Width and height of window
                                                 hwnd, (HMENU) ID_RADIO_ONEANDAHALF, NULL, NULL);
            SendMessage(oneAndAHalfButtonhwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            ShowWindow(oneAndAHalfButtonhwnd, SW_SHOWDEFAULT);
            UpdateWindow(oneAndAHalfButtonhwnd);
            // End oneAndAHalfButtonhwnd start doubleButtonhwnd
            HWND doubleButtonhwnd;
            doubleButtonhwnd = CreateWindow("Button",
                                            TEXT("Double time (2X)"),
                                            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                                            275, 90,
                                            170, 20,
                                            hwnd, (HMENU) ID_RADIO_DOUBLE, NULL, NULL);
            SendMessage(doubleButtonhwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            ShowWindow(doubleButtonhwnd, SW_SHOWDEFAULT);
            UpdateWindow(doubleButtonhwnd);
            
            if (oneAndAHalfButtonhwnd == NULL || doubleButtonhwnd == NULL) {
                MessageBox(NULL, TEXT("Radio buttons creation failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
            }
            PostMessage(oneAndAHalfButtonhwnd, BM_SETCHECK, (WPARAM) BST_CHECKED, 0); // Initially set the first radio button oneAndAHalfButtonhwnd checked
            PostMessage(doubleButtonhwnd, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0); // Initially set the second radio button to be cleared
            // End radio button creation
            // Make two comboboxes for hours and minutes for class time
            HWND classTimeHourshwnd = CreateWindow("COMBOBOX",
                                                   TEXT(""),
                                                   CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_SORT | WS_TABSTOP | WS_VSCROLL,
                                                   275, 140,
                                                   50, 350,
                                                   hwnd, (HMENU) ID_COMBO_CLASSHOURS, NULL, NULL);
                                                   
            SendMessage(classTimeHourshwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            TCHAR classHours[24] [3] = {TEXT("00"), TEXT("01"), TEXT("02"), TEXT("03"), TEXT("04"), TEXT("05"), TEXT("06"), TEXT("07"), TEXT("08"), TEXT("09"),
            TEXT("10"), TEXT("11"), TEXT("12"), TEXT("13"), TEXT("14"), TEXT("15"), TEXT("16"), TEXT("17"), TEXT("18"), TEXT("19"), TEXT("20"), TEXT("21"), TEXT("22"), TEXT("23")};
            
            for (int i = 0; i < 24; i++) {
                SendMessage(classTimeHourshwnd, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) classHours[i]);
            }
            SendMessage(classTimeHourshwnd, (UINT) CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);
            
            ShowWindow(classTimeHourshwnd, SW_SHOWDEFAULT);
            UpdateWindow(classTimeHourshwnd);
            // End hours combobox
            HWND classTimeMinuteshwnd = CreateWindow("COMBOBOX",
                                                     TEXT(""),
                                                     CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_SORT | WS_TABSTOP | WS_VSCROLL,
                                                     375, 140,
                                                     50, 350,
                                                     hwnd, (HMENU) ID_COMBO_CLASSMINUTES, NULL, NULL);
                                                     
            SendMessage(classTimeMinuteshwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            // Borrow from minuteTime to fill our minutes combobox
            for (int i = 0; i < 60; i++) {
                SendMessage(classTimeMinuteshwnd, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) minuteTime[i]);
            }
            SendMessage(classTimeMinuteshwnd, (UINT) CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);
            
            // Do some error checking on the combobox creation
            if (comboBoxHourhwnd == NULL || comboBoxMinutehwnd == NULL || comboboxTimePeriodhwnd == NULL || classTimeHourshwnd == NULL || classTimeMinuteshwnd == NULL) {
                MessageBox(NULL, TEXT("Combobox creation failed"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
            }
            // Make the calculate button
            HWND calculateButtonhwnd;
            calculateButtonhwnd = CreateWindow("Button",
                                               TEXT("Calculate"),
                                               WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP,
                                               0, 300,
                                               495, 50,
                                               hwnd, (HMENU) ID_BUTTON_CALCULATE, NULL, NULL);
                                               
            if (calculateButtonhwnd == NULL) {
                MessageBox(NULL, TEXT("Calculate button creation failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
            }
            lfHeight = -MulDiv(20, GetDeviceCaps(myWindc, LOGPIXELSY), 72);
            myFont = CreateFont(lfHeight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas");
            
            SendMessage(calculateButtonhwnd, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            ShowWindow(calculateButtonhwnd, SW_SHOWDEFAULT);
            UpdateWindow(calculateButtonhwnd);
            // End calculate button
            // Begin the result edit box
            HWND resultEditBox = CreateWindow("EDIT",
                                              TEXT("12:00 PM"),
                                              ES_READONLY | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                                              275, 240,
                                              200, 50,
                                              hwnd, (HMENU) ID_EDIT_RESULT, NULL, NULL);
                                              
            if (resultEditBox == NULL) {
                MessageBox(NULL, TEXT("Result text box creation failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
            }
            lfHeight = -MulDiv(20, GetDeviceCaps(myWindc, LOGPIXELSY), 72);
            myFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas");
            
            SendMessage(resultEditBox, (UINT) WM_SETFONT, (WPARAM) myFont, TRUE);
            
            ShowWindow(resultEditBox, SW_SHOWDEFAULT);
            UpdateWindow(resultEditBox);
        }
        break;
        
        case WM_COMMAND: {// Sent when a user selects a command item from a menu, when a control sends a notification message to its parent window, or when an accelerator keystroke is translated
            switch(LOWORD(wParam)) {
                case ID_BUTTON_CALCULATE: {
                    if (HIWORD(wParam) == BN_CLICKED) { // Check for a button click message
                        HWND beganMinutehwnd = GetDlgItem(hwnd, ID_COMBO_BEGANMINUTES); // Collect our handles to all the user input boxes
                        HWND beganHourhwnd = GetDlgItem(hwnd, ID_COMBO_BEGANHOURS);
                        HWND timePeriodhwnd = GetDlgItem(hwnd, ID_COMBO_TIMEPERIOD);
                        HWND classMinutehwnd = GetDlgItem(hwnd, ID_COMBO_CLASSMINUTES);
                        HWND classHourhwnd = GetDlgItem(hwnd, ID_COMBO_CLASSHOURS);
                        
                        int beganMinuteIndex = SendMessage(beganMinutehwnd, (UINT) CB_GETCURSEL, 0, 0);
                        int beganHourIndex = SendMessage(beganHourhwnd, (UINT) CB_GETCURSEL, 0, 0);
                        int timePeriodIndex = SendMessage(timePeriodhwnd, (UINT) CB_GETCURSEL, 0, 0);
                        int classMinuteIndex = SendMessage(classMinutehwnd, (UINT) CB_GETCURSEL, 0, 0);
                        int classHourIndex = SendMessage(classHourhwnd, (UINT) CB_GETCURSEL, 0, 0);
                        int extensionIndex;
                        // In order to get the radio buttons index we need to use an if else statement
                        if(IsDlgButtonChecked(hwnd, ID_RADIO_ONEANDAHALF) == BST_CHECKED) { // We need to only check one radio button to get the index
                            extensionIndex = 0;
                        }
                        else {
                            extensionIndex = 1;
                        }
                        
                        std::string displayStr = CalculateTestTime(beganMinuteIndex, beganHourIndex, timePeriodIndex, extensionIndex, classHourIndex, classMinuteIndex);
                        
                        HWND resultBoxhwnd = GetDlgItem(hwnd, ID_EDIT_RESULT);
                        SendMessage(resultBoxhwnd, (UINT) WM_SETTEXT, (WPARAM) 0, (LPARAM) displayStr.c_str());
                    }
                    else {
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                    }
                }
                break;
                
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
                
                case ID_FILE_ABOUT:
                    MessageBox(hwnd, TEXT("The Test Time Calculator program was created by Sean Purcell, 2016\nContact: qwertymarkup@openmailbox.org"), TEXT("About"), MB_ICONASTERISK | MB_OK);
                break;
                
                case ID_FILE_HELP:
                    MessageBox(hwnd, TEXT("1. Enter the time (don't forget AM / PM) when the student began testing.\n\n2. Check the appropriate time extension for the student (time-and-a-half or double).\n\n3. Enter the amount of time the class gets using the \"Minnute(s)\" and \"Hour(s)\" drop down box.\n\n4. Click the \"Calculate\" button at the bottom, the result will be displayed above the \"Calculate\" button."),
                    TEXT("Help"), MB_ICONASTERISK | MB_OK);
                break;
                
                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        }
        break;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC myWindc = BeginPaint(hwnd, &ps); // A WM_PAINT message should always begin like this
            
            long lfHeight = -MulDiv(12, GetDeviceCaps(myWindc, LOGPIXELSY), 72); // GetDeviceCaps will get the logical y pixels per inch, MulDiv multiplies the first two numbers then divides by the third (32 bit * 32 bit = 64 bit / 32 bit)
            HFONT myFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas"); // Make times new roman font at 12 point with clear type quality
            SelectObject(myWindc, myFont);
            
            LPCTSTR introText = (LPCTSTR)"Time student began testing:";
            LPCTSTR semiColon = (LPCTSTR)":";
            LPCTSTR timeMultiplicty = (LPCTSTR)"Time extension for student:";
            LPCTSTR classTime = (LPCTSTR)"Amount of time class gets:";
            LPCTSTR hoursString = (LPCTSTR)"Hour(s)";
            LPCTSTR minutesString = (LPCTSTR)"Minute(s)";
            LPCTSTR resultString = (LPCTSTR)"Testing time should end by:";
            
            // Change the font size for the semiColon
            lfHeight = -MulDiv(25, GetDeviceCaps(myWindc, LOGPIXELSY), 72);
            myFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas");
            SelectObject(myWindc, myFont);
            
            bool semiColonCreationFlag = TextOut(myWindc, 325, 2, semiColon, lstrlen(semiColon));
            // Change the font back after the semiColon
            lfHeight = -MulDiv(12, GetDeviceCaps(myWindc, LOGPIXELSY), 72);
            myFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas");
            SelectObject(myWindc, myFont);
            
            bool introTextCreationFlag = TextOut(myWindc, 10, 10, introText, lstrlen(introText));
            bool timeMultiplictyCreationFlag = TextOut(myWindc, 10, 75, timeMultiplicty, lstrlen(timeMultiplicty));
            bool classTimeCreationFlag = TextOut(myWindc, 10, 140, classTime, lstrlen(classTime));
            bool hoursStringCreationFlag = TextOut(myWindc, 275, 170, hoursString, lstrlen(hoursString));
            bool minutesStringCreationFlag = TextOut(myWindc, 375, 170, minutesString, lstrlen(minutesString));
            bool resultStringCreationFlag = TextOut(myWindc, 10, 250, resultString, lstrlen(resultString));
            
            // All of our TextOut error checking in one if statement
            if (!introTextCreationFlag || !semiColonCreationFlag || !timeMultiplictyCreationFlag || !hoursStringCreationFlag || !minutesStringCreationFlag || !resultStringCreationFlag) {
                MessageBox(NULL, "TextOut creation failed!", "Weird Error!", MB_ICONEXCLAMATION | MB_OK);
            }
            
            EndPaint(hwnd, &ps); // A WM_PAINT message should always end like this
        }
        break;
        
        case WM_CTLCOLORSTATIC: // Used to set the background of the static text for radio buttons
            SetBkMode((HDC)wParam, TRANSPARENT);
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        break;
        
        break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

std::string CalculateTestTime(int &minuteIndex, int &hourIndex, int &periodIndex, int &extensionIndex, int &classHourIndex, int &classMinuteIndex) {
    hourIndex++; // Offsets the index value being one lower than the actual value
    
    long double timeVal = 0.0; // We create a long double value to first hold the extension value then to hold the result of that multiplied by the current time
    unsigned int timeValInt = 0;
    int endHour = 0, endMinute = 0;
    std::string periodStr = "", returnStr = "";
    
    if (hourIndex == 12) { // If the hourIndex is set to 12 we set it to zero since 12 is the roll over point for AM and PM
        hourIndex = 0;
    }
    switch(periodIndex) {
        case 0: // Do nothing
        break;
        
        case 1: // Add twelve hours for PM
            hourIndex += 12;
        break;
        
        default:
            MessageBox(NULL, "Time period index out of range!", "Error!", MB_ICONEXCLAMATION | MB_OK); // Get cases that go outside of our periodIndex range
            return "Error!";
    }
    
    switch(extensionIndex) {
        case 0: // 1.5X time and a half
            timeVal = 1.5;
        break;
        
        case 1: // 2X double time
            timeVal = 2.0;
        break;
        
        default:
            MessageBox(NULL, "Time extensions index out of range!", "Error!", MB_ICONEXCLAMATION | MB_OK); // Get cases that go outside of our extensionIndex range
            return "Error!";
    }
    
    classHourIndex *= 60; // Convert the hours the class gets to minutes
    timeVal *= (classHourIndex + classMinuteIndex); // We set timeVal equal to the amount of time in minutes the student testing should get
    timeValInt = round(timeVal); // Here we round the timeVal so we don't have to deal with seconds and then store it into our unsigned int holder, This is the only place we use cmath library
    
    hourIndex *= 60; // Convert the starting time hours to minutes
    timeValInt += (hourIndex + minuteIndex); // We add the time from when we began in minutes to the amount of time the student gets in minutes
    timeValInt %= 1440; // We take the remainded of timeValInt after one day in minutes (24 * 60) in case the students time limit goes over the current day (rare). Other wise this simply returns the value already held in timeValInt
    
    endHour = timeValInt / 60; // We do integer division on timeValInt to get the amount of full hours when the student ends and store it in endHour
    endMinute = timeValInt % 60; // We take the mod to get the minutes and store it in endMinute
    
    if (endHour > 12) { // If it's greater than 12 set periodStr to PM and subtract 12 from endHour
        periodStr = "PM";
        endHour -= 12;
    }
    else if (endHour == 12) { // If it equals 12 set periodStr to PM but do not subtract 12 from endHour
        periodStr = "PM";
    }
    else { // Otherwise endHours < 12 so set periodStr to Am
        periodStr = "AM";
    }
    if (endHour == 0) { // 12:00 AM = 00:00 so we check to see if endHour is at 0 and then set it to 12
        endHour = 12;
    }
    
    // Now we start building the returnStr
    if (endHour < 10) { // Check to see if endHour is less than 10 which means that endHour is 1 digit long
        returnStr = "0"; // Prepend a zero if that is the case
        returnStr += std::to_string(endHour); // Then add the actual digit, in range 1 - 9
    }
    else { // Otherwise endHour is already two digits and we don't need to prepend a 0
        returnStr = std::to_string(endHour);
    }
    returnStr += ":"; // Add the colon in between hours and minutes
    
    if (endMinute < 10) { // We do the same checking for minutes as we did hours
        returnStr += "0";
        returnStr += std::to_string(endMinute);
    }
    else {
        returnStr += std::to_string(endMinute);
    }
    returnStr += " "; // Add a space before the PM / AM
    returnStr += periodStr;
    
    return returnStr;
}