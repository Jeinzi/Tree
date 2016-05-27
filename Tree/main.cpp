#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <fcntl.h>
#include "../Utility/Utility.h"
#include "../Progresscounter/Progresscounter.h"
#include "Path.h"

#ifdef _WIN32
// Used for _kbhit() and _getch().
#include <conio.h>
#else
#endif

using namespace std;

const string version = "1.36";
void ResetTerminal();
void RestoreTree(string filePath);

int main()
{
	// Initializing console.
	string input = "";
	string command = "";
	ResetTerminal();

	// Loop waits for correct user input.
	while (true)
	{
		ChangeColor(Color::Input);
		cout << "Tree.exe> ";
		getline(cin, input, '\n');
		command = ToLower(GetWord(input, 0));
		ChangeColor(Color::Output);

		// Parsing.
		if (command == "exit" || command == "quit" || command == "close")
		{
			// Closing application on "exit" or "quit".
			break;
		}
		else if (command == "help")
		{
			// Shows a description of all available commands.
			cout << "   Available Commands:" << endl;
			cout << "   help\t\tShows this page" << endl;
			cout << "   info\t\tGeneral information" << endl;
			cout << "   clear/cls\tClears the screen" << endl;
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
			cout << "   (c) 2012 - 2016 Julian Heinzel" << endl << endl;
		}
		else if (command == "clear" || command == "cls")
		{
			// Resets the terminal.
			ResetTerminal();
		}
		else if (!PathExists(input))
		{
			// Printing an error, if neither a command nor a valid path has been typed.
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

void ResetTerminal()
{
	string description("Please name the file which should be source of the tree.\nType \"help\" for further information.");
	PrepareTerminal("Tree", version, description);
}


// Restores the directory and file structure.
void RestoreTree(string filePath)
{
	string line;
	ifstream fileIn;
	Progresscounter progressBar;
	Path path;

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
	for (int i = 0; i < 3; i++)
		getline(fileIn, line);

	// Get the filename, replace '.' by '_', set the resulting string as root folder.
	string rootFolder = GetFileName(filePath);
	replace(rootFolder.begin(), rootFolder.end(), '.', '_');
	// Check if there is an equally named file and adapt the root folder name.
	if (IsFile(rootFolder))
	{
		rootFolder += "_Dir";
	}
	path.AddLevel(rootFolder);
	CreateDirectory(path.GetLevel(0));

	// The behaviour of the terminal needs to be changed when using linux
	// in order to check if a key has been pressed, without
	// blocking the thread and printing the char.
#ifdef __linux__
	// Disable echo and waiting for user input.
	// See Wait() for detailed comments.
	termios oldSettings, newSettings;
	tcgetattr(STDIN_FILENO, &oldSettings);

	newSettings = oldSettings;
	newSettings.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

	// Using fcntl() in fcntl.h to manipulate file descriptor.
	// F_GETFL:	Get file descriptor of standard in-/output.
	int oldFileDescriptor = fcntl(STDIN_FILENO, F_GETFL);
	// Set new file descriptor with O_NONBLOCK flag disabled.
	// O_NONBLOCK:	No operation on the file descriptor
	// 		will cause the calling process to wait.
	fcntl(STDIN_FILENO, F_SETFL, oldFileDescriptor | O_NONBLOCK);
#endif

	// Loops through every line in the file.
	while (getline(fileIn, line))
	{
		bool readingName = false;
		int level = 0;
		int indention = 0;
		string newName = "";

		// Update the terminal, quit on 'q' and 'e'
		progressBar.Increment();
		progressBar.Print();

		char c = 0;
#ifdef _WIN32
		if (_kbhit())
			c = _getch();
#else
		chr = getchar();
#endif
		// Quit application on "q" or "e".
		if (c == 'q' || c == 'e')
		{
			cout << endl << "   Program canceled. Processed "
				<< progressBar.GetCounter() << " of " << progressBar.GetMaxElements()
				<< " lines. (" << progressBar.GetString() << ")" << endl << endl;
			return;
		}

		if (line.length() == 0)
			break;

		// Extract directory/file name from line.
		for (unsigned int i = 0; i < line.length(); i++)
		{
			unsigned char c = line[i];
			if ((c != ' ') && (c != '-') && (c != '\\') && (c != '|') && (c != '+') && (readingName == false))
				readingName = true;

			if (readingName == true && isprint(c))
				newName += c;
			else
				indention++;
		}
		level = indention / 4;

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

	// Reset terminal to previous settings when running on linux.
#ifdef __linux__
	tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
	fcntl(STDIN_FILENO, F_SETFL, oldFileDescriptor);
#endif
}