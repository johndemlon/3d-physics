/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
  ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

	Copyright (c) 2013 Charilaos Kalogirou.
	All rights reserved.

	Redistribution and use in source and binary forms are permitted
	provided that the above copyright notice and this paragraph are
	duplicated in all such forms and that any documentation,
	advertising materials, and other materials related to such
	distribution and use acknowledge that the software was developed
	by Charilaos Kalogirou. The name of the
	Charilaos Kalogirou may not be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/
#include <cassert>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <dirent.h>
#endif

#include <errno.h>
#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif



#include "shader.h"
#include "tokenizer.h"

using namespace std;
CShaderFlags::CShaderFlags()
{
	//COMMON
	//brush
	structural  = 1;
	opaque      = 1;

	//brushplane
	prior       = 0;
	keep        = 1;
	portal      = 0;
	leak        = 0;

	tessSize = 0.0;
	smoothNormals = false;

	shaderId = -1;
}

CShaderFlags::CShaderFlags(bool opaq, bool structur, bool toKeep, bool pr, int por)
{
	setOpaque(opaq);
	setStructural(structur);
	setKeep(toKeep);
	prior = pr;  
	portal = por;
	leak   = 0;  
	smoothNormals = false;

	shaderId = -1;
	/*  if(loadFromFile(filePath))
	continue;
	cout << "WARNING!!!:'"<<filePath "' shader not found " << endl;
	if(loadFromFile("common/common"))
	continue;
	cout << "Not 'common/common' shader found to replace the missing  one '" << filePath<<"'"  <<endl;
	cout << "Exiting..."<<endl;
	exit(1);*/
}

void CShaderFlags::setOpaque(bool s)
{
	opaque = s;
}

void CShaderFlags::setStructural(bool t)
{
	structural = t;
}

void CShaderFlags::setKeep(bool d)
{
	keep = d;
}


void CShaderFlags::setPrior(bool pr)
{
	prior = pr;
}

void CShaderFlags::setPortal(const int& pNo)
{
	portal = pNo;
}

void CShaderFlags::setSmoothNormals(bool s){
	smoothNormals = s;
}

bool CShaderFlags::isSmoothNormals(){
	return smoothNormals;
}

bool CShaderFlags::isStructural()
{
	return structural;
}
bool CShaderFlags::isOpaque()
{
	return opaque;
}

/*bool CShaderFlags::isSplit()
{
return split;
}*/

bool CShaderFlags::isPrior()
{
	return prior;
}

bool CShaderFlags::isKeep()
{
	return keep;
}

bool CShaderFlags::isLeak(){ return leak; }
void CShaderFlags::setLeak(bool a){ leak = a; }
int CShaderFlags::isPortal()
{
	return portal;
}

void CShaderFlags::setTessSize(float t){
	tessSize = t;
}

float CShaderFlags::getTessSize(){
	return tessSize;
}

void CShaderFlags::printFlags()
{
	cout << "--------------"<<endl;
	cout << "portal    :"<<portal<<endl;
	cout << "keep      :"<<keep<<endl;
	cout << "opaque    :"<<opaque<<endl;
	cout << "structural:"<<structural<<endl;
	cout << "prior     :"<< prior<<endl;
}
CShaderLoader::CShaderLoader(bool load)
{

	//shaders["common/portal"] = CShaderFlags(1, 0, 0, 1);
	//solid, notr, nodraw, prior
	whichWasRead = 0;
	shaders = 0;                                    
	setUpShadersInfo();
	if(load)
		loadShaders(true);
}


void CShaderLoader::setUpShadersInfo()
{
	extcharno = 8;
	extension = new char[extcharno];
	extension = ".shader";


	directory = "../base/scripts/";
}

//CShaderLoader::extension = ;
//CShaderLoader::extcharno = 8;
int CShaderLoader::selector(char *name){
	size_t nameLength = strlen(name);

	if(nameLength < extcharno)
		return 0;

	if(!strcmp(strrchr(name, '.'), extension))
		return 1;

	return 0;
}

void CShaderLoader::loadShaders(bool readAll){
	if(!whichWasRead)
		whichWasRead = new map<string, string>();
	else {    
		(*whichWasRead).clear();
	}

	assert(shaders);

#ifdef _WIN32
	struct	_finddata_t fileinfo;

	string pattern = directory + "*.shader";

	int hd = _findfirst(pattern.c_str(), &fileinfo);
	if(hd != -1){
		// It is a directory
		string data("");
		do {
			if(!selector(fileinfo.name))
				continue;

			readShaderFile(fileinfo.name, readAll);
			//data += fullName + "/" + fileinfo.name + "\n";
		} while (_findnext(hd, &fileinfo) != -1);
		_findclose(hd);
		return;
	}
#else
	DIR *dir;
	struct dirent *entry;

	dir = opendir(directory.c_str());
	if (!dir){
		cout << strerror(errno) <<" error shaders reading directory : '"<<directory<<"' "<<  endl;
		exit(0);
	}

	while((entry = readdir(dir))){
		if(!selector(entry->d_name))
			continue;

		readShaderFile(entry->d_name, readAll);
	}

#endif

	cout << " reading shaders finished" << endl;
}


