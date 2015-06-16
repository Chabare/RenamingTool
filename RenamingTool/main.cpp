#include "dirent.h"
#include <iostream>
#include <list>
#include <string>

bool contains (std::string input, std::list<std::string> list) {
	for (std::list<std::string>::iterator it = list.begin (); it != list.end(); it++)
		if (*it == input)
			return true;
	
	return false;
}

bool starts_with (std::string string, std::string prefix) {
	return strcmp(string.substr (0, prefix.length ()).c_str(), prefix.c_str()) == 0;
}

int main() {
	bool showFiles = true;
	DIR *dir = NULL;
	struct dirent *pent = NULL;
	std::string directory = ".";
	std::list<std::string> prefixes;
	std::list<std::string> files;
	std::list<std::string> filesWPrefix;
	std::list<int> prefixLen;

	// Get the directory from user('.' for current)
	std::cout << "Please enter the directory(Type '.' for the current directory)." << std::endl;
	char in[128];
	std::cin.getline (in, sizeof (in));
	directory = in;

	// Open directory
	dir = opendir (directory.c_str());

	// CHeck if directory exists
	if (dir == NULL) {
		std::cout << "Sorry, the directory couldn't be opened." << std::endl;
		std::cin.get ();
		exit (1);
	}

	if (showFiles)
		std::cout << "Following files are in the given directory: " << std::endl;
	// Put the files into the list(not "." and "..")
	while (pent = readdir (dir)) {
		if (pent == NULL) {
			std::cout << "Sorry, the directory couldn't be read." << std::endl;
			std::cin.get ();
			exit (3);
		} else
			if (!(strcmp (pent->d_name, ".") == 0 || strcmp (pent->d_name, "..") == 0)) {
				if (showFiles)
					std::cout << pent->d_name << std::endl;
				files.push_back (pent->d_name);
			}
	}

	std::cout << std::endl;

	std::string input;
	// Get the prefixes
	while (input != ".") {
		std::cout << "Please enter the prefix which you want to remove(Type '.' to stop entering prefixes)." << std::endl;
		std::cin.getline (in, sizeof(in));
		input = in;
		if (input != "." && !contains(input, prefixes))
			prefixes.push_back (input);
	}
	input.clear ();

	// Put the files with one of the given prefixes into the list
	// Put the length of the prefix into the list
	for (std::list<std::string>::iterator i = files.begin (); i != files.end (); i++)
		for (std::list<std::string>::iterator j = prefixes.begin (); j != prefixes.end (); j++)
			if (starts_with (*i, *j)) {
				filesWPrefix.push_back (*i);
				prefixLen.push_back (j->length());
			}

	// Output the files with one of the given prefixes
	std::cout << "Files found with given prefixes: " << std::endl;
	for (std::list<std::string>::iterator i = filesWPrefix.begin (); i != filesWPrefix.end (); i++)
		std::cout << *i << std::endl;
	
	std::list<int>::iterator l = prefixLen.begin ();
	for (std::list<std::string>::iterator i = filesWPrefix.begin (); i != filesWPrefix.end (); i++, l++)
		std::cout << "OLD: " << *i << "\tNEW: " << i->substr(*l, i->length() - *l) << std::endl;

	// Close the directory
	closedir (dir);

	std::cin.ignore().get();
	return 0;
}