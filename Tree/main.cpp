#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <Shlwapi.h>
#include <stdlib.h>
#include <algorithm>
#include "General.h"
#include "Progress.h"
#pragma comment(lib, "shlwapi.lib")

#ifdef DEBUG
	#define VERSION "1.32d"
#endif
#ifdef RELEASE
	#define VERSION "1.32r"
#endif

using namespace std;

void RestoreTree(string filePath);

int main()
{
	// Initializing console.
	string input = "";
	string command = "";
	PrepareConsole("Tree", VERSION, "Please name the file which should be source of the tree.\nType \"help\" for further information.");

	// Loop waits for correct input.
	while (true)
	{
		Color(INPUT_COLOR);
		cout << "Tree.exe> ";
		getline(cin, input, '\n');
		command = ToLower(GetWord(input, 1));
		Color(OUTPUT_COLOR);

		// Parsing.
		if (command == "exit" || command == "quit")
		{
			// Leaving program when "exit" or "quit" has been typed.
			return(EXIT_SUCCESS);
		}
		else if (command == "help")
		{
			// Shows a description of all available commands.
			cout << "   Available Commands:" << endl;
			cout << "   help\t\tShows this page" << endl;
			cout << "   info\t\tGeneral information" << endl;
			cout << "   clear/cls\tClears the Screen" << endl;
			cout << "   exit\t\tTerminates the program" << endl << endl;
			cout << "   Enter any other string to designate the file containing the tree." << endl;
			cout << "   The program will create the listed files and directories," << endl;
			cout << "   until you press 'q' or the file reaches its end." << endl << endl;
		}
		else if (command == "info")
		{
			// Shows general info about the software.
			cout << "   Tree Version " << VERSION << endl;
			cout << "   Compiled: " << __DATE__ << ", " << __TIME__ << endl;
			cout << "   (c) 2012 - 2015 Julian Heinzel" << endl << endl;
		}
		else if (command == "clear" || command == "cls")
		{
			// Resets the terminal.
			PrepareConsole("Tree", VERSION, "Please name the file which should be source of the tree.\nType \"help\" for further information.");
		}
		else if (!PathFileExists(input.c_str()))
		{
			// Printing an error, if no command and no valid path has been typed.
			cout << "   Did not find file \"" + input + "\"." << endl << endl;
		}
		else
		{
			RestoreTree(input);
		}
	}
	return(EXIT_SUCCESS);
}


// Restores the directory and file structure.
void RestoreTree(string filePath)
{
	string		line;
	ifstream	fileIn;
	Progress	progressBar;

	// Counting the total number of lines in the specified textfile.
	int numberOfLines = 0;
	cout << "   File found, counting lines..." << endl;
	fileIn.open(filePath);
	while (getline(fileIn, line))
	{
		numberOfLines += 1;
	}
	progressBar.Reset(numberOfLines, 3);

	// Opening file and skipping the first three lines.
	cout << "   Processing " << numberOfLines << " lines of text." << endl << "   ";
	fileIn.close();
	fileIn.open(filePath);
	for (int i = 0; i < 3; i++) getline(fileIn, line);

	// Get the filename, replace '.' by '_', set the resulting string as root folder.
	string path[100];
	string fileName = GetFileName(filePath);
	replace(fileName.begin(), fileName.end(), '.', '_');
	path[0] = fileName;
	CreateDirectory(path[0].c_str(), 0);

	// Loops through every line in the file.
	while (getline(fileIn, line))
	{
		bool	readingName = false;
		int		level = 0;
		int		length = strlen(line.c_str());
		int		indention = 0;
		string	newName = "";

		// Update the terminal, quit on 'q' and 'e'
		progressBar.Increment();
		progressBar.Print();
		if (_kbhit())
		{
			char C = _getch();
			if (C == 'q' || C == 'e')
			{
				cout << endl << "   Program canceled. Processed "
					<< progressBar.GetCounter() << " of " << progressBar.GetMaxElements()
					<< " lines. (" << progressBar.GetString() << ")" << endl << endl;
				return;
			}
		}

		if (length == 0)
		{
			break;
		}

		// Extract directory/file name from line.
		for (int i = 0; i < length; i++)
		{
			char c = line[i];
			if ((c != ' ') && (c != '-') && (c != '\\') && (c != '|') && (c != '+') && (readingName == false))
			{
				readingName = true;
			}

			if (readingName == true)
			{
				newName += c;
			}
			else
			{
				indention++;
			}
		}
		level = ((indention / 4) - 1);

		// If there is no valid file or directory name, the next line is beeing evaluated.
		if (newName == "")
		{
			continue;
		}

		// If there is a '+' or a '\' in the given line, the name will be interpreted as a directory.
		// The new path is saved and the directory created.
		if ((line.find('+') != -1) || (line.find("\\") != -1))
		{
			level += 1;
			path[level] = path[level - 1] + "\\" + newName;
			CreateDirectory(path[level].c_str(), 0);
		}
		else
		{
			// Otherwise a file is listed, which will then be created.
			ofstream fileOut;
			fileOut.open(path[level] + "\\" + newName);
			fileOut.close();
		}
	}

	if (progressBar.GetPercent() == 100)
	{
		// Info if the file has been successfully processed.
		cout << endl << "   Generated files from \"" + filePath + "\" succesfully." << endl << endl;
	}
}