//
// Created by 刘凯源 on 24-10-13.
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
            //TODO:从给定点出发获得一条回路
            std::list<Vertex> circuit;
            Vertex head = start;
            circuit.push_back(head);

            while (!graph.List()[head].adj.empty()) {
                Vertex tail = graph.List()[head].adj.front().dest;
                
                // 删除边 head - tail
                graph.DeleteEdge(head, tail);

                circuit.push_back(tail);

                if (tail == start) {
                    break;  // 回路闭合，退出
                }
                head = tail;
            }
            return circuit;
        }



        std::list<Vertex> EulerCircle(const LGraph& graph) {
            //TODO:获取欧拉回路,你可以使用GetCircuit函数
            if (!IsConnected(graph)) return {};  // 图不连通

            Vertex n = graph.VertexCount();
            for (Vertex i = 0; i < n; ++i) {
                if (graph.List()[i].adj.size() % 2 != 0) {
                    return {};  // 存在奇数度节点
                }
            }
            LGraph tmp_graph = graph;  // 拷贝图以便修改
            std::list<Vertex> circuit = GetCircuit(tmp_graph, 0);  // 从顶点0开始

            for (auto it = circuit.begin(); it != circuit.end(); ++it) {
                Vertex v = *it;
                while (!tmp_graph.List()[v].adj.empty()) {
                    // 有剩余边，从v构造新的回路
                    std::list<Vertex> sub_circuit = GetCircuit(tmp_graph, v);
                    // 插入回路
                    auto insert_pos = std::next(it);
                    circuit.insert(insert_pos, std::next(sub_circuit.begin()), sub_circuit.end());
                }
            }

            return circuit;
        }

        

        bool HaveEulerCircle(const LGraph& graph) {
            //TODO:判断是否有欧拉回路
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
            //TODO:广度优先搜索整个图
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
            if (n == 0) return true; // 空图是连通的
            
            std::vector<bool> visited(n, false);
            
            // 执行 BFS 遍历整个图
            BFSv(graph, 0, visited);
            
            // 检查是否所有顶点都被访问
            for (Vertex i = 0; i < n; i++) {
                if (!visited[i]) 
                    return false;
            }
            return true;
        }

        ShortestPathResult GetShortestPath(const LGraph& graph, const std::string& vertex_name_x, const std::string& vertex_name_y) {
            //TODO:获取两点之间的最短路径
            size_t n = graph.VertexCount();
             const std::map<std::string, Graph::Vertex>& name_to_id = graph.Map();
            // 判断两个点是否存在
            if (name_to_id.count(vertex_name_x) == 0 || name_to_id.count(vertex_name_y) == 0) { //count是统计出现的次数
                std::cerr << "输入的顶点名不存在！" << std::endl;
            }
            // 获取编号
            Vertex start = name_to_id.at(vertex_name_x);
            Vertex end = name_to_id.at(vertex_name_y);

            std::vector<bool> collectedist(n, false);   // 是否已收录到最短路径中
            std::vector<Vertex> dist(n, INT_MAX);        // 最短距离数组，初始化为无穷大
            std::vector<Vertex> path(n, Graph::NIL);            // 前驱数组，初始化为NIL 
            dist[start]=0;  //起点到自己的距离是0
            for (const EdgeNode &edge :graph.List()[start].adj){
                dist[edge.dest]=edge.weight;
                path[edge.dest]=start;
            }
            collectedist[start]=true;
            for (size_t i=0;i<n;++i){
                // 找当前未被收录的、dist最小的顶点
                int mindist = INT_MAX;
                Vertex u =NIL;
                for (Vertex v = 0; v < n; ++v) {
                    if (!collectedist[v] && dist[v] < mindist) {
                        mindist = dist[v];
                        u = v;
                    }
                }
                if (u == Graph::NIL) break; // 全部收录或找不到可达顶点
                collectedist[u] = true;
                // 更新u邻接点的dist
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
                result.path_string = "不可达";
                return result;
            }
            // 回溯路径
            std::vector<Vertex> result_path;
            for (Vertex v = end; v != Graph::NIL; v = path[v]) {
                result_path.push_back(v);
            }
            std::reverse(result_path.begin(), result_path.end());

            // 映射 ID -> 名称
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
            //TODO:获取拓扑受限的最短路径，拓扑序由path给出
            const int INF = 1e9;
            std::unordered_map<std::string, int> dist;
            std::unordered_map<std::string, std::string> prev;
            // 初始化所有点为 INF
            for (const auto& name : path) {
                dist[name] = INF;
            }
            // 起点距离为 0
            dist[path[0]] = 0;
            // 按照拓扑序列处理每个点
            for (const auto& u_name : path) {
                if (!graph.exist_vertex(u_name)) continue;
                if (graph.Map().count(u_name) == 0) continue; // 如果图中没有这个名字，跳过
                Vertex u = graph.Map().at(u_name);
                for (const auto& edge : graph.List()[u].adj) {
                    std::string v_name = graph.GetVertex(edge.dest).name;
                    // 只考虑拓扑序列里的点
                    if (dist.find(v_name) != dist.end()) {
                        int new_dist = dist[u_name] + edge.weight; 
                        if (new_dist < dist[v_name]) {
                            dist[v_name] = new_dist;
                            prev[v_name] = u_name;  // 记录路径
                        }
                    }
                }
            }
            std::string end = path.back();
            if (dist[end] == INF) {
                std::cout << "无可行路径" << std::endl;
                return -1;
            }
            // 追踪路径
            std::vector<std::string> route;
            std::string cur = end;
            while (cur != path[0]) {
                route.push_back(cur);
                cur = prev[cur];
            }
            route.push_back(path[0]);
            std::reverse(route.begin(), route.end());
            // 输出路径
            std::cout << "最短路径为：";
            for (size_t i = 0; i < route.size(); ++i) {
                std::cout << route[i];
                if (i + 1 != route.size()) std::cout << " -> ";
            }
            std::cout << std::endl;

            return dist[end];
        }

        

        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph) {
            //TODO:计算最小生成树，并返回树上的边
            std::vector<EdgeNode> res;
            std::vector<EdgeNode> edges = graph.SortedEdges();  //按权值排序
            size_t n= graph.VertexCount();
            Algorithm:: DSU dsu(n);     //创建并查集

            for (const EdgeNode &edge : edges){
                if( !dsu.same(edge.from,edge.dest)){    //如果不在同一个集合
                    dsu.unite(edge.from,edge.dest);     //合并
                    res.push_back(edge);
                    if(res.size() == n-1){  //最小生成树边数为(n-1)
                        break;
                    }
                }
            }
            return res;
        }

        PlanVisitResult PlanTeachingVisit(const LGraph& graph, const std::string& start_gate) {
            const auto& name_to_id = graph.Map();
            if (name_to_id.count(start_gate) == 0) {
                throw std::invalid_argument("起点校门不存在！");
            }

            Vertex start = name_to_id.at(start_gate);
            std::vector<Vertex> teaching_buildings;

            // 找出所有教学楼
            for (Vertex v = 0; v < graph.VertexCount(); ++v) {
                Graph::LocationInfo info = graph.GetVertex(v);
                if (info.type == "Teaching_Research_and_Administration") {
                    teaching_buildings.push_back(v);
                }
            }

            // 构建 id 到 name 映射
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
                    throw std::runtime_error("部分教学楼不可达！");
                }

                // 路程时间
                total_time_seconds += best_path.distance;

                // 参观时间
                total_time_seconds += graph.GetVertex(next).visitTime * 60;

                visit_order.push_back(next);
                current = next;
                unvisited.erase(next);
            }

            // 构建路径字符串
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

            // 初始化随机种子
            std::srand(static_cast<unsigned int>(std::time(nullptr)));

            // 连通分量分析
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
            
            // 如果图已连通，不需要添加边
            if (components.size() <= 1) {
                return suggestions;
            }
            
            // 构建 ID 到名称的映射
            std::map<Vertex, std::string> id_to_name;
            const auto& name_to_id = graph.Map();
            for (const auto& pair : name_to_id) {
                id_to_name[pair.second] = pair.first;
            }
            
            // 为每个连通分量之间添加一条边
            for (size_t i = 1; i < components.size(); i++) {
                // 从前一个分量随机选一个顶点
                Vertex from_idx = components[i-1][std::rand() % components[i-1].size()];
                std::string from = id_to_name[from_idx];
                
                // 从当前分量随机选一个顶点
                Vertex to_idx = components[i][std::rand() % components[i].size()];
                std::string to = id_to_name[to_idx];
                
                // 随机权重：50-150米
                int weight = 50 + std::rand() % 101;
                
                suggestions.push_back({from, to, weight});
            }
            
            return suggestions;
        }
        
    }
}