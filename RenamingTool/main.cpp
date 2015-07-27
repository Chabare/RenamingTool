#include "dirent.h"
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>

#define MAX_INT 2147483647

std::list<std::string> &split(const std::string &s, char delim, std::list<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);

	return elems;
}

/**
@param &s String to split
@param delim Delimiter to split the string
@return A list of strings divided by delim
*/
std::list<std::string> split(const std::string &s, char delim) {
	std::list<std::string> elems;
	split(s, delim, elems);
	return elems;
}

/**
Checks whether a list contains a certain string.

@param string String to search
@param list List to check
@return String is in list
*/
bool contains(std::string string, std::list<std::string> list) {
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		if (*it == string)
			return true;

	return false;
}

bool contains(std::string string, std::string containing) {
	return string.find_first_of(containing) != string.npos;
}

bool contains(std::string string, char containing) {
	return string.find_first_of(containing) != string.npos;
}

bool containsOnce(std::string string, std::string containing) {
	return string.find_first_of(containing) != string.npos && string.find_first_of(containing) + containing.length() + 2, string.length() < string.length() && (string.substr(string.find_first_of(containing) + containing.length() + 2, string.length()).find_first_of(containing) == string.npos);
}

/**
Checks whether a string starts with a certain prefix

@param string String to check
@param prefix Prefix to check for
@return Whether prefix is the prefix of string
*/
bool starts_with(std::string string, std::string prefix) {
	return strcmp(string.substr(0, prefix.length()).c_str(), prefix.c_str()) == 0;
}

/**
Checks whether a string ends with a certain suffix

@param string String to check
@param suffix Suffix to check for
@return Whether suffix is the prefix of string
*/
bool ends_with(std::string string, std::string suffix) {
	return strcmp(string.substr(string.length() - suffix.length(), string.length()).c_str(), suffix.c_str()) == 0;
}

/**
Prints a list of strings

@param list List to be printed
*/
void printList(std::list<std::string> list) {
	for (std::list<std::string>::iterator i = list.begin(); i != list.end(); i++)
		std::cout << *i << std::endl;
}

/**
Prints a list of ints

@param list List to be printed
*/
void printList(std::list<int> list) {
	for (std::list<int>::iterator i = list.begin(); i != list.end(); i++)
		std::cout << *i << std::endl;
}

/**
Prints an error.

@param error Error to print
@param exit Whether the program should exit after the error
*/
void printError(std::string error, bool exit) {
	std::cerr << error << std::endl;
	if (exit)
		printError(error, 1);
}

/**
Waits for user input and then exits

@param exitCode The exit code when the program stops
*/
void getAndExit(int exitCode) {
	std::cin.get();
	exit(exitCode);
}

/**
Prints an error.

@param error Error to print
@param exitCode The exit code when the program stops
*/
void printError(std::string string, int exitCode) {
	std::cerr << string << std::endl;

	getAndExit(exitCode);
}
/**
Returns the file type of a file name

@param fileName Name of the file
@return Type of the file
*/
std::string getFileType(std::string fileName) {
	int pointPos = fileName.find_last_of('.');
	if (pointPos == -1)
		return "";

	return fileName.substr(pointPos + 1, fileName.length() - pointPos);
}

/**
Removes the file type and the '.' from a filename

@param fileName Name of the file
@param fileType Type of the file
@return Filename without file type
*/
std::string removeFileType(std::string fileName, std::string fileType) {
	return fileName.substr(0, fileName.length() - (fileType.length() + 1));
}

/**
Removes the file type and the '.' from a filename

@param fileName Name of the file
@return Filename without file type
*/
std::string removeFileType(std::string fileName) {
	return removeFileType(fileName, getFileType(fileName));
}

bool isDigit(char c) {
	return (int)c >= 48 && (int)c <= 57;
}

bool isDigit(std::string str) {
	for (char c : str)
		if (!isDigit(c))
			return false;

	return true;
}

