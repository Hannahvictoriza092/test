//
// Created by ����Դ on 24-5-16.
//

#include "LGraph.h"
#include <algorithm>
#include <vector>
#include <string>

namespace Graph {
    LGraph::LGraph(bool directed)
        : n_verts(0), m_edges(0), directed(directed), ver_list(std::vector<HeadNode>()) {}

    bool LGraph::exist_vertex(const std::string& name) const {
        //TODO:�ж��Ƿ����name���ƵĽڵ�
        return vertex_map.find(name) != vertex_map.end() && deleted.find(name) == deleted.end();
    }

    bool LGraph::exist_edge(const std::string& vertex_x_name, const std::string& vertex_y_name) const {
        //TODO:�ж��Ƿ����x-y�ı�
        if (!exist_vertex(vertex_x_name) )return false;
        Vertex x_id = vertex_map.at(vertex_x_name);
        for (const auto& edge : ver_list[x_id].adj) {
            if (ver_list[edge.dest].data.name == vertex_y_name) return true;
        }
        return false;
    }

    void LGraph::InsertVertex (const LocationInfo& vertex_info) {
        //TODO:����ڵ㣬�ڵ���Ϣ��LocationInfo�����
        if (exist_vertex(vertex_info.name)) {
        throw std::invalid_argument("Vertex already exists.");
    	}
    	vertex_map[vertex_info.name] = n_verts;
    	ver_list.emplace_back(HeadNode(vertex_info));
    	n_verts++;
    }

    void LGraph::DeleteVertex(const LocationInfo& vertex_info) {
        //TODO:ɾ���ڵ㣬�ڵ���Ϣ��LocationInfo�����
        if (!exist_vertex(vertex_info.name)) return;
	    Vertex v_id = vertex_map[vertex_info.name];
	    // ɾ�������Ըö���Ϊ���ı�
	    m_edges -= ver_list[v_id].adj.size();
	    ver_list[v_id].adj.clear();
	    // ɾ�����������ڽӱ���ָ��ö���ı�
	    for (auto& head : ver_list) {
	        head.adj.remove_if([v_id](const EdgeNode& e) { return e.dest == v_id; });
	    }
	    // ���¶���ӳ���ɾ������
	    vertex_map.erase(vertex_info.name);
	    deleted.insert(vertex_info.name);
	    n_verts--;
    }

    void LGraph::UpdateVertex(const LocationInfo& old_info, LocationInfo& new_info) {
        //TODO:���½ڵ㣬��/�ɽڵ����Ϣ��LocationInfo�����
        if (!exist_vertex(old_info.name)) {
        throw std::invalid_argument("Vertex does not exist.");
	    }
	    Vertex v_id = vertex_map[old_info.name];
	    ver_list[v_id].data = new_info;
	    if (old_info.name != new_info.name) {
	        vertex_map.erase(old_info.name);
	        vertex_map[new_info.name] = v_id;
	    }
    }

    VertInfo LGraph::GetVertex(const std::string& name) const {
        //TODO:��ȡ�ڵ㣬�ɽڵ�����ѯ�ڵ���Ϣ
        if (!exist_vertex(name)) {
        throw std::invalid_argument("Vertex does not exist.");
    }
    	return ver_list[vertex_map.at(name)].data;
    }

    VertInfo LGraph::GetVertex(const Vertex vertex) const {
        //TODO:��ȡ�ڵ㣬�ɽڵ��ID��ѯ
        if (vertex >= ver_list.size()) {
        throw std::out_of_range("Invalid vertex ID.");
    }
    	return ver_list[vertex].data;
    }

    void LGraph::InsertEdge(const std::string& vertex_x_name, const std::string& vertex_y_name, GElemSet weight) {
        //TODO:�����
        if (!exist_vertex(vertex_x_name) || !exist_vertex(vertex_y_name)) {
            throw std::invalid_argument("One or both vertices do not exist.");
        }
        Vertex x_id = vertex_map[vertex_x_name];
        Vertex y_id = vertex_map[vertex_y_name];
        // ���x->y�ı�
        ver_list[x_id].adj.emplace_back(x_id, y_id, weight);
        m_edges++;
        // ���������ͼ�����y->x�ı�
        if (!directed) {
            ver_list[y_id].adj.emplace_back(y_id, x_id, weight);
            m_edges++;
        }
    }

