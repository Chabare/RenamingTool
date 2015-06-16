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
	return strcmp(string.substr (0, prefix.length ()).c_str(), prefix.c_str());
}

int main() {
	bool showFiles = true;
	DIR *dir = NULL;
	struct dirent *pent = NULL;
	std::string directory = ".";
	std::list<std::string> prefixes;
	std::list<std::string> files;
	std::list<std::string> filesWPrefix;

	std::cout << "Please enter the directory(Type '.' for the current directory)." << std::endl;
	char in[128];
	std::cin.getline (in, sizeof(in));
	directory = in;
	delete in;

	dir = opendir (directory.c_str());

	std::cout << starts_with ("SV_ASD", "SV_");

	if (dir == NULL) {
		std::cout << "Sorry, the directory couldn't be opened." << std::endl;
		std::cin.get ();
		exit (1);
	}

	if (showFiles) {
		std::cout << "Following files are in the given directory: " << std::endl;
		while (pent = readdir (dir)) {
			if (pent == NULL) {
				std::cout << "Sorry, the directory couldn't be read." << std::endl;
				std::cin.get ();
				exit (3);
			} else {
				if (!(strcmp (pent->d_name, ".") == 0 || strcmp (pent->d_name, "..") == 0)) {
					std::cout << pent->d_name << std::endl;
					files.push_back (pent->d_name);
				}
			}
		}
	}

	std::string input;
	while (input != ".") {
		std::cout << "Please enter the prefix which you want to remove(Type '.' to stop entering prefixes)." << std::endl;
		std::cin >> input;
		if (input != "." && !contains(input, prefixes))
			prefixes.push_back (input);
	}
	input.clear ();

	for (std::list<std::string>::iterator i = files.begin (); i != files.end (); i++)
		for (std::list<std::string>::iterator j = prefixes.begin (); j != prefixes.end (); j++)
			if (starts_with (*i, *j))
				filesWPrefix.push_back (*i);

	for (std::list<std::string>::iterator i = filesWPrefix.begin (); i != filesWPrefix.end (); i++)
		std::cout << *i << std::endl;

	closedir (dir);

	std::cin.ignore().get();
	return 0;
}