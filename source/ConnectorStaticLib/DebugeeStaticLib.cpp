// DebugeeStaticLib.cpp : Defines the functions for the static library.
//
#include "pch.h"
#include"DebuggerConnector.h"


DebuggerConnector::DebuggerConnector()
{
    meshUpdated = false;
    init();
}

void DebuggerConnector::init()
{
    read_sem = OpenSemaphore(
        SEMAPHORE_ALL_ACCESS,          // 访问标志
        TRUE,                          // 继承标志
        L"READ_MESH_DATA");                   // 信号量名
    write_sem = OpenSemaphore(
        SEMAPHORE_ALL_ACCESS,          // 访问标志
        TRUE,                          // 继承标志
        L"WRITE_MESH_DATA");                   // 信号量名

    // 打开共享的文件对象
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, L"ShareMemory");
    if (hMapFile)
    {
        lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        cout << "Debuggee connected" << endl;
    }
    else
    {
        // 打开共享内存句柄失败
        printf("OpenMapping Error");
    }

}

void DebuggerConnector::HighlightVertex(double* v)
{
    json vert = { v[0],v[1],v[2] };
    vertices.push_back(vert);
}

void DebuggerConnector::HighlightEdge(double* v1, double* v2) {
    json vert1 = { v1[0],v1[1],v1[2] };
    json vert2 = { v2[0],v2[1],v2[2] };

    json edge = { vert1,vert2 };
    edges.push_back(edge);
}

void DebuggerConnector::SetModel(std::string fileDumped, std::string inputExtension) {
    Mesh mesh;
    OpenMesh::IO::Options ropt;
    istringstream iss;
    iss.str(fileDumped);
    if (OpenMesh::IO::read_mesh(mesh, iss, inputExtension, ropt)) {
        SetModel(mesh);
    }
    else {
        cerr << "DEBUGGER : CANNOT READ MESH" << endl;
    }
}

void DebuggerConnector::SetModel(Mesh mesh) {
    ostringstream oss;
    OpenMesh::IO::Options wopt;
    OpenMesh::IO::write_mesh(mesh, oss, ".ply");
    this->mesh = oss.str();
    meshUpdated = true;
}

void DebuggerConnector::Update() {
    try
    {
        inTryBlock(write_sem, read_sem, lpBase, vertices, edges, mesh, meshUpdated);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        ReleaseSemaphore(read_sem, 1, NULL);
        cout << "Some errors happened, but connector will continue to work" << endl;
    }
    vertices.clear();
    edges.clear();
    meshUpdated = false;
}

std::string DebuggerConnector::getDumpString() {
    json j = {
        {"vertices",vertices},
        {"edges",edges},
        {"mesh_ply",mesh}
    };
    return j.dump();
}


void inTryBlock(HANDLE write_sem, HANDLE read_sem, LPVOID lpBase, json vertices, json edges, json mesh, bool meshUpdated) {
    WaitForSingleObject(write_sem, INFINITE);
    // 将共享内存数据拷贝出来
    json j = { {"a","b"} };
    if (vertices.size() > 0) {
        j.push_back({ "vertices",vertices });
    }
    if (edges.size() > 0) {
        j.push_back({ "edges",edges });
    }
    if (meshUpdated) {
        j.push_back({ "mesh_ply",mesh });
    }
    memcpy_s(lpBase, BUF_SIZE, (j.dump() + "\0").c_str(), j.dump().size() + 1);
    ReleaseSemaphore(read_sem, 1, NULL);
}