/**
Checks whether given char is alphabetic or not

@param c Character to check
@return Char is alphabetic
*/
bool isAlphabetical(wchar_t c) {
	return ((int)c >= (int)'a' && (int)c <= (int)'z') || ((int)c >= (int)'A' && (int)c <= (int)'Z');
}

bool isAlphabetical(std::string string) {
	for (wchar_t c : string)
		if (isAlphabetical(c))
			return true;

	return false;
}

/**
Removes the leading none (alphabetic) characters from a file

@param fileName Filename to remove leading none (alphabetic) characters from
@return File starting with alphabetic character
*/
std::string removeLeadingNoneCharacters(std::string fileName) {
	while (fileName.size() > 0 && !isAlphabetical(fileName.at(0)))
		fileName = fileName.substr(1, fileName.length() - 1);

	return fileName;
}

/**
Removes the leading none (alphabetic) characters from a list of files

@param list List of filenames to remove leading none (alphabetic) characters from
@return List of filenames starting with alphabetic character
*/
std::list<std::string> removeLeadingNoneCharacters(std::list<std::string> list) {
	for (std::list<std::string>::iterator i = list.begin(); i != list.end(); i++)
		*i = removeLeadingNoneCharacters(*i);

	return list;
}

int getCopyNumber(std::string name, std::string directory, std::string fileType, std::list<std::string> files, std::list<std::string> renamedFiles) {
	int nr = 1;
	name.append(" (1)");

	while (contains(name + "." + fileType, files) || contains(directory + name + "." + fileType, renamedFiles)) {
		nr++;
		name = name.substr(0, name.length() - 2);
		name.append(std::to_string(nr) + ")");
	}

	return nr;
}

std::string removeLeadingWhitespaces(std::string fileName) {
	while (fileName.at(0) == ' ')
		fileName = fileName.substr(1, fileName.length() - 1);

	return fileName;
}

/**
Finds the correct name for the file

@param newName Default new name
@param files List of all files in directory
@param renamedFiles A list of the renamed files
@param directory The directory of the files
@param remLeadingNoneChar Whether leading none alphabetic characters should be removed
@param cap Whether name should be capitalized
@return New name for the file
*/
std::string determineNewFileName(std::string newName, std::list<std::string> files, std::list<std::string> renamedFiles, std::string directory, bool remLeadingNoneChar, bool cap) {
	std::regex regexFilename = std::regex("[^<>:\"/\\|?*]*");
	if (std::regex_match(newName, regexFilename, std::regex_constants::match_default) != 1) {
		printError("Renaming following file failed: " + directory + "\\" + newName + "\nReason: " + strerror(errno) + "\n", false);

		std::cout << "Continuing..." << std::endl << std::endl;

		return directory + newName;
	}

	if (remLeadingNoneChar)
		newName = removeLeadingNoneCharacters(newName);

	std::string fileType = getFileType(newName);
	if (fileType != "")
		newName = removeFileType(newName);
	if ((contains(newName + "." + fileType, files) || contains(directory + newName + "." + fileType, renamedFiles)) && !containsOnce(newName, "- Copy"))
		newName.append(" - Copy");

	if (contains(newName + "." + fileType, files) || contains(directory + newName + "." + fileType, renamedFiles))
		newName.append(" (" + std::to_string(getCopyNumber(newName, directory, fileType, files, renamedFiles))).append(")");

	newName = removeLeadingWhitespaces(newName);
	// Capitalize
	if (cap)
		newName.at(0) = toupper(newName.at(0));

	newName.append("." + fileType);

	return directory + newName;
}

/**
	Determines the length of the prefix

	@param fileName Name of the file
	@return Length of the prefix
*/
int determineAddPrefixLength(std::string fileName) {
	int cnt = 0;

	for (unsigned int i = 0; i < fileName.length() && !isAlphabetical(fileName.at(i)); i++)
		cnt++;

	return cnt;
}

