#ifndef _TOOL_MESH_H_
#define _TOOL_MESH_H_

#include<map>
#include<vector>

#include <Mesh/BaseMesh.h>
#include <Mesh/Vertex.h>
#include <Mesh/HalfEdge.h>
#include <Mesh/Edge.h>
#include <Mesh/Face.h>
#include <Mesh/boundary.h>
#include <Mesh/iterators.h>
#include <Parser/parser.h>
#include <Geometry/Point.h>
#include <Geometry/Point2.H>
//#include <Eigen/Eigen>

using namespace std;

namespace MeshLib
{
	class CToolVertex : public CVertex
	{
	public:
		CToolVertex() {};
		~CToolVertex() {};
		void _to_string();
		void _from_string();

		CPoint& rgb() { return m_rgb; };
		void setColor(float, float, float);
		void removeEdge(CEdge*);
		bool onFront = false;
		bool onQuad = false;

	protected:
		CPoint m_rgb;
	};

	inline void CToolVertex::_from_string()
	{
		CParser parser(m_string);

		for (std::list<CToken*>::iterator iter = parser.tokens().begin(); iter != parser.tokens().end(); ++iter)
		{
			CToken* token = *iter;
			/*if (token->m_key == "uv")
			{
				double u, v;
				sscanf(token->m_value.c_str(), "(%lf %lf)", &u, &v);
				m_huv[0] = u;
				m_huv[1] = v;
			}*/
		}
	}

	inline void CToolVertex::_to_string()
	{
		std::string a;
		m_string = a;

		if (1)
		{
			CParser parser3(m_string);
			parser3._removeToken("rgb");
			parser3._toString(m_string);
			std::stringstream iss3;
			iss3 << "rgb=(" << m_rgb[0] << " " << m_rgb[1] << " " << m_rgb[2] << ")";
			if (m_string.length() > 0)
			{
				m_string += " ";
			}
			m_string += iss3.str();
		}
	}

	inline void CToolVertex::setColor(float r = 0, float g = 0, float b = 0)
	{
		m_rgb[0] = r;
		m_rgb[1] = g;
		m_rgb[2] = b;
	}

	inline void CToolVertex::removeEdge(CEdge* tar)
	{
		assert(tar);
		m_edges.remove(tar);
	}

	class CToolEdge :public CEdge
	{
	public:
		CToolEdge() {
		};
		~CToolEdge() {};
		void _to_string();
		void _from_string();

		double getWeight(bool useTriangle = false) {
			return 1.0;
		}
	protected:
	};

	inline void CToolEdge::_to_string()
	{
		std::string a;
		m_string = a;

	}

	inline void CToolEdge::_from_string()
	{
		CParser parser(m_string);

		for (std::list<CToken*>::iterator iter = parser.tokens().begin(); iter != parser.tokens().end(); ++iter)
		{
			CToken* token = *iter;
			/*if (token->m_key == "uv")
			{
			double u, v;
			sscanf(token->m_value.c_str(), "(%lf %lf)", &u, &v);
			m_huv[0] = u;
			m_huv[1] = v;
			}*/
		}
	}

	class CToolFace : public CFace
	{
	public:
		CToolFace() {};
		~CToolFace() {};
		void _to_string();
	};

	inline void CToolFace::_to_string()
	{
		std::string a;
		m_string = a;
	}

	class CToolHalfEdge : public CHalfEdge
	{
	public:
		CToolHalfEdge() {};
		~CToolHalfEdge() {};
		double& angle() { return m_angle; };
		void _to_string();
		CToolHalfEdge* leftSide = NULL;  // upright halfedge
		CToolHalfEdge* rightSide = NULL; // upright halfedge
		CToolHalfEdge* topEdge = NULL;
		CToolHalfEdge* nextFe = NULL;
		CToolHalfEdge* prevFe = NULL;
		char classification = -1;
		bool needTopEdge = true;
		bool isFront = false;

	protected:
		double m_angle = 0;
	};

	inline void CToolHalfEdge::_to_string()
	{
		//std::string a;
		//m_string = a;
	}

	template<typename V, typename E, typename F, typename H>
	class CToolMesh : public CBaseMesh<V, E, F, H>
	{
	public:
		typedef V CVertex;
		typedef E CEdge;
		typedef F CFace;
		typedef H CHalfEdge;

