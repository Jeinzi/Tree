#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "../Utility/Utility.h"
#include "../Progresscounter/Progresscounter.h"
#include "Path.h"

#ifdef _WIN32
	#include <conio.h>
#else
#endif

using namespace std;

const string version = "1.35";
void RestoreTree(string filePath);

int main()
{
	// Initializing console.
	string input = "";
	string command = "";
	PrepareTerminal("Tree", version, "Please name the file which should be source of the tree.\nType \"help\" for further information.");

	// Loop waits for correct input.
	while (true)
	{
		ChangeColor(Color::Input);
		cout << "Tree.exe> ";
		getline(cin, input, '\n');
		command = ToLower(GetWord(input, 0));
		ChangeColor(Color::Output);

		// Parsing.
		if (command == "exit" || command == "quit")
		{
			// Leaving program when "exit" or "quit" has been typed.
			break;
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
			cout << "   Tree Version " << version << endl;
			cout << "   Compiled: " << __DATE__ << ", " << __TIME__ << endl;
			cout << "   (c) 2012 - 2015 Julian Heinzel" << endl << endl;
		}
		else if (command == "clear" || command == "cls")
		{
			// Resets the terminal.
			PrepareTerminal("Tree", version, "Please name the file which should be source of the tree.\nType \"help\" for further information.");
		}
		else if (!PathExists(input))
		{
			// Printing an error, if no command and no valid path has been typed.
			cout << "   Did not find file \"" + input + "\"." << endl << endl;
		}
		else
		{
			RestoreTree(input);
		}
	}

	ChangeColor(Color::White);
	return(EXIT_SUCCESS);
}


// Restores the directory and file structure.
void RestoreTree(string filePath)
{
	string			line;
	ifstream		fileIn;
	Progresscounter	progressBar;
	Path			path;

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
	string fileName = GetFileName(filePath);
	replace(fileName.begin(), fileName.end(), '.', '_');
	path.AddLevel(fileName);
	CreateDirectory(path.GetLevel(0));

	// Loops through every line in the file.
	while (getline(fileIn, line))
	{
		bool	readingName = false;
		int		level = 0;
		int		indention = 0;
		string	newName = "";

		// Update the terminal, quit on 'q' and 'e'
		progressBar.Increment();
		progressBar.Print();


#ifdef _WIN32
		// Aborting the program is supported only under windows.
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
#endif

		if (line.length() == 0)
		{
			break;
		}

		// Extract directory/file name from line.
		for (unsigned int i = 0; i < line.length(); i++)
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
		level = (indention / 4);

		// If there is no valid file or directory name, the next line is beeing evaluated.
		if (newName == "")
		{
			continue;
		}

		// If there is a '+' or a '\' in the given line, the name will be interpreted as a directory.
		// The new path is saved and the directory created.
		path.RemoveLevels(level, path.Depth());
		path.AddLevel(newName);
		if ((line.find('+') != -1) || (line.find("\\") != -1))
		{
			CreateDirectory(path.GetPath());
		}
		else
		{
			// Otherwise a file is listed, which will then be created.
			ofstream fileOut;
			fileOut.open(path.GetPath());
			fileOut.close();
		}
	}

	if (progressBar.GetPercent() == 100)
	{
		// Info if the file has been successfully processed.
		cout << endl << "   Generated files from \"" + filePath + "\" succesfully." << endl << endl;
	}
}