/**
Renames a list of files

@param files List of files to remain
@param prefixLen Length of prefix for each file
@param directory The directory in which the file is
@param remLeadingNoneChar Whether leading none alphabetic characters should be removed
@param defaultName The default name for a file
@param remLeadingNoneChar Whether leading none alphabetic-characters should be removed
@param cap Whether name should be capitalized
*/
void renameFiles(std::list<std::string> files, std::list<std::string> newFiles, std::list<int> prefixLen, std::string directory, std::string defaultName, std::string newNameR, bool remLeadingNoneChar, bool cap) {
	std::list<std::string> renamedFiles;
	std::list<int>::iterator l = prefixLen.begin();
	std::string newName;

	// Iterate through files
	for (std::list<std::string>::iterator i = newFiles.begin(); i != newFiles.end() && l != prefixLen.end(); i++, l++) {
		if (remLeadingNoneChar)
			*l += determineAddPrefixLength(i->substr(0, i->length() - getFileType(*i).length() - 1));
		if (i->length() - *l - getFileType(*i).length() - 1 == 0) {
			newName = defaultName + "." + getFileType(*i);
			*l = 0;
		}
		else
			newName = *i;

		// Rename file
		if (newNameR.length() == 0)
			newName = determineNewFileName(newName.substr(*l, newName.length() - *l), files, renamedFiles, directory, remLeadingNoneChar, cap);
		else
			newName = determineNewFileName(newNameR + "." + getFileType(newName), files, renamedFiles, directory, remLeadingNoneChar, cap);

		if (rename((directory + *i).c_str(), newName.c_str()) != 0)
			printError("Renaming following file failed: " + directory + *i + "\nReason: " + strerror(errno) + "\nNew name: " + newName + "\n", false);
		else
			renamedFiles.push_back(newName);
	}
}

/**
Checks whether the pointer is a directory or not

@param dir Pointer to file/dir
@return Struct is directory
*/
bool isDirectory(struct dirent *dir) {
	return dir->d_type == 16384;
}

std::list<DIR> getDirectories(struct dirent *pent, DIR *dir, std::string directory, std::list<DIR> *dirs) {
	DIR *newDir;

	while (pent = readdir(dir))
		if (isDirectory(pent) && !(strcmp(pent->d_name, ".") == 0 || strcmp(pent->d_name, "..") == 0)) {
			newDir = opendir((directory + "\\" + pent->d_name).c_str());
			if (newDir != NULL) {
				dirs->push_back(*newDir);
				getDirectories(pent, newDir, directory, dirs);
			}
		}

	return *dirs;
}

/**
	Gets subdirectories from given directory
*/
void getDirectoryNames(struct dirent *pent, DIR *dir, std::string directory, std::list<std::string> *dirs, std::list<std::string> excludedFolders, int recDepth, int i) {
	if (i >= recDepth)
		return;
	else
		i++;

	while (pent = readdir(dir))
		if (isDirectory(pent) && !(strcmp(pent->d_name, ".") == 0 || strcmp(pent->d_name, "..") == 0))
			if (!contains(pent->d_name, excludedFolders)) {
				dirs->push_back(directory + "\\" + pent->d_name);
				getDirectoryNames(pent, opendir((directory + "\\" + pent->d_name).c_str()), directory + "\\" + pent->d_name, dirs, excludedFolders, recDepth, i);
			}
}

void getDirectoryNames(struct dirent *pent, DIR *dir, std::string directory, std::list<std::string> *dirs, std::list<std::string> excludedFolders, int recDepth) {
	getDirectoryNames(pent, dir, directory, dirs, excludedFolders, recDepth, 0);
}

/**
Gets the filenames from directory

@param pent Pointer to a struct of type dirent
@param dir Pointer to the directory
@return Returns the files from given directory
*/
std::list<std::string> getFilesFromDir(struct dirent *pent, DIR *dir) {
	std::list<std::string> files;

	// Put the files into the list(not "." and "..")
	while (pent = readdir(dir))
		if (pent == NULL) {
			printError("Sorry, the directory couldn't be read.", 3);
		}
		else
			// Check for '.' and '..' links and don't add them to the file list
			if (!(strcmp(pent->d_name, ".") == 0 || strcmp(pent->d_name, "..") == 0) && !isDirectory(pent))
				files.push_back(pent->d_name);

	return files;
}

