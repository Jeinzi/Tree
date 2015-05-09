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
#define VERSION "1.3d"
#endif
#ifdef RELEASE
#define VERSION "1.3r"
#endif

using namespace std;

int main()
{
	//Startup
	string Input = "";
	string Command = "";
	PrepareConsole("Tree", VERSION, "Please name the file which should be source of the tree.\nType \"help\" for further information.");

	while (true)
	{
		//Schleife wated auf korrekte Eingabe
		Color(INPUT_COLOR);
		cout << "Tree.exe> ";
		getline(cin, Input, '\n');
		Command = ToLower(GetWord(Input, 0));
		Color(OUTPUT_COLOR);

		if (Command == "exit" || Command == "quit")
		{
			//Beenden durch Eingabe von "exit" oder "quit"
			return(0);
		}
		else if (Command == "help")
		{
			//Kurze Beschreibung und verfügbare Befehle
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
			//Allgemeine Info
			cout << "   Tree Version " << VERSION << endl;
			cout << "   Compiled: " << __DATE__ << ", " << __TIME__ << endl;
			cout << "   (c) 2012 - 2014 Julian Heinzel" << endl << endl;
		}
		else if (Command == "clear" || Command == "cls")
		{
			//Leeren des Konsolenfensters und erneute Ausgabe des Headers
			PrepareConsole("Tree", VERSION, "Please name the file which should be source of the tree.\nType \"help\" for further information.");
		}
		else if (!PathFileExists(Input.c_str()))
		{
			//Fehlermeldung, falls Eingabe kein Befehl war und keinem existenten File entspricht
			cout << "   Did not find file \"" + Input + "\"." << endl << endl;
		}
		else
		{
			//Wiederhertellung der Ordner- und Dateistruktur, wenn das File existiert
			string		Line;
			ifstream	FileIn;
			Progress	ProgressBar;

			//Zählen der Gesamtzahl von Zeilen im File
			int NumberLines = 0;
			cout << "   File found, counting lines..." << endl;
			FileIn.open(Input);
			while (getline(FileIn, Line))
			{
				NumberLines += 1;
			}
			ProgressBar.Reset(NumberLines, 3);

			//Öffnen und überspringen der ersten drei Zeilen
			cout << "   Processing " << NumberLines << " lines of text." << endl << "   ";
			FileIn.close();
			FileIn.open(Input);
			getline(FileIn, Line);
			getline(FileIn, Line);
			getline(FileIn, Line);

			//Wiederherstellung der Ordner- und Dateistruktur
			string Pfad[100];
			Pfad[0] = "C\\";
			if (!PathFileExists("C\\"))
			{
				CreateDirectory("C\\", 0);
			}

			while (getline(FileIn, Line))
			{
				//Durchgehen jeder Zeile in der Datei
				bool	Start = true;
				int		Level = 0;
				int		Length = strlen(Line.c_str());
				int		Space = 0;
				string	NewName = "";

				//Update der Konsole, Abbruch bei 'q' oder 'e'
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

				//Datei- bzw. Ordnername wird aus Zeile extrahiert
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

				//Ist in der Zeile kein Datei- bzw. Ordnername enthalten, wird sie übersprungen, die nächste eingelesen und diese verarbeitet
				if (NewName == "")
				{
					continue;
				}

				//Ist in der Zeile ein '+' oder '\',  so ist es ein Ordner
				//Neuer Pfad wird abgespeichert, Ordner wird erzeugt
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
					//Andernfalls ist es ein File, welches dann erstellt wird
					ofstream FileOut;
					FileOut.open(Pfad[Level] + "\\" + NewName);
					FileOut.close();
				}
			}

			if (ProgressBar.GetPercent() == 100)
			{
				//Meldung bei abgeschlossener Schleife
				cout << endl << "   Generated files from \"" + Input + "\" succesfully." << endl << endl;
			}
		}
	}
	return(0);
}