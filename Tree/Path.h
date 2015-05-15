#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>

class Path
{
public:
	// Constructor.
	Path(std::string path = "");

	// Returns the entire path as one string separated by '/'.
	std::string GetPath();

	// Adds a new level to the end of the path.
	void AddLevel(std::string levelName);

	// Returns the identifier at the specified level within the path.
	// When level is out of range, the root or the identifier at the deepest level is returned.
	std::string GetLevel(unsigned int level);

	// Removes a level in the path.
	void RemoveLevels(unsigned int level, unsigned int span = 1);

	// Clears the entire path.
	void Clear();

	// Returns the number of levels within the path.
	unsigned int Depth();

	// Parses a string into the datastructure. Returns false, if an invalid path is passed as a parameter.
	void Parse(std::string path);

private:
	// Vector to store the directory- and filenames at different levels within the path.
	std::vector<std::string> levels;

	// String separating the different levels in the directory/file structure.
	std::string delimiter;
};

#endif