// osuClick.cpp : Defines the entry point for the console application.
// Will connect to a specified serial connection
// It sends a keyboard press / release signal to the computer
// based on characters received from the serial connection.

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

string configFileName = "click config.txt";

string portname = "COM6";
char key1press = 'z';
char key1release = 'a';
char key2press = 'x';
char key2release = 's';
char quitChar = 'q';
INPUT k1 = { 0 }, k1r = { 0 }, k2 = { 0 }, k2r = { 0 };

void doSetup() {
	//read/create config file
	fstream configFile;
	configFile.open(configFileName.c_str());
	if (configFile.is_open()) {
		string line;
		while (getline(configFile, line)) {
			//correct config line as of is length of 8+ char, comment lines start with "//"
			if (line.substr(0, 2) != "//" && line.length() >= 8) {
				string tag = line.substr(0, 6); //name of configuration to set
				string setto = line.substr(7); //value to set to
				cout << "read: " + tag + "=" + setto << '\n';
				if (tag == "press1") {
					key1press = setto.at(0);;
				}
				else if (tag == "rleas1") {
					key1release = setto.at(0);;
				}
				else if (tag == "press2") {
					key2press = setto.at(0);;
				}
				else if (tag == "rleas2") {
					key2release = setto.at(0);;
				}
				else if (tag == "cmport") {
					portname = "\\\\.\\" + setto;
				}
				else {
					cout << "what is " + tag + " ?" << '\n';
				}
			}
		}
	}
	else {
		cout << "Could not open config file, I would make one for you but that's not a feature. Sorry" << '\n';
	}



	//instantiate and set all the input signals
	k1.type = INPUT_KEYBOARD;
	k1.ki.wVk = 0x5A;
	k1.ki.dwFlags = 0;

	k2.type = INPUT_KEYBOARD;
	k2.ki.wVk = 0x58;
	k2.ki.dwFlags = 0;

	k1r.type = INPUT_KEYBOARD;
	k1r.ki.wVk = 0x5A;//release key 1
	k1r.ki.dwFlags = KEYEVENTF_KEYUP;

	k2r.type = INPUT_KEYBOARD;
	k2r.ki.wVk = 0x58;//release key 2
	k2r.ki.dwFlags = KEYEVENTF_KEYUP;
}

// send key signal to Press/Release a keyboard key depending on char passed as parameter
void click(char receivedChar) {
	if (receivedChar == key1press) {
		::SendInput(1, &k1, sizeof(INPUT));
	}
	else if (receivedChar == key2press) {
		::SendInput(1, &k2, sizeof(INPUT));
	}
	else if (receivedChar == key1release) {
		::SendInput(1, &k1r, sizeof(INPUT));
	}
	else if (receivedChar == key2release) {
		::SendInput(1, &k2r, sizeof(INPUT));
	}

}

int main(int argc, char** argv) {
	HANDLE handle; //a handle for the connection
	DCB config; //comm state stuff
	DWORD eventMask;

	doSetup();

	handle = CreateFileA(portname.c_str(), //name of serial port  
		GENERIC_READ, //read only
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (handle == INVALID_HANDLE_VALUE) {
		cout << "Error (getting handle)";
		return 0;
	}
	//set configuration
	if (GetCommState(handle, &config)) {
		config.BaudRate = 9600;
		config.ByteSize = 8;
		config.fBinary = TRUE;
		config.fParity = TRUE;
		config.Parity = NOPARITY;
		config.StopBits = ONESTOPBIT;
	}
	else {
		cout << "Error (getting comm state)" << "\n";
	}
	if (!SetCommState(handle, &config)) {
		cout << "Error (setting comm state)" << "\n";
	}

	//set event to when a char is received
	if (!SetCommMask(handle, EV_RXCHAR)) {
		cout << "Error (setting comm events)" << "\n";
	}

	if (WaitCommEvent(handle, &eventMask, NULL)) {
		char buffer = ' ';
		DWORD readSize;
		int errorCount = 0;

		while (buffer != quitChar) {// keep reading until the char read is the one to quit
			if (ReadFile(handle, &buffer, 1, &readSize, NULL) != 0) {
				if (readSize > 0) {
					click(buffer);
				}
			}
			else {
				if (errorCount++ > 16)
					break;
				cout << "wtf ";
			}

		}
	}
	else {
		std::cout << "Error (Waiting for event)" << "\n";
	}

	//closing connection
	if (handle != INVALID_HANDLE_VALUE) {
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
		std::cout << "Port closed!";
	}
	return 0;
}