		typedef CBoundary<V, E, F, H> CBoundary;
		typedef CLoop<V, E, F, H> CLoop;

		typedef MeshVertexIterator<V, E, F, H> MeshVertexIterator;
		typedef MeshEdgeIterator<V, E, F, H> MeshEdgeIterator;
		typedef VertexVertexIterator<V, E, F, H> VertexVertexIterator;
		typedef VertexEdgeIterator<V, E, F, H> VertexEdgeIterator;
		typedef MeshFaceIterator<V, E, F, H> MeshFaceIterator;
		typedef FaceVertexIterator<V, E, F, H> FaceVertexIterator;
		typedef VertexFaceIterator<V, E, F, H> VertexFaceIterator;
		typedef FaceHalfedgeIterator<V, E, F, H> FaceHalfedgeIterator;
		typedef VertexOutHalfedgeIterator<V, E, F, H> VertexOutHalfedgeIterator;
		typedef VertexInHalfedgeIterator<V, E, F, H> VertexInHalfedgeIterator;
		typedef FaceEdgeIterator<V, E, F, H> FaceEdgeIterator;

	public:
		void read_m(const char* input);
		void deleteVertex(CVertex*);
		void deleteEdge(CEdge*);

		V* addVertex(CEdge*);

		// Can NOT process a vertex with 0 or 1 edge connected to it.
		E* addEdge(CVertex*, CVertex*);

		// Assumes adjacent faces of the edge are all triangles.
		V* splitEdge(CEdge*, CPoint&);

		// Assumes the two adjacent faces of the edge are both triangles.
		E* swapEdge(CEdge*);

		bool inside(V* [4], V*);
		void findInternalVertices(V* [4], V*, set<V*>&);
		void clearQuad(H* [4]);

		// get next boundary front halfedge
		H* nextBoundaryHalfedge(CHalfEdge*);

		int numQuad(CToolEdge* edge);
		int numQuad(CToolVertex* vert);
		bool isQuad(CToolFace* face);
		H* sourceTargetHalfEedge(CToolVertex*, CToolVertex*);

	protected:
		int nextVid = 0;
		int nextFid = 0;

		V* addVertexOnBoundary(CEdge*);
		V* splitEdgeOnBoundary(CEdge*, CPoint&);
	};

	template<typename V, typename E, typename F, typename H>
	void CToolMesh<V, E, F, H>::read_m(const char* input)
	{
		CBaseMesh<V, E, F, H>::read_m(input);
		for (auto i = m_verts.begin(); i != m_verts.end(); i++)
			nextVid = this->vertexId(*i) > nextVid ? this->vertexId(*i) : nextVid;
		nextVid++;
		for (auto i = m_faces.begin(); i != m_faces.end(); i++)
			nextFid = this->faceId(*i) > nextFid ? this->faceId(*i) : nextFid;
		nextFid++;
	}

	template<typename V, typename E, typename F, typename H>
	void CToolMesh<V, E, F, H>::deleteVertex(CVertex* tar)
	{
		assert(tar != NULL);
		VertexEdgeIterator eiter(tar);
		list<E*> edges;
		for (eiter; !eiter.end(); eiter++)
			edges.push_back(*eiter);
		for (auto i = edges.begin(); i != edges.end(); i++)
			deleteEdge(*i);
		m_map_vert.erase(vertexId(tar));
		m_verts.remove(tar);

		delete tar;
	}

