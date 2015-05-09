#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <Shlwapi.h>
#include "General.h"
#include "Progress.h"
#pragma comment(lib, "shlwapi.lib")

#ifdef DEBUG
#define VERSION "1.31d"
#endif
#ifdef RELEASE
#define VERSION "1.31r"
#endif

using namespace std;

int main()
{
	// Initializing console.
	string Input = "";
	string Command = "";
	PrepareConsole("Tree", VERSION, "Please name the file which should be source of the tree.\nType \"help\" for further information.");

	// Loop waits for correct input.
	while (true)
	{
		Color(INPUT_COLOR);
		cout << "Tree.exe> ";
		getline(cin, Input, '\n');
		Command = ToLower(GetWord(Input, 0));
		Color(OUTPUT_COLOR);

		// Parsing.
		if (Command == "exit" || Command == "quit")
		{
			// Leaving program when "exit" or "quit" has been typed.
			return(0);
		}
		else if (Command == "help")
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
		else if (Command == "info")
		{
			// Shows general info about the software.
			cout << "   Tree Version " << VERSION << endl;
			cout << "   Compiled: " << __DATE__ << ", " << __TIME__ << endl;
			cout << "   (c) 2012 - 2014 Julian Heinzel" << endl << endl;
		}
		else if (Command == "clear" || Command == "cls")
		{
			// Resets the terminal.
			PrepareConsole("Tree", VERSION, "Please name the file which should be source of the tree.\nType \"help\" for further information.");
		}
		else if (!PathFileExists(Input.c_str()))
		{
			// Printing an error, if no command and no valid path has been typed.
			cout << "   Did not find file \"" + Input + "\"." << endl << endl;
		}
		else
		{
			// Restoring the directory and file structure.
			string		Line;
			ifstream	FileIn;
			Progress	ProgressBar;

			// Counting the total number of lines in the specified textfile.
			int NumberLines = 0;
			cout << "   File found, counting lines..." << endl;
			FileIn.open(Input);
			while (getline(FileIn, Line))
			{
				NumberLines += 1;
			}
			ProgressBar.Reset(NumberLines, 3);

			// Opening file and skipping the first three lines.
			cout << "   Processing " << NumberLines << " lines of text." << endl << "   ";
			FileIn.close();
			FileIn.open(Input);
			getline(FileIn, Line);
			getline(FileIn, Line);
			getline(FileIn, Line);

			// Creating directories and files.
			string Pfad[100];
			Pfad[0] = "C\\";
			if (!PathFileExists("C\\"))
			{
				CreateDirectory("C\\", 0);
			}

			// Loops through every line in the file.
			while (getline(FileIn, Line))
			{
				bool	Start = true;
				int		Level = 0;
				int		Length = strlen(Line.c_str());
				int		Space = 0;
				string	NewName = "";

				// Update the terminal, quit on 'q' and 'e'
				ProgressBar.Increment();
				ProgressBar.Print();
				if (_kbhit())
				{
					char C = _getch();
					if (C == 'q' || C == 'e')
					{
						cout << endl << "   Program canceled. Processed " << ProgressBar.GetCounter() << " of " << ProgressBar.GetMaxElements() << " lines. (" << ProgressBar.GetString() << ")" << endl << endl;
						break;
					}
				}

				if (Length == 0)
				{
					break;
				}

				// Extract directory/file name from line.
				for (int i = 0; i < Length; i++)
				{
					char Character = Line[i];
					if ((Character != ' ') && (Character != '-') && (Character != '\\') && (Character != '|') && (Character != '+') && (Start = true))
					{
						Start = false;
					}
					if (Start == true)
					{
						Space++;
					}
					if (Start == false)
					{
						NewName += Character;
					}
				}
				Start = true;
				Level = ((Space / 4) - 1);

				// If there is no valid file or directory name, the next line is beeing evaluated.
				if (NewName == "")
				{
					continue;
				}

				// If there is a '+' or a '\' in the given line, the name will be interpreted as a directory.
				// The new path is saved and the directory created.
				if ((Line.find('+') != -1) || (Line.find("\\") != -1))
				{
					Level += 1;
					Pfad[Level] = Pfad[Level - 1] + "\\" + NewName;
					if (PathFileExists(Pfad[Level - 1].c_str()))
					{
						if (!PathFileExists(Pfad[Level].c_str()))
						{
							CreateDirectory(Pfad[Level].c_str(), 0);
						}
					}
				}
				else
				{
					// Otherwise a file is listed, which will then be created.
					ofstream FileOut;
					FileOut.open(Pfad[Level] + "\\" + NewName);
					FileOut.close();
				}
			}

			if (ProgressBar.GetPercent() == 100)
			{
				// Info if the file has been successfully processed.
				cout << endl << "   Generated files from \"" + Input + "\" succesfully." << endl << endl;
			}
		}
	}
	return(0);
}