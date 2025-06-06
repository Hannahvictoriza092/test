//
// Created by ����Դ on 24-10-13.
//
#include "Algorithm.h"
#include <queue>
#include "LGraph.h"
#include "LGraph.cpp"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <sstream>
#include <cstdlib> 
#include <ctime>  


namespace Graph {
    namespace Algorithm {
        std::list<Vertex> GetCircuit(LGraph& graph, Vertex start) {
            //TODO:�Ӹ�����������һ����·
            std::list<Vertex> circuit;
            Vertex head = start;
            circuit.push_back(head);

            while (!graph.List()[head].adj.empty()) {
                Vertex tail = graph.List()[head].adj.front().dest;
                
                // ɾ���� head - tail
                graph.DeleteEdge(head, tail);

                circuit.push_back(tail);

                if (tail == start) {
                    break;  // ��·�պϣ��˳�
                }
                head = tail;
            }
            return circuit;
        }



        std::list<Vertex> EulerCircle(const LGraph& graph) {
            //TODO:��ȡŷ����·,�����ʹ��GetCircuit����
            if (!IsConnected(graph)) return {};  // ͼ����ͨ

            Vertex n = graph.VertexCount();
            for (Vertex i = 0; i < n; ++i) {
                if (graph.List()[i].adj.size() % 2 != 0) {
                    return {};  // ���������Ƚڵ�
                }
            }
            LGraph tmp_graph = graph;  // ����ͼ�Ա��޸�
            std::list<Vertex> circuit = GetCircuit(tmp_graph, 0);  // �Ӷ���0��ʼ

            for (auto it = circuit.begin(); it != circuit.end(); ++it) {
                Vertex v = *it;
                while (!tmp_graph.List()[v].adj.empty()) {
                    // ��ʣ��ߣ���v�����µĻ�·
                    std::list<Vertex> sub_circuit = GetCircuit(tmp_graph, v);
                    // �����·
                    auto insert_pos = std::next(it);
                    circuit.insert(insert_pos, std::next(sub_circuit.begin()), sub_circuit.end());
                }
            }

            return circuit;
        }

        

        bool HaveEulerCircle(const LGraph& graph) {
            //TODO:�ж��Ƿ���ŷ����·
            if ( !IsConnected(graph)){
                return false;
            }
            else{
                Vertex n=graph.VertexCount();
                for (size_t i=0;i<n;++i){
                    Vertex degree=graph.List()[i].adj.size();
                    if (degree%2 ==1) {
                        return false;
                    }
                }
                return true;
            }
        }

        void BFSv(const LGraph& graph, Vertex v, std::vector<bool>& visited) {
            //TODO:���������������ͼ
            std::queue<Vertex> q;
            q.push(v);
            visited[v]=true;
            while( !q.empty() ){
                Vertex u = q.front();
                q.pop();
                for (const auto& edge : graph.List()[u].adj) {
                    Graph::Vertex w = edge.dest;
                    if( !visited[w]){
                        visited[w]=true;
                        q.push(w);
                    }
                }
            }
        }

        bool IsConnected (const LGraph& graph) {
            Vertex n = graph.VertexCount();
            if (n == 0) return true; // ��ͼ����ͨ��
            
            std::vector<bool> visited(n, false);
            
            // ִ�� BFS ��������ͼ
            BFSv(graph, 0, visited);
            
            // ����Ƿ����ж��㶼������
            for (Vertex i = 0; i < n; i++) {
                if (!visited[i]) 
                    return false;
            }
            return true;
        }

