/*
   Simple data base which stores people's names and their IDs
*/
#pragma once
#ifndef NOT_FOUND
#define NOT_FOUND "Cannot find requested data"
#endif // !NOT_FOUND

#include <vector>
#include <iostream>
#include <ctime>
#include <random>

using namespace std;

struct Data
{
	int id;
	std::string name;
};

static Data people[] = { {237620,"Elmer Page"}, {564057,"Robert Moss"},
	{ 925847,"Michael Hunt" }, { 575561,"Brenda Smith" }, { 109687,"Mary Barker" },
	{ 324588,"Joseph Martinez" }, { 400050,"John Keller" }, { 238928,"Wayne Maldonado" },
	{ 765612,"George Thompson" }, { 673008,"William Watkins" },{ 420169,"Frank May" },
	{ 206565,"Michael Martin" }, { 987796,"Stephanie Murray" }, { 971855,"Kay Thompson" },
	{ 513532,"Leo Chandler" }, { 188787,"Tammy Campbell" }, { 321453,"Angela Gonzales" },
	{ 384224,"Louise Burns" }, { 212522,"Kim Walker" }, { 959482,"Miguel Hernandez" },
	{ 808508,"Laura Bishop" }, { 576600,"Charles Smith" }, { 879666,"Christy Walker" },
	{ 292845,"James Rogers" }, { 503670,"Linda Perez" }, { 324156,"Anne Herrera" },
	{ 701997,"Janice Rodriguez" }, { 258548,"Robin Thomas" }, { 627333,"Betty Gibson" },
	{ 413638,"Brett Hart" }, { 426424,"Aaron Taylor" } };

static std::vector<Data> employees;

void initData();
const char* parseData(int);
const char* parseData(std::string);
int addData(std::string);