/**
Gets the input from user

@param inputReq Message to be displayed to request input from user
@param inputEnd String which needs to be entered to stop input
@param ignoreStart Ignores start character of input (e.g. '.' in '.mp3' -> mp3
@return Returns the input from the user
*/
std::list<std::string> getInput(std::string inputReq, std::string inputEnd, char ignoreStart) {
	std::string input;
	char in[128];
	std::list<std::string> list;

	std::cout << inputReq << std::endl;
	while (input != ".") {
		std::cin.getline(in, sizeof(in));
		input = in;
		if (input != "." && !contains(input, list)) {
			if (ignoreStart != ' ' && input.at(0) == ignoreStart)
				input = input.substr(1, input.length() - 1);
			list.push_back(input);
		}
	}

	return list;
}

/**
Gets the files from list which have one of the given prefixes and match the suffixes(if any)

@param files List of all the files
@param prefixes List of prefixes
@param suffix List of suffixes
@param prexifLen Pointer to an empty list where the length of the prefixes is written to
@param remLeadingNoneChar Whether leading none alphabetic characters should be removed
@return Returns a list of files which have one of the prefixes and match the suffixes
*/
std::list<std::string> getFilesWithPrefixAndSuffix(std::list<std::string> files, std::list<std::string> prefixes, std::list<std::string> suffix, std::list<int> *prefixLen, bool remLeadingNoneChar, bool remTilCertChar, char remUntilChar, bool getAll) {
	std::list<std::string> filesWPrefix;

	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++) {
		if (getAll) {
			filesWPrefix.push_back(*i);
			prefixLen->push_back(0);
		}
		for (std::list<std::string>::iterator j = prefixes.begin(); j != prefixes.end(); j++)
			if (starts_with(*i, *j)) {
				if (suffix.size() > 0) {
					// Iterate through suffixes
					for (std::list<std::string>::iterator s = suffix.begin(); s != suffix.end(); s++)
						if (ends_with(*i, *s)) {
							filesWPrefix.push_back(*i);
							prefixLen->push_back(j->length());
						}
				}
				else {
					filesWPrefix.push_back(*i);
					prefixLen->push_back(j->length());
				}
			}

		if (remLeadingNoneChar && !isAlphabetical(i->at(0)))
			if (isAlphabetical(removeFileType(*i)))
				if (suffix.size() > 0) {
					// Iterate through suffixes
					for (std::list<std::string>::iterator s = suffix.begin(); s != suffix.end(); s++)
						if (ends_with(*i, *s)) {
							filesWPrefix.push_back(*i);
							prefixLen->push_back(0);
						}
				}
				else {
					filesWPrefix.push_back(*i);
					prefixLen->push_back(0);
				}

				if (remTilCertChar)
					if (contains(*i, remUntilChar) && !contains(i->substr(0, i->find_first_of(remUntilChar)), filesWPrefix)) {
						filesWPrefix.push_back(*i);
						prefixLen->push_back(i->find_first_of(remUntilChar) + 1);
					}
	}

	return filesWPrefix;
}

