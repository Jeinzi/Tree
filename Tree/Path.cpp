#include "Path.h"

// Initialize all stuff. Parameter path defaults to "".
Path::Path(std::string path)
{
	delimiter = "\\";
	if (!path.empty())
	{
		Path::Parse(path);
	}
}

// Returns the path as a string, separated by delimiter.
std::string Path::GetPath()
{
	std::string path;
	for (unsigned int i = 0; i < Path::levels.size(); i++)
	{
		path += Path::levels.at(i);
		if (i + 1 < Path::levels.size())
		{
			path += delimiter;
		}
	}
	return(path);
}

// Adds a new level.
void Path::AddLevel(std::string levelName)
{
	Path::levels.push_back(levelName);
}

// Gets the identifier of the specified level, or returns "" if the level does not exist.
std::string Path::GetLevel(unsigned int level)
{
	std::string levelIdentifier;
	if (Path::levels.empty() || level >= Path::levels.size())
	{
		levelIdentifier = "";
	}
	else
	{
		levelIdentifier = Path::levels.at(level);
	}
	return(levelIdentifier);
}

// Removes the specified levels. Span defaults to 1.
void Path::RemoveLevels(unsigned int level, unsigned int span)
{
	if (level + span > Path::levels.size())
	{
		span = Path::levels.size() - level;
	}

	if (level < Path::levels.size())
	{
		Path::levels.erase(Path::levels.begin() + level, Path::levels.begin() + level + span);
	}
}

// Deletes the levels vector.
void Path::Clear()
{
	Path::levels.clear();
}

// Returns the number of levels in the path.
unsigned int Path::Depth()
{
	return(Path::levels.size());
}

// Parses the given string into the datastructure.
void Path::Parse(std::string path)
{
	// Save the old levels vector.
	int pos;
	std::string token;
	levels.clear();

	// Parse the string into the levels vector.
	do
	{
		pos = path.find(delimiter);
		//if (pos == std::string::npos)
		//{
		//	pos = path.length();
		//}
		token = path.substr(0, pos);
		path.erase(0, pos + delimiter.length());
		if (!token.empty())
		{
			levels.push_back(token);
		}
	} while (pos != std::string::npos);
}