    void LGraph::DeleteEdge(const std::string& vertex_x_name, const std::string& vertex_y_name) {
        //TODO:ɾ���ߣ��������ڵ���ȷ��һ����
        if (!exist_edge(vertex_x_name, vertex_y_name)) return;
        Vertex x_id = vertex_map[vertex_x_name];
        Vertex y_id = vertex_map[vertex_y_name];
        // ɾ��x->y�ı�
        ver_list[x_id].adj.remove_if([y_id](const EdgeNode& e) { return e.dest == y_id; });
        m_edges--;
        // ���������ͼ��ɾ��y->x�ı�
        if (!directed) {
            ver_list[y_id].adj.remove_if([x_id](const EdgeNode& e) { return e.dest == x_id; });
            m_edges--;
        }
    }

    void LGraph::DeleteEdge(Vertex vertex_x, Vertex vertex_y) {
        //TODO:ɾ���ߣ��������ڵ�IDȷ��һ����
        if (vertex_x >= ver_list.size() || vertex_y >= ver_list.size()) return;
        // ɾ��x->y�ı�
        ver_list[vertex_x].adj.remove_if([vertex_y](const EdgeNode& e) { return e.dest == vertex_y; });
        m_edges--;
        // ���������ͼ��ɾ��y->x�ı�
        if (!directed) {
            ver_list[vertex_y].adj.remove_if([vertex_x](const EdgeNode& e) { return e.dest == vertex_x; });
            m_edges--;
        }
    }


    void LGraph::UpdateEdge(const std::string& vertex_x_name, const std::string& vertex_y_name, GElemSet new_weight) {
        //TODO:���±ߣ��������ڵ���ȷ��һ����
        if (!exist_edge(vertex_x_name, vertex_y_name)) {
            throw std::invalid_argument("Edge does not exist.");
        }
        Vertex x_id = vertex_map[vertex_x_name];
        Vertex y_id = vertex_map[vertex_y_name];
        // ����x->y�ı�
        for (auto& edge : ver_list[x_id].adj) {
            if (edge.dest == y_id) edge.weight = new_weight;
        }
        // ���������ͼ������y->x�ı�
        if (!directed) {
            for (auto& edge : ver_list[y_id].adj) {
                if (edge.dest == x_id) edge.weight = new_weight;
            }
        }
    }

    GElemSet LGraph::GetEdge(const std::string& vertex_x_name, const std::string& vertex_y_name) const {
        //TODO:��ȡ�ߵ���Ϣ
        if (!exist_edge(vertex_x_name, vertex_y_name)) {
            throw std::invalid_argument("Edge does not exist.");
        }
        Vertex x_id = vertex_map.at(vertex_x_name);
        for (const auto& edge : ver_list[x_id].adj) {
            if (ver_list[edge.dest].data.name == vertex_y_name) return edge.weight;
        }
        return GElemSet(); // �����ϲ���ִ�е�����
    }

    std::vector<EdgeNode> LGraph::SortedEdges(std::function<bool(const GElemSet&, const GElemSet&)> cmp) const {
        //TODO:��ȡ����Ȩ�͸���������������б�
        std::vector<EdgeNode> all_edges;
        for (const auto& head : ver_list) {
            for (const auto& edge : head.adj) {
                // ����ͼȥ�أ�ֻ����from < dest�ı�
                if (directed || edge.from < edge.dest) {
                    all_edges.push_back(edge);
                }
            }
        }
        std::sort(all_edges.begin(), all_edges.end(), [&cmp](const EdgeNode& a, const EdgeNode& b) {
            return cmp(a.weight, b.weight);
        });
        return all_edges;
    
    }


}