void renameFilesInDirectory(DIR dir, std::string directory, bool showOutput, bool remLeadingNoneChar, std::list<std::string> prefixes, std::list<std::string> suffix, std::list<int> prefixLen, std::string defRename, std::string newName, bool remTilCertChar, char remUntilChar, bool cap) {
	struct dirent *pent = NULL;
	std::list<std::string> files;
	std::list<std::string> filesWPrefix;

	// Check for '\' as end and append if not there
	if (directory.at(directory.length() - 1) != '/' || directory.at(directory.length() - 1) != '\\')
		directory.append("\\");

	files = getFilesFromDir(pent, &dir);
	if (showOutput) {
		std::cout << "Following files are in the given directory: " << std::endl;
		printList(files);
	}

	filesWPrefix = getFilesWithPrefixAndSuffix(files, prefixes, suffix, &prefixLen, remLeadingNoneChar, remTilCertChar, remUntilChar, (newName.length() != 0));

	// Output the files with one of the given prefixes
	if (showOutput) {
		std::cout << "Files found with given prefixes: " << std::endl;
		if (filesWPrefix.size() == 0)
			std::cout << "None" << std::endl;
		printList(filesWPrefix);
	}

	renameFiles(files, filesWPrefix, prefixLen, directory, defRename, newName, remLeadingNoneChar, cap);
}

