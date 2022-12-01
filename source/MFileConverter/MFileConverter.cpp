#pragma once
#include <iostream>
#include<string>
#include "ToolMesh.h"

using namespace std;
using namespace MeshLib;



int main(int argc, char** argv) {
	CToolMesh<CToolVertex, CToolEdge, CToolFace, CToolHalfEdge> mesh;
	mesh.read_m(argv[1]);
	string filename;
	filename = argv[1];
	// get file ext
	string ext = filename.substr(filename.find_last_of(".") + 1);
	// get file name
	string name = filename.substr(filename.find_last_of("\\") + 1);
	
	// connect string
	string newname = name.substr(0, name.find_last_of(".")) + ".obj";
	
	mesh.write_obj(newname.c_str());
	return 0;
}