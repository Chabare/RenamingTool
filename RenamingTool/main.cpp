#include "dirent.h"
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>

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

/**
	Checks whether given char is alphanumeric or not

	@param c Character to check
	@return Char is alphanumeric
*/
bool isAlphanumeric(char c) {
	return ((int)c >= (int)'a' && (int)c <= (int)'z') || ((int)c >= (int)'A' && (int)c <= (int)'Z');
}

/**
	Removes the leading none (alphanumeric) characters from a file

	@param fileName Filename to remove leading none (alphanumeric) characters from
	@return File starting with alphanumeric character
*/
std::string removeLeadingNoneCharacters(std::string fileName) {
	std::cout << fileName << std::endl;
	while (!isAlphanumeric(fileName.at(0))) {
		std::cout << "BEFORE: " << fileName << std::endl;
		fileName = fileName.substr(1, fileName.length() - 1);
		std::cout << "After:  " << fileName << std::endl;
	}
	std::cout << std::endl;
	
	return fileName;
}

/**
	Removes the leading none (alphanumeric) characters from a list of files

	@param list List of filenames to remove leading none (alphanumeric) characters from
	@return List of filenames starting with alphanumeric character
*/
std::list<std::string> removeLeadingNoneCharacters(std::list<std::string> list) {
	for (std::list<std::string>::iterator i = list.begin(); i != list.end(); i++)
		*i = removeLeadingNoneCharacters(*i);

	return list;
}

/**
	Finds the correct name for the file

	@param newName Default new name
	@param files List of all files in directory
	@param renamedFiles A list of the renamed files
	@param directory The directory of the files
	@param remLeadingNoneChar Whether leading none alphanumeric characters should be removed
	@return New name for the file
*/
std::string determineNewFileName(std::string newName, std::list<std::string> files, std::list<std::string> renamedFiles, std::string directory, bool remLeadingNoneChar) {
	if (remLeadingNoneChar)
		newName = removeLeadingNoneCharacters(newName);

	std::string fileType = getFileType(newName);
	if(fileType != "")
		newName = removeFileType(newName);
	while (contains(newName + "." + fileType, files) || contains(directory + newName + "." + fileType, renamedFiles))
		newName.append(" - Copy");

	newName.append("." + fileType);

	return directory + newName;
}