        ShortestPathResult GetShortestPath(const LGraph& graph, const std::string& vertex_name_x, const std::string& vertex_name_y) {
            //TODO:��ȡ����֮������·��
            size_t n = graph.VertexCount();
             const std::map<std::string, Graph::Vertex>& name_to_id = graph.Map();
            // �ж��������Ƿ����
            if (name_to_id.count(vertex_name_x) == 0 || name_to_id.count(vertex_name_y) == 0) { //count��ͳ�Ƴ��ֵĴ���
                std::cerr << "����Ķ����������ڣ�" << std::endl;
            }
            // ��ȡ���
            Vertex start = name_to_id.at(vertex_name_x);
            Vertex end = name_to_id.at(vertex_name_y);

            std::vector<bool> collectedist(n, false);   // �Ƿ�����¼�����·����
            std::vector<Vertex> dist(n, INT_MAX);        // ��̾������飬��ʼ��Ϊ�����
            std::vector<Vertex> path(n, Graph::NIL);            // ǰ�����飬��ʼ��ΪNIL 
            dist[start]=0;  //��㵽�Լ��ľ�����0
            for (const EdgeNode &edge :graph.List()[start].adj){
                dist[edge.dest]=edge.weight;
                path[edge.dest]=start;
            }
            collectedist[start]=true;
            for (size_t i=0;i<n;++i){
                // �ҵ�ǰδ����¼�ġ�dist��С�Ķ���
                int mindist = INT_MAX;
                Vertex u =NIL;
                for (Vertex v = 0; v < n; ++v) {
                    if (!collectedist[v] && dist[v] < mindist) {
                        mindist = dist[v];
                        u = v;
                    }
                }
                if (u == Graph::NIL) break; // ȫ����¼���Ҳ����ɴﶥ��
                collectedist[u] = true;
                // ����u�ڽӵ��dist
                for (const auto& edge : graph.List()[u].adj) {
                    Vertex v = edge.dest;
                    if (!collectedist[v] && dist[u] + edge.weight < dist[v]) {
                        dist[v] = dist[u] + edge.weight;
                        path[v] = u;
                    }
                }
            }
            ShortestPathResult result;
            if (dist[end] == INT_MAX) {
                result.distance = -1;
                result.path_string = "���ɴ�";
                return result;
            }
            // ����·��
            std::vector<Vertex> result_path;
            for (Vertex v = end; v != Graph::NIL; v = path[v]) {
                result_path.push_back(v);
            }
            std::reverse(result_path.begin(), result_path.end());

            // ӳ�� ID -> ����
            std::map<Vertex, std::string> id_to_name;
            for (const auto& pair : name_to_id) {
                id_to_name[pair.second] = pair.first;
            }

            std::ostringstream oss;
            for (size_t i = 0; i < result_path.size(); ++i) {
                oss << id_to_name[result_path[i]];
                if (i < result_path.size() - 1) oss << " ";
            }

            result.distance = dist[end];
            result.path_string = oss.str();
            return result;
            }