//if readAll == true it will read all the shaders will find
//else it will read all the shaders already exist in the shaders map

void CShaderLoader::readShaderFile(const char *fileName, bool readAll){
	string filePath(directory);
	filePath +=fileName;
	FILE *f = fopen(filePath.c_str(), "r");
	if(!f){
		cout <<"Error reading shader: "<<strerror(errno)<<endl;
		return;
	}

	//put all the chars of the file into a string
	string data="";
	int c;
	while((c = fgetc(f))!=EOF){
		if(c == '}' || c == '{')
		{
			data += " ";
			data +=(char)c;
			data += " ";
		}
		else
			data += (char)c;
	}
	fclose(f);


	//give it to tokenizer
	CTokenizerNoComments tokenizer(data);
	tokenizer.setLowerCaseTokens(true);



	string token;
	//start parsing
	int level = 0; //the shaders name level
	string currentShaderName="vassilis";
	bool waitForParam=false;
	CShaderFlags *flags;
	bool skipShader=false;
	while(tokenizer.nextToken()){
		token = tokenizer.getToken();


		if(token == "{"){
			++level;
			continue;
		}
		else if (token == "}"){
			--level;
			continue;
		}

		if(level == 0){
			if(!readAll)
				if((*shaders).find(token)==(*shaders).end()){
					skipShader=true;            
					continue;
				}


				if((*whichWasRead).find(token)!=(*whichWasRead).end()){
					//cout << "WARNING: Redefinition of shader '"<<token<<"' in '"<< fileName<<"'. First defined in '"<<(*whichWasRead)[token]<<"'"<<endl;          
					//if(readAll){
					skipShader=true;
					continue;
					//}
				}

				skipShader=false;
				currentShaderName = token;
				(*whichWasRead)[currentShaderName] = fileName;

				//cout <<" "<<currentShaderName<<",";
				if(readAll){          
					(*shaders)[currentShaderName] = new CShaderFlags();

				}
				else{
					//CShaderFlags *newSFlags = (*shaders)[currentShaderName];
					//newSFlags->setShaderId(shadersNames.size());
					//shadersNames.push_back(currentShaderName);        
					;
				}

				continue;
		}


		/*if(level != 1){
		waitForParam=false;
		continue;
		}*/

		if(skipShader)
			continue;


		if((token != "surfaceparm")&&!waitForParam)
			continue;

		//if(!tokenizer.nextToken()){      
		//    cout << "unexpected end of shader '"<<currentShaderName<<"' at file "<<filePath<<endl;
		//    continue;
		// }

		if(!waitForParam){ //means tokken is surfaceparm
			waitForParam=true;
			continue;
		}

		waitForParam=false;

		//if(parameters.find(token) == parameters.end())
		//  continue;

		flags = (*shaders)[currentShaderName];            
		if(token == "nodraw")
			flags->setKeep(false);
		else if(token == "trans")
			flags->setStructural(false);
		else if(token == "nosolid")
			flags->setOpaque(false);      
		else if(token == "leak")
			flags->setLeak(true);
		else if(token == "hint")
			flags->setPrior(true);
		else if(token == "smooth")
			flags->setSmoothNormals(true);
		else if(token == "tess"){
			tokenizer.nextToken();
			token = tokenizer.getToken();
			float tess;
			sscanf(token.c_str(), "%f", &tess);
			if (tess <= 0.0) {
				cout << "ERROR : wrong tess value" << endl;
			} else {
				flags->setTessSize(tess);
			}
		}
	}
}

void CShaderLoader::setShadersToRead(map<string, CShaderFlags *> *sh)
{
	shaders = sh;
}

void CShaderLoader::freeShaders(){
	if(!shaders)
		return;

	map<string, CShaderFlags*>::iterator cIter, cEnd;
	cIter = shaders->begin();
	cEnd = shaders->end();
	CShaderFlags *flags;
	while(cIter!=cEnd){
		flags = cIter->second;
		if(flags)
			delete flags;
		++cIter;
	}
	delete shaders;
	shaders = 0;
}