/**
	Renames a list of files

	@param files List of files to remain
	@param prefixLen Length of prefix for each file
	@param directory The directory in which the file is
	@param remLeadingNoneChar Whether leading none alphanumeric characters should be removed
	@param The default name for a file
*/
void renameFiles(std::list<std::string> files, std::list<std::string> newFiles, std::list<int> prefixLen,  std::string directory, std::string defaultName, bool remLeadingNoneChar) {
	std::list<std::string> renamedFiles;
	std::list<int>::iterator l = prefixLen.begin();
	// Iterate through files
	for (std::list<std::string>::iterator i = newFiles.begin(); i != newFiles.end() && l != prefixLen.end(); i++, l++) {
		if (i->length() - *l == 0)
			*i = defaultName;
		
		// Rename file
		std::string newName = determineNewFileName(i->substr(*l, i->length() - *l), files, renamedFiles, directory, remLeadingNoneChar);
		if (rename((directory + *i).c_str(), newName.c_str()) != 0)
			printError("Renaming following file failed: " + *i + "\nReason: " + strerror(errno) + "\nNew name: " + newName + "\n", false);
		else
			renamedFiles.push_back(newName);
	}
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
	while(pent = readdir(dir))
		if (pent == NULL)
			printError("Sorry, the directory couldn't be read.", 3);
		else
			// Check for '.' and '..' links and don't add them to the file list
			if (!(strcmp(pent->d_name, ".") == 0 || strcmp(pent->d_name, "..") == 0))
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

	while (input != ".") {
		std::cout << inputReq << std::endl;
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
	@param remLeadingNoneChar Whether leading none alphanumeric characters should be removed
	@return Returns a list of files which have one of the prefixes and match the suffixes
*/
std::list<std::string> getFilesWithPrefixAndSuffix(std::list<std::string> files, std::list<std::string> prefixes, std::list<std::string> suffix, std::list<int> *prefixLen, bool remLeadingNonChar) {
	std::list<std::string> filesWPrefix;

	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++)
		for (std::list<std::string>::iterator j = prefixes.begin(); j != prefixes.end(); j++)
		if (starts_with(*i, *j)) {
			if (suffix.size() > 0) {
				// Iterate through suffixes
				for (std::list<std::string>::iterator s = suffix.begin(); s != suffix.end(); s++)
					if (ends_with(*i, *s)) {
						filesWPrefix.push_back(*i);
						prefixLen->push_back(j->length());
					}
			} else {
				filesWPrefix.push_back(*i);
				prefixLen->push_back(j->length());
			}
		} else if (remLeadingNonChar && !isAlphanumeric(i->at(0)))
			if (suffix.size() > 0) {
				// Iterate through suffixes
				for (std::list<std::string>::iterator s = suffix.begin(); s != suffix.end(); s++)
					if (ends_with(*i, *s)) {
						filesWPrefix.push_back(*i);
						prefixLen->push_back(0);
					}
			} else {
				filesWPrefix.push_back(*i);
				prefixLen->push_back(0);
			}


	return filesWPrefix;
}

int main(int argc, char* argv[]) {
	bool dirSet = false;
	bool showOutput = false;
	bool prefSet = false;
	bool suffSet = false;
	bool remLeadingNonChar = false;
	char delimiter = ';';
	DIR *dir = NULL;
	struct dirent *pent = NULL;
	std::regex regexFilename = std::regex("[^<>:\"/\\|?*]*");
	std::string defaultName = "RenamedByRenamingTool";
	std::string directory = ".";
	std::list<std::string> prefixes;
	std::list<std::string> files;
	std::list<std::string> filesWPrefix;
	std::list<int> prefixLen;
	std::list<std::string> suffix;
	
	// Get command line arguments
	for (int i = 1; i < argc; i += 2) {
		// Command line argument: Directory
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-directory") == 0) {
			directory = argv[i + 1];
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
				remLeadingNonChar = true;
			else if (strcmp(argv[i + 1], "true") == 0)
				remLeadingNonChar = true;
			else if (strcmp(argv[i + 1], "false") == 0)
				remLeadingNonChar = false;
			else {
				remLeadingNonChar = true;
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
			prefixes = split(argv[i + 1], ';');
			if (prefixes.size() > 0)
				prefSet = true;
		}
		// Command line argument: Suffix
		else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-suffix") == 0) {
			if (strcmp(argv[i + 1], "any") != 0)
				suffix = split(argv[i + 1], ';');

			suffSet = true;
		}
		// Command line argument: Help
		else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "?") == 0) {
			std::cout << "-d,\t-directory\tThe directory you want to be in." << std::endl;
			std::cout << "-def,\t-default\tThe filename which is defaulted to if there are problems with the one without prefix." << std::endl;
			std::cout << "-delim,\t-delimiter\tSets the delimiter for the prefixes and suffixes input" << std::endl;
			std::cout << "-o,\t-output\t\tWhether the program will generate output or not(Doesn't need an argument)." << std::endl;
			std::cout << "-p,\t-prefix\t\tThe prefixes you want to remove." << std::endl;
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
		directory = in;
	}

	// Open directory
	dir = opendir(directory.c_str());

	// Check if directory exists
	if (dir == NULL)
		printError("Sorry, the directory couldn't be opened.", true);

	// Check for '\' as end and append if not there
	if (directory.at(directory.length() - 1) != '/' || directory.at(directory.length() - 1) != '\\')
		directory.append("\\");

	files = getFilesFromDir(pent, dir);
	if (showOutput) {
		std::cout << "Following files are in the given directory: " << std::endl;
		printList(files);
	}

	// Get prefixes
	if(!prefSet)
		prefixes = getInput("Please enter the prefix which you want to remove(Type '.' to stop entering prefixes).", ".", ' ');


	// Get the suffixes
	if (!suffSet)
		suffix = getInput("Please enter the suffixes(file types) which you want to remove(Type '.' to stop entering suffixes). You don't need to write the '.' (e.g. mp3 is enough)", ".", '.');

	filesWPrefix = getFilesWithPrefixAndSuffix(files, prefixes, suffix, &prefixLen, remLeadingNonChar);

	// Output the files with one of the given prefixes
	if (showOutput) {
		std::cout << "Files found with given prefixes: " << std::endl;
		if (filesWPrefix.size() == 0)
			std::cout << "None" << std::endl;
		printList(filesWPrefix);
	}

	renameFiles(files, filesWPrefix, prefixLen, directory, defaultName, remLeadingNonChar);

	if (filesWPrefix.size() > 0)
		std::cout << "Renaming finished!" << std::endl;

	// Close the directory
	closedir(dir);

	std::cin.get();
	return 0;
}