        int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path) {
            //TODO:��ȡ�������޵����·������������path����
            const int INF = 1e9;
            std::unordered_map<std::string, int> dist;
            std::unordered_map<std::string, std::string> prev;
            // ��ʼ�����е�Ϊ INF
            for (const auto& name : path) {
                dist[name] = INF;
            }
            // ������Ϊ 0
            dist[path[0]] = 0;
            // �����������д���ÿ����
            for (const auto& u_name : path) {
                if (!graph.exist_vertex(u_name)) continue;
                if (graph.Map().count(u_name) == 0) continue; // ���ͼ��û��������֣�����
                Vertex u = graph.Map().at(u_name);
                for (const auto& edge : graph.List()[u].adj) {
                    std::string v_name = graph.GetVertex(edge.dest).name;
                    // ֻ��������������ĵ�
                    if (dist.find(v_name) != dist.end()) {
                        int new_dist = dist[u_name] + edge.weight; 
                        if (new_dist < dist[v_name]) {
                            dist[v_name] = new_dist;
                            prev[v_name] = u_name;  // ��¼·��
                        }
                    }
                }
            }
            std::string end = path.back();
            if (dist[end] == INF) {
                std::cout << "�޿���·��" << std::endl;
                return -1;
            }
            // ׷��·��
            std::vector<std::string> route;
            std::string cur = end;
            while (cur != path[0]) {
                route.push_back(cur);
                cur = prev[cur];
            }
            route.push_back(path[0]);
            std::reverse(route.begin(), route.end());
            // ���·��
            std::cout << "���·��Ϊ��";
            for (size_t i = 0; i < route.size(); ++i) {
                std::cout << route[i];
                if (i + 1 != route.size()) std::cout << " -> ";
            }
            std::cout << std::endl;

            return dist[end];
        }

        

        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph) {
            //TODO:������С�����������������ϵı�
            std::vector<EdgeNode> res;
            std::vector<EdgeNode> edges = graph.SortedEdges();  //��Ȩֵ����
            size_t n= graph.VertexCount();
            Algorithm:: DSU dsu(n);     //�������鼯

            for (const EdgeNode &edge : edges){
                if( !dsu.same(edge.from,edge.dest)){    //�������ͬһ������
                    dsu.unite(edge.from,edge.dest);     //�ϲ�
                    res.push_back(edge);
                    if(res.size() == n-1){  //��С����������Ϊ(n-1)
                        break;
                    }
                }
            }
            return res;
        }

        PlanVisitResult PlanTeachingVisit(const LGraph& graph, const std::string& start_gate) {
            const auto& name_to_id = graph.Map();
            if (name_to_id.count(start_gate) == 0) {
                throw std::invalid_argument("���У�Ų����ڣ�");
            }

            Vertex start = name_to_id.at(start_gate);
            std::vector<Vertex> teaching_buildings;

            // �ҳ����н�ѧ¥
            for (Vertex v = 0; v < graph.VertexCount(); ++v) {
                Graph::LocationInfo info = graph.GetVertex(v);
                if (info.type == "Teaching_Research_and_Administration") {
                    teaching_buildings.push_back(v);
                }
            }

            // ���� id �� name ӳ��
            std::map<Vertex, std::string> id_to_name;
            for (const auto& pair : name_to_id) {
                id_to_name[pair.second] = pair.first;
            }

            std::set<Vertex> unvisited(teaching_buildings.begin(), teaching_buildings.end());
            Vertex current = start;
            std::vector<Vertex> visit_order;
            int total_time_seconds = 0;

            visit_order.push_back(current);

            while (!unvisited.empty()) {
                int min_dist = INT_MAX;
                Vertex next = Graph::NIL;
                ShortestPathResult best_path;

                for (Vertex v : unvisited) {
                    ShortestPathResult sp = GetShortestPath(graph, id_to_name[current], id_to_name[v]);
                    if (sp.distance != -1 && sp.distance < min_dist) {
                        min_dist = sp.distance;
                        next = v;
                        best_path = sp;
                    }
                }

                if (next == Graph::NIL) {
                    throw std::runtime_error("���ֽ�ѧ¥���ɴ");
                }

                // ·��ʱ��
                total_time_seconds += best_path.distance;

                // �ι�ʱ��
                total_time_seconds += graph.GetVertex(next).visitTime * 60;

                visit_order.push_back(next);
                current = next;
                unvisited.erase(next);
            }

            // ����·���ַ���
            std::ostringstream oss;
            for (size_t i = 0; i < visit_order.size(); ++i) {
                oss << id_to_name[visit_order[i]];
                if (i < visit_order.size() - 1) oss << " ";
            }

            PlanVisitResult result;
            result.path_string = oss.str();
            result.total_time_minutes = (total_time_seconds + 30) / 60;
            return result;
        }


        std::vector<ConnectionEdge> GetConnectionSuggestions(const LGraph& graph) {
            std::vector<ConnectionEdge> suggestions;
            Vertex n = graph.VertexCount();
            if (n == 0) return suggestions;

            // ��ʼ���������
            std::srand(static_cast<unsigned int>(std::time(nullptr)));

            // ��ͨ��������
            std::vector<bool> visited(n, false);
            std::vector<int> component(n, -1);
            std::vector<std::vector<Vertex>> components;
            
            for (Vertex v = 0; v < n; v++) {
                if (!visited[v]) {
                    std::vector<Vertex> comp;
                    std::queue<Vertex> q;
                    q.push(v);
                    visited[v] = true;
                    comp.push_back(v);
                    
                    while (!q.empty()) {
                        Vertex u = q.front();
                        q.pop();
                        
                        for (const auto& edge : graph.List()[u].adj) {
                            Vertex w = edge.dest;
                            if (!visited[w]) {
                                visited[w] = true;
                                comp.push_back(w);
                                q.push(w);
                            }
                        }
                    }
                    components.push_back(comp);
                }
            }
            
            // ���ͼ����ͨ������Ҫ��ӱ�
            if (components.size() <= 1) {
                return suggestions;
            }
            
            // ���� ID �����Ƶ�ӳ��
            std::map<Vertex, std::string> id_to_name;
            const auto& name_to_id = graph.Map();
            for (const auto& pair : name_to_id) {
                id_to_name[pair.second] = pair.first;
            }
            
            // Ϊÿ����ͨ����֮�����һ����
            for (size_t i = 1; i < components.size(); i++) {
                // ��ǰһ���������ѡһ������
                Vertex from_idx = components[i-1][std::rand() % components[i-1].size()];
                std::string from = id_to_name[from_idx];
                
                // �ӵ�ǰ�������ѡһ������
                Vertex to_idx = components[i][std::rand() % components[i].size()];
                std::string to = id_to_name[to_idx];
                
                // ���Ȩ�أ�50-150��
                int weight = 50 + std::rand() % 101;
                
                suggestions.push_back({from, to, weight});
            }
            
            return suggestions;
        }
        
    }
}