int main(int argc, char* argv[]) {
	bool capitalize = true;
	bool dirSet = false;
	bool prefSet = false;
	bool recursive = false;
	bool remLeadingNoneChar = false;
	bool remTilCertChar = false;
	bool showOutput = false;
	bool suffSet = false;
	char delimiter = ';';
	char remUntilChar;
	DIR *dir = NULL;
	int recursiveDepth = MAX_INT;
	std::list<int> prefixLen;
	std::list<std::string> dirNames;
	std::list<std::string> excludedFolders;
	std::list<std::string> files;
	std::list<std::string> filesWPrefix;
	std::list<std::string> prefixes;
	std::list<std::string> suffix;
	std::regex regexFilename = std::regex("[^<>:\"/\\|?*]*");
	std::string defaultName = "renamed";
	std::string newName = "";
	struct dirent *pent = NULL;

	// Get command line arguments
	for (int i = 1; i < argc; i += 2) {
		// Command line argument: Capitalize
		if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-capitalize") == 0) {
			if (i + 1 == argc)
				capitalize = true;
			else if (strcmp(argv[i + 1], "true") == 0)
				capitalize = true;
			else if (strcmp(argv[i + 1], "false") == 0)
				capitalize = false;
			else {
				capitalize = true;
				i--;
			}
		}
		// Command line argument: Directory
		else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-directory") == 0) {
			dirNames = split(argv[i + 1], delimiter);

			dirSet = true;
		}
		// Command line argument: Default
		else if (strcmp(argv[i], "-def") == 0 || strcmp(argv[i], "-default") == 0) {
			if (std::regex_match(argv[i + 1], regexFilename, std::regex_constants::match_default) == 1)
				defaultName = argv[i + 1];
			else
				printError("The default name doesn't pass the naming conventions for filenames.", false);
		}
		// Command line argument: Delimiter
		else if (strcmp(argv[i], "-delim") == 0 || strcmp(argv[i], "-delimiter") == 0) {
			delimiter = *argv[i + 1];
		}
		// Command line argument: None char
		else if (strcmp(argv[i], "-nc") == 0 || strcmp(argv[i], "-nonechar") == 0) {
			if (i + 1 == argc)
				remLeadingNoneChar = true;
			else if (strcmp(argv[i + 1], "true") == 0)
				remLeadingNoneChar = true;
			else if (strcmp(argv[i + 1], "false") == 0)
				remLeadingNoneChar = false;
			else {
				remLeadingNoneChar = true;
				i--;
			}
		}
		// Command line argument: Output
		else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0) {
			if (i + 1 == argc)
				showOutput = true;
			else if (strcmp(argv[i + 1], "true") == 0)
				showOutput = true;
			else if (strcmp(argv[i + 1], "false") == 0)
				showOutput = false;
			else {
				showOutput = true;
				i--;
			}
		}
		// Command line argument: Prefix
		else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "-prefix") == 0) {
			prefixes = split(argv[i + 1], delimiter);
			if (prefixes.size() > 0)
				prefSet = true;
		}
		// Command line argument: Recursive
		else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-recursive") == 0) {
			if (i + 1 == argc)
				recursive = true;
			else if (strcmp(argv[i + 1], "true") == 0)
				recursive = true;
			else if (strcmp(argv[i + 1], "false") == 0)
				recursive = false;
			else {
				recursive = true;
				i--;
			}
		}
		// Command line argument: Recursive depth
		else if (strcmp(argv[i], "-rd") == 0 || strcmp(argv[i], "-recursivedepth") == 0) {
			if (isDigit(argv[i + 1]))
				if (atoi(argv[i + 1]) > 0)
					recursiveDepth = atoi(argv[i + 1]);
		}
		// Command line argument: Recursive exlusion
		else if (strcmp(argv[i], "-re") == 0 || strcmp(argv[i], "-recusriveexclude") == 0) {
			excludedFolders = split(argv[i + 1], delimiter);
		}
		// Command line argument: Suffix
		else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-suffix") == 0) {
			if (strcmp(argv[i + 1], "any") != 0)
				suffix = split(argv[i + 1], delimiter);

			suffSet = true;
		}
		// Command line argument: To 'filename'
		else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-to") == 0) {
			newName = argv[i + 1];
		}
		// Command lind argument: Until character
		else if (strcmp(argv[i], "-uc") == 0 || strcmp(argv[i], "-untilcharacter") == 0) {
			remTilCertChar = true;
			remUntilChar = argv[i + 1][0];
		}
		// Command line argument: Help
		else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "?") == 0) {
			std::cout << "-d,\t-directory\tThe directory you want to be in." << std::endl;
			std::cout << "-def,\t-default\tThe filename which is defaulted to if there are problems with the one without prefix." << std::endl;
			std::cout << "-delim,\t-delimiter\tSets the delimiter for the prefixes and suffixes input." << std::endl;
			std::cout << "-nc,\t-nonechar\tDeletes none-alphabetic characters from start of the file." << std::endl;
			std::cout << "-o,\t-output\t\tWhether the program will generate output or not(Doesn't need an argument)." << std::endl;
			std::cout << "-p,\t-prefix\t\tThe prefixes you want to remove." << std::endl;
			std::cout << "-r,\t-recursive\tWhether you want to operate recurisve." << std::endl;
			std::cout << "-rd,\t-recursivedepth\tHow deep you want the recustion to be." << std::endl;
			std::cout << "-re,\t-recursiveexclude\tThe folders you don't want to be renamed." << std::endl;
			std::cout << "-s,\t-suffix\t\tThe suffixes(file types) which are required to select the files to remain." << std::endl;

			std::cin.get();
			exit(0);
		}
	}

	char in[128];
	if (!dirSet) {
		// Get the directory from user('.' for current)
		std::cout << "Please enter the directory(Type '.' for the current directory)." << std::endl;
		std::cin.getline(in, sizeof(in));
		dirNames.push_back(in);
	}

	// Open directory
	dir = opendir(dirNames.front().c_str());

	// Check if directory exists
	if (dir == NULL)
		printError("Sorry, the directory couldn't be opened.", 1);

	// Get prefixes
	if (!prefSet)
		;//  prefixes = getInput("Please enter the prefix which you want to remove(Type '.' to stop entering prefixes).", ".", ' ');

		 // Get the suffixes
	if (!suffSet)
		;//  suffix = getInput("Please enter the suffixes(file types) which you want to remove(Type '.' to stop entering suffixes). You don't need to write the '.' (e.g. mp3 is enough)", ".", '.');

	if (recursive)
		for (std::list<std::string>::iterator i = dirNames.begin(); i != dirNames.end(); i++)
			getDirectoryNames(pent, opendir(i->c_str()), *i, &dirNames, excludedFolders, recursiveDepth);

	for (std::list<std::string>::iterator n = dirNames.begin(); n != dirNames.end(); n++)
		renameFilesInDirectory(*opendir(n->c_str()), *n, showOutput, remLeadingNoneChar, prefixes, suffix, prefixLen, defaultName, newName, remTilCertChar, remUntilChar, capitalize);

	std::cout << "Renaming finished!" << std::endl;

	std::cin.get();
	return 0;
}