	template<typename V, typename E, typename F, typename H>
	void CToolMesh<V, E, F, H>::deleteEdge(CEdge* tar)
	{
		assert(tar != NULL);
		CVertex* vert[] = { edgeVertex1(tar),edgeVertex2(tar) };
		CHalfEdge* hEdge[] = { edgeHalfedge(tar, 0),edgeHalfedge(tar,1) };
		CFace* face[] = { edgeFace1(tar), edgeFace2(tar) };

		// If any vertex or face have these two halfedges as member value,
		// change it to another one.
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (vertexHalfedge(vert[i]) == hEdge[j])
					vert[i]->halfedge() = vertexNextCcwInHalfEdge(hEdge[j]);
				if (faceHalfedge(face[i]) == hEdge[j])
					face[i]->halfedge() = halfedgeNext(hEdge[j]);
			}
		}

		// For the previous and next halfedges, change their members accordingly.
		CHalfEdge* prev[] = { halfedgePrev(hEdge[0]), halfedgePrev(hEdge[1]) };
		CHalfEdge* next[] = { halfedgeNext(hEdge[0]), halfedgeNext(hEdge[1]) };
		assert(prev[0]);
		assert(prev[1]);
		assert(next[0]);
		assert(next[1]);
		prev[0]->he_next() = next[1];
		next[0]->he_prev() = prev[1];
		prev[1]->he_next() = next[0];
		next[1]->he_prev() = prev[0];

		vert[0]->removeEdge(tar);
		vert[1]->removeEdge(tar);
		m_edges.remove(tar);
		delete tar->halfedge(0);
		delete tar->halfedge(1);
		delete tar;

		// Merge two adjecent face
		// Due to the structure that all the vertices of a face are recorded 
		// only by a single halfedge variable, so all we need to do is just
		// delete one of them.

		if (face[0] == face[1])
			return;
		list<CTMesh::CHalfEdge*> hEdges;
		CTMesh::CHalfEdge* curHE = faceHalfedge(face[1]);
		hEdges.push_back(curHE);
		curHE = halfedgeNext(curHE);
		while (curHE != faceHalfedge(face[1]))
		{
			hEdges.push_back(curHE);
			curHE = halfedgeNext(curHE);
		}
		for (auto iter = hEdges.begin(); iter != hEdges.end(); iter++)
			(*iter)->face() = face[0];

		m_faces.remove(face[1]);
		m_map_face.erase(faceId(face[1]));
		delete face[1];
	}

	template<typename V, typename E, typename F, typename H>
	V* CToolMesh<V, E, F, H>::addVertex(CEdge* oldEdge)
	{
		H* oldHE[2] = { edgeHalfedge(oldEdge,0), edgeHalfedge(oldEdge,1) };
		if (oldHE[0] == NULL || oldHE[1] == NULL)
			return addVertexOnBoundary(oldEdge);
		H* inHE[2] = { halfedgePrev(oldHE[0]), halfedgePrev(oldHE[1]) };
		H* outHE[2] = { halfedgeNext(oldHE[1]), halfedgeNext(oldHE[0]) };
		V* vert[2] = { halfedgeSource(oldHE[0]), halfedgeTarget(oldHE[0]) };
		F* face[2] = { halfedgeFace(oldHE[0]), halfedgeFace(oldHE[1]) };

		H* new0in, * new0out, * new1in, * new1out;
		new0in = new H;
		new0out = new H;
		new1in = new H;
		new1out = new H;
		E* newEdge[2] = { new E, new E };
		V* newVert = new CVertex();


		new0out->edge() = newEdge[0];
		newEdge[0]->halfedge(0) = new0out;
		new0out[0].face() = face[0];
		face[0]->halfedge() = new0out;
		new0out->he_prev() = inHE[0];
		inHE[0]->he_next() = new0out;
		new0out->he_next() = new1in;
		new0out->vertex() = newVert;
		newVert->halfedge() = new0out;

		new0in->edge() = newEdge[0];
		newEdge[0]->halfedge(1) = new0in;
		new0in->face() = face[1];
		face[1]->halfedge() = new0in;
		new0in->he_next() = outHE[0];
		outHE[0]->he_prev() = new0in;
		new0in->he_prev() = new1out;
		new0in->vertex() = vert[0];
		vert[0]->halfedge() = new0in;

		new1in->edge() = newEdge[1];
		newEdge[1]->halfedge(0) = new1in;
		new1in->face() = face[0];
		new1in->he_next() = outHE[1];
		outHE[1]->he_prev() = new1in;
		new1in->he_prev() = new0out;
		new1in->vertex() = vert[1];
		vert[1]->halfedge() = new1in;

		new1out->edge() = newEdge[1];
		newEdge[1]->halfedge(1) = new1out;
		new1out->face() = face[1];
		new1out->he_next() = new0in;
		new1out->he_prev() = inHE[1];
		inHE[1]->he_next() = new1out;
		new1out->vertex() = newVert;

		vert[0]->edges().push_back(newEdge[0]);
		vert[0]->edges().remove(oldEdge);
		vert[1]->edges().push_back(newEdge[1]);
		vert[1]->edges().remove(oldEdge);


		newVert->id() = nextVid++;
		newVert->boundary() = false;
		newVert->point() = (vert[0]->point() + vert[1]->point()) / 2;
		m_verts.push_back(newVert);
		m_map_vert.insert(std::pair<int, CVertex*>(newVert->id(), newVert));

		m_edges.remove(oldEdge);
		delete oldEdge;
		delete oldHE[0];
		delete oldHE[1];
		return newVert;
	}

	template<typename V, typename E, typename F, typename H>
	V* CToolMesh<V, E, F, H>::addVertexOnBoundary(CEdge* oldEdge)
	{
		H* oldHE = edgeHalfedge(oldEdge, 0) == NULL ? edgeHalfedge(oldEdge, 1) : edgeHalfedge(oldEdge, 0);
		V* vert[2] = { halfedgeSource(oldHE), halfedgeTarget(oldHE) };
		F* face = halfedgeFace(oldHE);
		H* inHE = halfedgePrev(oldHE);
		H* outHE = halfedgeNext(oldHE);

		H* new0out, * new1in;
		new0out = new H;
		new1in = new H;
		E* newEdge[2] = { new E, new E };
		V* newVert = new CVertex();


		new0out->edge() = newEdge[0];
		newEdge[0]->halfedge(0) = new0out;
		new0out[0].face() = face;
		face->halfedge() = new0out;
		new0out->he_prev() = inHE;
		inHE->he_next() = new0out;
		new0out->he_next() = new1in;
		new0out->vertex() = newVert;
		newVert->halfedge() = new0out;

		new1in->edge() = newEdge[1];
		newEdge[1]->halfedge(0) = new1in;
		new1in->face() = face;
		new1in->he_next() = outHE;
		outHE->he_prev() = new1in;
		new1in->he_prev() = new0out;
		new1in->vertex() = vert[1];
		vert[1]->halfedge() = new1in;

		vert[0]->edges().push_back(newEdge[0]);
		vert[0]->edges().remove(oldEdge);
		vert[1]->edges().push_back(newEdge[1]);
		vert[1]->edges().remove(oldEdge);


		newVert->id() = nextVid++;
		newVert->boundary() = true;
		newVert->point() = (vert[0]->point() + vert[1]->point()) / 2;
		m_verts.push_back(newVert);
		m_map_vert.insert(std::pair<int, CVertex*>(newVert->id(), newVert));

		m_edges.remove(oldEdge);
		delete oldEdge;
		delete oldHE;
		return newVert;
	}

	template<typename V, typename E, typename F, typename H>
	E* CToolMesh<V, E, F, H>::addEdge(CVertex* v1, CVertex* v2)
	{
		list<E*> edges[2];
		for (VertexEdgeIterator eIter(v1); !eIter.end(); eIter++)
			edges[0].push_back(*eIter);
		for (VertexEdgeIterator eIter(v2); !eIter.end(); eIter++)
			edges[1].push_back(*eIter);



		assert(edges[0].size() > 1);
		assert(edges[1].size() > 1);

		V* vert[2] = { v1,v2 };
		F* oldFace = NULL;
		//Find the face on which the two vertex are.
		for (VertexFaceIterator fIter(vert[0]); !fIter.end(); fIter++)
		{
			for (FaceVertexIterator vIter(*fIter); !vIter.end(); vIter++)
			{
				if (*vIter == vert[1])
				{
					oldFace = *fIter;
					break;
				}
			}
		}
		assert(oldFace != NULL);

		E* newEdge = CBaseMesh<V, E, F, H>::createEdge(v1, v2);
		H* he[2] = { new H, new H };
		newEdge->halfedge(0) = he[0];
		newEdge->halfedge(1) = he[1];
		he[0]->edge() = he[1]->edge() = newEdge;
		he[0]->target() = vert[0];
		he[1]->target() = vert[1];

		H* inHe[2], * outHe[2];
		inHe[0] = inHe[1] = outHe[0] = outHe[1] = NULL;

		for (int i = 0; i < 2; i++)
		{
			for (list<E*>::iterator eIter = edges[i].begin(); eIter != edges[i].end(); eIter++)
			{
				for (int j = 0; j < 2; j++)
				{
					H* oldHe = edgeHalfedge(*eIter, j);
					if (oldHe == NULL)
						continue;
					if (halfedgeFace(oldHe) != oldFace)
						continue;
					if (halfedgeTarget(oldHe) == vert[i])
						inHe[i] = oldHe;
					else
						outHe[i] = oldHe;
				}
			}
		}

		he[0]->he_next() = outHe[0];
		outHe[0]->he_prev() = he[0];

		he[0]->he_prev() = inHe[1];
		inHe[1]->he_next() = he[0];

		he[1]->he_next() = outHe[1];
		outHe[1]->he_prev() = he[1];

		he[1]->he_prev() = inHe[0];
		inHe[0]->he_next() = he[1];

		he[0]->face() = oldFace;
		oldFace->halfedge() = he[0];

		F* newFace = new CFace();
		newFace->id() = nextFid++;
		newFace->halfedge() = he[1];
		he[1]->face() = newFace;
		for (H* curHe = halfedgeNext(he[1]); curHe != he[1]; curHe = halfedgeNext(curHe))
			curHe->face() = newFace;
		m_faces.push_back(newFace);
		m_map_face.insert(std::pair<int, tFace>(newFace->id(), newFace));

		return newEdge;
	}

	template<typename V, typename E, typename F, typename H>
	V* CToolMesh<V, E, F, H>::splitEdge(CEdge* oldEdge, CPoint& pos)
	{
		if (edgeHalfedge(oldEdge, 0) == NULL || edgeHalfedge(oldEdge, 1) == NULL)
			return splitEdgeOnBoundary(oldEdge, pos);
		assert(numQuad(oldEdge) == 0);
		V* oldVert[2];
		oldVert[0] = halfedgeTarget(halfedgeNext(edgeHalfedge(oldEdge, 0)));
		oldVert[1] = halfedgeTarget(halfedgeNext(edgeHalfedge(oldEdge, 1)));
		V* newVert = addVertex(oldEdge);
		newVert->point() = pos;
		addEdge(oldVert[0], newVert);
		addEdge(oldVert[1], newVert);
		return newVert;
	}

	template<typename V, typename E, typename F, typename H>
	V* CToolMesh<V, E, F, H>::splitEdgeOnBoundary(CEdge* oldEdge, CPoint& pos)
	{
		H* oldHE = edgeHalfedge(oldEdge, 0);
		if (oldHE == NULL)
			oldHE = edgeHalfedge(oldEdge, 1);
		V* oldVert = halfedgeTarget(halfedgeNext(oldHE));
		V* newVert = addVertex(oldEdge);
		newVert->point() = pos;
		addEdge(oldVert, newVert);
		return newVert;
	}

	template<typename V, typename E, typename F, typename H>
	E* CToolMesh<V, E, F, H>::swapEdge(CEdge* oldEdge)
	{
		assert(numQuad(oldEdge) == 0);
		V* oldVert[2];
		oldVert[0] = halfedgeTarget(halfedgeNext(edgeHalfedge(oldEdge, 0)));
		oldVert[1] = halfedgeTarget(halfedgeNext(edgeHalfedge(oldEdge, 1)));
		deleteEdge(oldEdge);
		return addEdge(oldVert[0], oldVert[1]);
	}

	template<typename V, typename E, typename F, typename H>
	bool CToolMesh<V, E, F, H>::inside(V* box[4], V* tar)
	{
		for (int i = 0; i < 4; i++)
			if (box[i] == tar)
				return false;
		CPoint edge[4], ref[4], n;
		int ret[4];
		for (int i = 0; i < 4; i++)
		{
			edge[i] = box[(i + 1) % 4]->point() - box[i]->point();
			ref[i] = tar->point() - box[i]->point();
		}
		n = ((edge[0] ^ edge[1]) + (edge[2] ^ edge[3])) / 2;
		for (int i = 0; i < 4; i++)
			ret[i] = (edge[i] ^ ref[i]) * n > 0 ? 1 : -1;
		for (int i = 0; i < 4; i++)
			if (ret[i] * ret[(i + 1) % 4] < 0)
				return false;
		return true;
	}

	template<typename V, typename E, typename F, typename H>
	void CToolMesh<V, E, F, H>::findInternalVertices(V* box[4], V* cur, set<V*>& st)
	{
		if (inside(box, cur))
		{
			if (st.find(cur) != st.end())
				return;
			st.insert(cur);
			for (VertexVertexIterator vIter(cur); !vIter.end(); vIter++)
				findInternalVertices(box, *vIter, st);
		}
		else
			return;
	}

	template<typename V, typename E, typename F, typename H>
	void CToolMesh<V, E, F, H>::clearQuad(H* edge[4])
	{
		assert(halfedgeTarget(edge[0]) == halfedgeSource(edge[1]));
		assert(halfedgeTarget(edge[1]) == halfedgeSource(edge[2]));
		assert(halfedgeTarget(edge[2]) == halfedgeSource(edge[3]));
		assert(halfedgeTarget(edge[3]) == halfedgeSource(edge[0]));

		V* box[4];
		box[0] = halfedgeSource(edge[0]);
		box[1] = halfedgeTarget(edge[0]);
		for (int i = 2; i < 4; i++)
			for (int j = i - 1; j < 4; j++)
				if (halfedgeSource(edge[j]) == box[i - 1])
					box[i] = halfedgeTarget(edge[j]);

		set<V*> st;
		V* starter = NULL;
		for (int i = 0; i < 4; i++)
		{
			for (VertexVertexIterator vIter(box[i]); !vIter.end(); vIter++)
			{
				if (inside(box, *vIter))
				{
					starter = *vIter;
					break;
				}
			}
			if (starter != NULL)
				break;
		}

		if (starter != NULL)
		{
			findInternalVertices(box, starter, st);
			for (set<V*>::iterator i = st.begin(); i != st.end(); i++)
				deleteVertex(*i);
		}
		
		E* diagonal[2] = { vertexEdge(box[0],box[2]), vertexEdge(box[1], box[3]) };

		for (int i = 0; i < 2; i++)
			if (diagonal[i] != NULL)
				deleteEdge(diagonal[i]);

		return;
	}

	template<typename V, typename E, typename F, typename H>
	H* CToolMesh<V, E, F, H>::nextBoundaryHalfedge(CHalfEdge* he) {
		CVertex* target = this->halfedgeTarget(he);
		return this->vertexMostClwOutHalfEdge(target);
	}

	template<typename V, typename E, typename F, typename H>
	int CToolMesh<V, E, F, H>::numQuad(CToolEdge* edge)
	{
		assert(edge != NULL);
		CToolFace* faces[2];
		faces[0] = edge->halfedge(0) == NULL ? NULL : edgeFace1(edge);
		faces[1] = edge->halfedge(1) == NULL ? NULL : edgeFace2(edge);
		int num = 0;
		for (int i = 0; i < 2; i++)
			if (faces[i] != NULL && isQuad(faces[i]))
				num++;
		return num;
	}

	template<typename V, typename E, typename F, typename H>
	int CToolMesh<V, E, F, H>::numQuad(CToolVertex* vert)
	{
		assert(vert != NULL);
		int num = 0;
		for (CTMesh::VertexFaceIterator vfiter(vert); !vfiter.end(); vfiter++)
			if (isQuad(*vfiter))
				num++;
		return num;
	}

	template<typename V, typename E, typename F, typename H>
	bool CToolMesh<V, E, F, H>::isQuad(CToolFace* face)
	{
		assert(face != NULL);
		int vertNum = 0;
		for (CTMesh::FaceVertexIterator fviter(face); !fviter.end(); fviter++)
			vertNum++;
		return vertNum == 4;
	}

	template<typename V, typename E, typename F, typename H>
	H* CToolMesh<V, E, F, H>::sourceTargetHalfEedge(CToolVertex* source, CToolVertex* target)
	{
		assert(source != NULL && target != NULL);
		H* result = vertexHalfedge(source, target);
		if (halfedgeTarget(result) != target)
			result = halfedgeSym(result);
		assert(result != NULL);
		return result;
	}


	typedef CToolMesh<CToolVertex, CToolEdge, CToolFace, CToolHalfEdge> CTMesh;
}


#endif