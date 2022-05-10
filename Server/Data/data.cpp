#include "data.h"

// Writting data to vector
void initData()
{
	for (size_t i = 0; i < sizeof(people) / sizeof(people[0]); i++)
	{
		employees.push_back(people[i]);
	}
}

// Search person by ID
const char* parseData(int id)
{
	for (size_t i = 0; i < employees.size(); i++)
	{
		if (employees[i].id == id)
		{
			return employees[i].name.c_str();
		}
	}
	return NOT_FOUND;
}

// Search person by name
const char* parseData(string name)
{
	for (size_t i = 0; i < employees.size(); i++)
	{
		if (employees[i].name == name)
		{
			string tmp = to_string(employees[i].id);
			return tmp.c_str();
		}
	}
	return NOT_FOUND;
}

// Add a new person to data base
int addData(string name)
{
	int new_id;
	bool is_unique;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1e5, 1e6-1);

	// Create unique ID for a new person
	do
	{
		is_unique = true;
		new_id = dist(gen);
		for (size_t i = 0; i < employees.size(); i++)
		{
			if (employees[i].id == new_id)
			{
				is_unique = false;
				break;
			}
		}
	} while (!is_unique);
	
	Data new_emp;
	new_emp.id = new_id;
	new_emp.name = name;

	employees.push_back(new_emp);

	return new_id;
}
