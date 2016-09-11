/* 
 * File:   main.cpp
 * 
 * Will connect to a specified serial connection
 * It sends a keyboard press/release signal to the computer
 * based on characters received from the serial connection.
 * 
 * 
 * 
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

// Press/Release a keyboard key depending on char passed as parameter
void click(char receivedChar){
    INPUT Input= {0};
    Input.type=INPUT_KEYBOARD;
    if(receivedChar=='z'){
        Input.ki.wVk=0x5A;//press z
        Input.ki.dwFlags=0;
    }else if(receivedChar=='x'){
        Input.ki.wVk=0x58;//press x
        Input.ki.dwFlags=0;
    }else if(receivedChar=='a'){
        Input.ki.wVk=0x5A;//press z
        Input.ki.dwFlags=KEYEVENTF_KEYUP;
    }else{
        Input.ki.wVk=0x58;//press x
        Input.ki.dwFlags=KEYEVENTF_KEYUP;
    }
    ::SendInput(1,&Input,sizeof(INPUT));
}

int main(int argc, char** argv) {
    HANDLE handle; //a handle for the connection
    DCB config; //comm state stuff
    DWORD eventMask;
    
    handle = CreateFile( "COM6", //name of serial port  
                    GENERIC_READ, //read only
                    0, 
                    NULL, 
                    OPEN_EXISTING,
                    0,
                    NULL);
    
    if (handle == INVALID_HANDLE_VALUE){
        cout << "Error (getting handle)";
        return 0;
    }
    //set configuration
    if(GetCommState(handle, &config)){
        config.BaudRate = 9600; 
        config.ByteSize = 8;
        config.fBinary = TRUE;
        config.fParity = TRUE;
        config.Parity = NOPARITY;
        config.StopBits = ONESTOPBIT;
    }else{
        cout << "Error (getting comm state)" << "\n";
    }
    if(!SetCommState(handle, &config)){
        cout << "Error (setting comm state)" << "\n";
    }
    
    //set event to when a char is received
    if(!SetCommMask(handle, EV_RXCHAR)){
        cout << "Error (setting comm events)" << "\n" ;
    }
    
    if(WaitCommEvent(handle, &eventMask, NULL)){
        char buffer;
        DWORD readSize;
        int errorCount=0;
        
        while(buffer!='q'){// keep reading until the char read is the one to quit
            if(ReadFile(handle, &buffer, 1, &readSize, NULL) != 0){
                if(readSize > 0){
                    click(buffer);
                }
            }else{
                if(errorCount++ > 16)
                    break;
                cout << "wtf ";
            }
            
        }
    }else{
        std::cout << "Error (Waiting for event)" << "\n";
    }

    //closing connection
    if(handle != INVALID_HANDLE_VALUE){
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        std::cout << "Port closed!";
    }
    return 0;
}


