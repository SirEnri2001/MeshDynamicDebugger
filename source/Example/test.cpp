#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>

#include "DebuggerConnector.h"

int main(int argc,char* argv[])
{
	DebuggerConnector connector; // Create a connector

	// Highlight 3 points
    connector.HighlightVertex(new double[3] {0, 0, 1});
    connector.HighlightVertex(new double[3] {0, 1, 1});
    connector.HighlightVertex(new double[3] {1, 0, 1});

    // Highlight 1 edge
    connector.HighlightEdge(new double[3] {0, 0.5, 0.5}, new double[3] {0.5, 0.5, 0.5});
	
	// print debug information
    cout << connector.getDumpString() << endl;

    ifstream input;
	
    if (argc < 2) {
        cout <<"Usage: TestExample.exe <Mesh file>"<< endl
            << "Specify input mesh file name at any format in the argument" << endl;
        return 1;
    }
	
    input.open(argv[1]);
	if (!input.is_open())
	{
		cout << "Could not open file" << endl;
		return 1;
	}
	
    std::string str((std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>());
	
    Mesh mesh;
    OpenMesh::IO::read_mesh(mesh, argv[1]);
    int i = 0;
    while (1) {
        // set or update mesh
        // !! you can use OpenMesh::PolyMesh_ArrayKernelT<> in Openmesh library
        connector.SetModel(mesh);
		// !! or you can use a string of file content like
        // connector.SetModel(str, ".ply"); // NOTE: Specify the file extension

		// highlight a vertex
        connector.HighlightVertex(new double[3] {0, 0, 1});
		
        // highlight an edge
        connector.HighlightEdge(new double[3] {4.0+i, 0.5, 0.5}, new double[3] {0.5+i, 0.5, 0.5});

        // Update Debugger renderer window
        connector.Update();
        Sleep(2000);
        i++;
    }
    
    return 0;
}
