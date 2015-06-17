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

std::list<std::string> split(const std::string &s, char delim) {
	std::list<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool contains(std::string input, std::list<std::string> list) {
	for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		if (*it == input)
		return true;

	return false;
}

bool starts_with(std::string string, std::string prefix) {
	return strcmp(string.substr(0, prefix.length()).c_str(), prefix.c_str()) == 0;
}


bool ends_with(std::string string, std::string suffix) {
	return strcmp(string.substr(string.length() - suffix.length(), string.length()).c_str(), suffix.c_str()) == 0;
}

void printList(std::list<std::string> list) {
	for (std::list<std::string>::iterator i = list.begin(); i != list.end(); i++)
		std::cout << *i << std::endl;
}

void printList(std::list<int> list) {
	for (std::list<int>::iterator i = list.begin(); i != list.end(); i++)
		std::cout << *i << std::endl;
}

void printError(std::string string, bool exit) {
	std::cerr << string << std::endl;
	if (exit)
		printError(string, 1);
}

void printError(std::string string, bool exit, int exitCode) {
	std::cerr << string << std::endl;
	if (exit)
		printError(string, exitCode);
}

void printError(std::string string, int exitCode) {
	std::cin.get();
	exit(exitCode);
}

int main(int argc, char* argv[]) {
	char delimiter = ';';
	std::regex regexFilename = std::regex("[^<>:\"/\\|?*]*");
	std::string defaultName = "RenamedByRenamingTool";
	bool dirSet = false;
	bool showOutput = false;
	bool prefSet = false;
	bool suffSet = false;
	DIR *dir = NULL;
	struct dirent *pent = NULL;
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
		// Command line argument: Output
		else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0) {
			if (i == argc)
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

	if (showOutput)
		std::cout << "Following files are in the given directory: " << std::endl;
	// Put the files into the list(not "." and "..")
	while (pent = readdir(dir)) {
		if (pent == NULL)
			printError("Sorry, the directory couldn't be read.", true, 3);
		else
			// Check for '.' and '..' links and don't add them to the file list
			if (!(strcmp(pent->d_name, ".") == 0 || strcmp(pent->d_name, "..") == 0)) {
				if (showOutput)
					std::cout << pent->d_name << std::endl;
				files.push_back(pent->d_name);
			}
	}


	std::string input;
	// Get the prefixes
	if (!prefSet) {
		while (input != ".") {
			std::cout << std::endl << "Please enter the prefix which you want to remove(Type '.' to stop entering prefixes)." << std::endl;
			std::cin.getline(in, sizeof(in));
			input = in;
			if (input != "." && !contains(input, prefixes))
				prefixes.push_back(input);
		}
		input.clear();
	}

	// Get the suffixes
	if (!suffSet) {
		while (input != ".") {
			std::cout << "Please enter the suffixes(file types) which you want to remove(Type '.' to stop entering suffixes). You don't need to write the '.' (e.g. mp3 is enough)" << std::endl;
			std::cin.getline(in, sizeof(in));
			input = in;
			if (input != "." && !contains(input, suffix)) {
				if (input.at(0) == '.')
					input = input.substr(1, input.length() - 1);
				suffix.push_back(input);
			}
		}
		input.clear();
	}

	// Put the files with one of the given prefixes into the list
	// Put the length of the prefix into the list
	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++)
		for (std::list<std::string>::iterator j = prefixes.begin(); j != prefixes.end(); j++)
		if (starts_with(*i, *j)) {
			if (suffix.size() > 0) {
				// Iterate through suffixes
				for (std::list<std::string>::iterator s = suffix.begin(); s != suffix.end(); s++)
					if (ends_with(*i, *s)) {
						filesWPrefix.push_back(*i);
						prefixLen.push_back(j->length());
					}
			} else {
				filesWPrefix.push_back(*i);
				prefixLen.push_back(j->length());
			}

		}

	// Output the files with one of the given prefixes
	if (showOutput) {
		std::cout << "Files found with given prefixes: " << std::endl;
		if (filesWPrefix.size() == 0)
			std::cout << "None" << std::endl;
		printList(filesWPrefix);

	}

	// Rename files
	std::list<int>::iterator l = prefixLen.begin();
	// Iterate through files
	for (std::list<std::string>::iterator i = filesWPrefix.begin(); i != filesWPrefix.end(); i++, l++) {
		if (i->length() - *l == 0)
			*i = defaultName;
		// Rename file
		if (rename((directory + *i).c_str(), (directory + (i->substr(*l, i->length() - *l))).c_str()) == -1)
			std::cout << "Renaming following file failed: " << *i << std::endl << "\nReason: " << errno << std::endl << std::endl;
	}

	if (showOutput)
		if (filesWPrefix.size() > 0)
		std::cout << "Renaming finished" << std::endl;
	// Close the directory
	closedir(dir);

	std::cin.get();
	return 0;
}