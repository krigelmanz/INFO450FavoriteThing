// Zachary Krigelman
// INFO 450
// Assignment 4 - INFO450FavoriteThing

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

// base class
class FavoriteThing
{
	string HouseNumber;
	string HouseName;
	int	SqFt;
	int Bedrooms;
	double Baths;
public:
	FavoriteThing();
	FavoriteThing(string HouseNumber, string HouseName, int Sqft, int Bedrooms, double Baths);
	void CaptureHouse();
	void ShowHouse();
	int SaveHouse(ofstream &outfile);
};

// class for lists
class HouseList
{
	FavoriteThing **list;
	int numrecords;
	int listsize;
	int ReallocateArray();
public:
	HouseList();
	~HouseList();
	void GetUserInput();
	void ShowHouseList();
	int SaveHouseList(string filename);
	int ReadHouseList(string filename);
};

// default constructor
FavoriteThing::FavoriteThing()
{
	HouseNumber = ""; HouseName = ""; SqFt = 0; Bedrooms = 0; Baths = 0;
}
// overload constructor
FavoriteThing::FavoriteThing(string num, string name, int foot, int bed, double bath)
{
	HouseNumber = num;
	HouseName = name;
	SqFt = foot;
	Bedrooms = bed;
	Baths = bath;
}
// Capture house from user
void FavoriteThing::CaptureHouse()
{
	cout << "House Num? -->";
	getline(cin, HouseNumber);
	cout << "House Nickname?  -->";
	getline(cin, HouseName);
	cout << "Square Footage? -->";
	cin >> SqFt;
	cin.ignore();
	cin.clear();
	cout << "Number of Bedrooms? -->";
	cin >> Bedrooms;
	cin.ignore();
	cin.clear();
	cout << "Number of Baths? -->";
	cin >> Baths;
	cin.ignore();
	cin.clear();
}
// Display house to console
void FavoriteThing::ShowHouse()
{
	cout << "House ID : " << HouseNumber << " Nickname : " << HouseName << " Square Footage : " << SqFt << " Bedrooms : " << Bedrooms << " Baths : " << Baths << endl;
}
// Save house to file given input file stream and user confirmation
int FavoriteThing::SaveHouse(ofstream& outfile)
{
	ShowHouse();
	string answer;
	cout << "Add this house to the list? Y/N" << endl;
	cin >> answer;
	if (outfile.is_open() && (answer == "y" || answer == "Y"))
	{
		outfile << HouseNumber << "|" << HouseName << "|" << SqFt << "|" << Bedrooms << "|" << Baths << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

// House List constructor -allocate default space for array
HouseList::HouseList()
{
	list = new FavoriteThing*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}
// deconstructor - free allocated memory
HouseList::~HouseList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}
// Reallocate memory if already at array capacity
int HouseList::ReallocateArray()
{
	FavoriteThing **temp;
	temp = new FavoriteThing*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}

// get user input for the list
void HouseList::GetUserInput()
{
	string answer;
	cout << "Enter item? Y or N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		list[numrecords] = new FavoriteThing();
		list[numrecords]->CaptureHouse();
		numrecords++;
		cout << "Enter item? Y or N?" << endl;
		getline(cin, answer);
	}
}
// Show list to the console
void HouseList::ShowHouseList()
{
	string answer;
	cout << "Would you like to display the list? Y/N" << endl;
	cin >> answer;

	if (answer == "Y" || answer == "y")
	{
		for (int i = 0; i < numrecords; i++)
			list[i]->ShowHouse();
	}
}
// Save entire list to file, given full-path file name
int HouseList::SaveHouseList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveHouse(output);
		}
		output.close();
		return 0;
	}
	else
		return WRITEERROR;
}
// Read inventory list from a file, given full-path filename
// If file not found or error, return error code
int HouseList::ReadHouseList(string filename)
{
	string inum, iname, isqft, ibed, ibath;
	string answer2;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}
	if (numrecords == listsize)
	{
		ReallocateArray();
	}
	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			ReallocateArray();
		}
		getline(infile, inum, '|');
		if (!inum.empty())
		{
			getline(infile, iname, '|');
			getline(infile, isqft, '|');
			int foot = stoi(isqft);
			getline(infile, ibed, '|');
			int bed = stoi(ibed);
			getline(infile, ibath);
			int bath = stoi(ibath);
			list[numrecords] = new FavoriteThing(inum, iname, foot, bed, bath);
			numrecords++;
		}
	}
	infile.close();
	return 0;
}

int main()
{
	string filename = "d:\\My Documents\\School\\Spring 2017\\INFO 450\\House List.txt";
	HouseList myHouses;
	string answer;
	int error;
	cout << "Default location: " << filename << endl;
	cout << "Change location? Y/N" << endl;
	cin >> answer;
	cin.ignore();
	cin.clear();
	if (answer != "n" && answer != "N")
	{
		cout << "Enter new file location: " << endl;
		getline(cin, filename);
	}
	error = myHouses.ReadHouseList(filename);
	if (error)
	{
		cout << "Cannot read list - continue creating new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}
	myHouses.GetUserInput();
	myHouses.SaveHouseList(filename);
	myHouses.ShowHouseList();

	return 0;
}
