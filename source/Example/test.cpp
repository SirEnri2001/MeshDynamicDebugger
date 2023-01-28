/*#include <iostream>
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
    std::string str2;
    if (argc == 3) {
        ifstream input2;
        input2.open(argv[2]);
        if (input2.is_open())
        {
			str2 = string((istreambuf_iterator<char>(input2)), istreambuf_iterator<char>());
		}
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
        if (i % 2 == 0) {
            connector.SetModel(mesh);
        }
        else {
            connector.SetModel(str2, ".obj");
        }
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
*/

/* ========================================================================= *
 *                                                                           *
 *                               OpenMesh                                    *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openmesh.org                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenMesh.                                            *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
 *                                                                           *
 * ========================================================================= */

#include <iostream>
 // -------------------- OpenMesh
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Mesh/Status.hh>

// ----------------------------------------------------------------------------

struct MyTraits : public OpenMesh::DefaultTraits
{
};


typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits>  MyMesh;


// ----------------------------------------------------------------------------
// Build a simple cube and delete it except one face

int main()
{
    MyMesh mesh;

    // the request has to be called before a vertex/face/edge can be deleted. it grants access to the status attribute
    mesh.request_face_status();
    mesh.request_edge_status();
    mesh.request_vertex_status();

    // generate vertices

    MyMesh::VertexHandle vhandle[8];
    MyMesh::FaceHandle   fhandle[6];

    vhandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1, 1));
    vhandle[1] = mesh.add_vertex(MyMesh::Point(1, -1, 1));
    vhandle[2] = mesh.add_vertex(MyMesh::Point(1, 1, 1));
    vhandle[3] = mesh.add_vertex(MyMesh::Point(-1, 1, 1));
    vhandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
    vhandle[5] = mesh.add_vertex(MyMesh::Point(1, -1, -1));
    vhandle[6] = mesh.add_vertex(MyMesh::Point(1, 1, -1));
    vhandle[7] = mesh.add_vertex(MyMesh::Point(-1, 1, -1));


    // generate (quadrilateral) faces

    std::vector<MyMesh::VertexHandle>  tmp_face_vhandles;

    tmp_face_vhandles.clear();
    tmp_face_vhandles.push_back(vhandle[0]);
    tmp_face_vhandles.push_back(vhandle[1]);
    tmp_face_vhandles.push_back(vhandle[2]);
    tmp_face_vhandles.push_back(vhandle[3]);
    fhandle[0] = mesh.add_face(tmp_face_vhandles);

    tmp_face_vhandles.clear();
    tmp_face_vhandles.push_back(vhandle[7]);
    tmp_face_vhandles.push_back(vhandle[6]);
    tmp_face_vhandles.push_back(vhandle[5]);
    tmp_face_vhandles.push_back(vhandle[4]);
    fhandle[1] = mesh.add_face(tmp_face_vhandles);

    tmp_face_vhandles.clear();
    tmp_face_vhandles.push_back(vhandle[1]);
    tmp_face_vhandles.push_back(vhandle[0]);
    tmp_face_vhandles.push_back(vhandle[4]);
    tmp_face_vhandles.push_back(vhandle[5]);
    fhandle[2] = mesh.add_face(tmp_face_vhandles);


    tmp_face_vhandles.clear();
    tmp_face_vhandles.push_back(vhandle[2]);
    tmp_face_vhandles.push_back(vhandle[1]);
    tmp_face_vhandles.push_back(vhandle[5]);
    tmp_face_vhandles.push_back(vhandle[6]);
    fhandle[3] = mesh.add_face(tmp_face_vhandles);


    tmp_face_vhandles.clear();
    tmp_face_vhandles.push_back(vhandle[3]);
    tmp_face_vhandles.push_back(vhandle[2]);
    tmp_face_vhandles.push_back(vhandle[6]);
    tmp_face_vhandles.push_back(vhandle[7]);
    fhandle[4] = mesh.add_face(tmp_face_vhandles);


    tmp_face_vhandles.clear();
    tmp_face_vhandles.push_back(vhandle[0]);
    tmp_face_vhandles.push_back(vhandle[3]);
    tmp_face_vhandles.push_back(vhandle[7]);
    tmp_face_vhandles.push_back(vhandle[4]);
    fhandle[5] = mesh.add_face(tmp_face_vhandles);

    // And now delete all faces and vertices
    // except face (vh[7], vh[6], vh[5], vh[4])
    // whose handle resides in fhandle[1]


    // If isolated vertices result in a face deletion
    // they have to be deleted manually. If you want this
    // to happen automatically, change the second parameter
    // to true.

    // Now delete the isolated vertices 0, 1, 2 and 3
    
    MyMesh::HalfedgeHandle he = mesh.halfedge_handle(fhandle[0]);
    MyMesh::EdgeHandle e = mesh.edge_handle(he);
    mesh.delete_edge(e);
    // Delete all elements that are marked as deleted
    // from memory.
    mesh.garbage_collection();

    // write mesh to output.obj
    try {
        if (!OpenMesh::IO::write_mesh(mesh, "output.off")) {
            std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
            return 1;
        }
    }
    catch (std::exception& x)
    {
        std::cerr << x.what() << std::endl;
        return 1;
    }

    return 0;
}