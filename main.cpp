#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include "LGraph.h"
#include "Algorithm.h"
#include "Algorithm.cpp"
#include "LocationInfo.h"
#include "GraphException.h"


using Graph::LGraph;
using Graph::VertInfo;
using namespace Graph::Algorithm;

// Hint: 如果不慎写坏项目附带的两个文件，可以在backup文件夹中获取原始文件
// TODO: 请根据可执行文件和csv的位置填写文件路径
const std::string nodes_path{ "nodes.csv" };
const std::string edges_path{ "edges.csv" };

struct edge {
    std::string from;
    std::string to;
    int length;
};

void init(LGraph& graph);

std::vector<VertInfo> read_nodes(const std::string& nodes_path);

std::vector<edge> read_edges(const std::string& edges_path);

void store_nodes(const std::string& nodes_path, const LGraph& graph);

void store_edges(const std::string& edges_path, const LGraph& graph);

void show_all_nodes(const LGraph& graph);

void show_all_edges(const LGraph& graph);

int main() {
    LGraph graph(false); //初始化一个无向图。
    init(graph);
    while (true) {
        int choice{ 0 };
        std::cout << "欢迎使用校园导航系统！" << std::endl
            << "请选择您要进行的操作：" << std::endl
            << "1.顶点相关操作" << std::endl
            << "2.边相关操作" << std::endl
            << "3.从文件中重新加载点与边" << std::endl
            << "4.判定图是否连通并建议添加道路" << std::endl 
            << "5.是否存在欧拉通路" << std::endl
            << "6.求任意两点间的最短距离" << std::endl
            << "7.求最小生成树" << std::endl
            << "8.求解拓扑受限时的最短路径" << std::endl
            << "9.从校门出发参观所有教学楼的路径规划" << std::endl
            << "10.退出程序" << std::endl
            << "请输入操作前的数字：";
        std::cin >> choice;
        if (choice == 1) { // 顶点相关操作
            std::cout << "顶点相关操作：" << std::endl
                << "1.输出特定顶点信息" << std::endl
                << "2.输出所有顶点信息" << std::endl
                << "3.添加一个顶点" << std::endl
                << "4.删除一个顶点" << std::endl
                << "5.存储顶点到文件中" << std::endl
                << "6.修改地点参观时间" << std::endl  // 新增功能
                << "7.按照类别查找地点" << std::endl
                << "8.回到上一级菜单" << std::endl
                << "请输入操作前的数字：";
            std::cin >> choice;
            if (choice == 1) {
                std::string name;
                std::cout << "请输入顶点名称：";
                std::cin >> name;
                try {
                    VertInfo v{ graph.GetVertex(name) };
                    std::cout << "顶点名称：" << v.name << std::endl
                        << "顶点类型：" << v.type << std::endl
                        << "建议游览时间：" << v.visitTime << std::endl;

                }
                catch (Graph::GraphException& e) {
                    std::cout << "名称为 " << name << " 的顶点不存在" << std::endl;
                }
            }
            else if (choice == 2) {
                std::cout << "顶点名称, 顶点类型, 建议游览时间：" << std::endl;
                show_all_nodes(graph);
            }
            else if (choice == 3) {
                std::cout << "请输入顶点名称，顶点类型，建议游览时间(空格分隔)：";
                std::string name, type;
                int visit_time;
                std::cin >> name >> type >> visit_time;
                try {
                    graph.InsertVertex({ name, visit_time, type });
                }
                catch (Graph::GraphException& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (choice == 4) {
                std::cout << "请输入顶点名称：";
                std::string name;
                std::cin >> name;
                try {
                    graph.DeleteVertex({ name, 0, "" });
                }
                catch (Graph::GraphException& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (choice == 5) {
                std::cout << "正在将顶点存储到test_nodes.csv中..." << std::endl;
                store_nodes("test_nodes.csv", graph);
                std::cout << "存储成功！" << std::endl;
            }
            else if (choice == 6) { // 修改地点参观时间
            std::cout << "请输入要修改的地点名称：";
            std::string name;
            std::cin >> name;
            
            try {
                VertInfo v = graph.GetVertex(name);
                std::cout << "当前地点: " << v.name << ", 当前参观时间: " << v.visitTime << " 分钟" << std::endl;
                
                int new_time;
                std::cout << "请输入新的参观时间(分钟)：";
                std::cin >> new_time;
                
                // 创建新的顶点信息
                VertInfo new_info = v;
                new_info.visitTime = new_time;
                
                // 更新顶点信息
                graph.UpdateVertex(v, new_info);
                std::cout << "参观时间已更新为 " << new_time << " 分钟！" << std::endl;
            }
            catch (Graph::GraphException& e) {
                std::cout << "错误: " << e.what() << std::endl;
                }
            }
            else if (choice == 7) { // 按照类别查找地点
            std::cout << "请输入要查找的类别（如 Teaching_Research_and_Administration, dormitory 等）：";
            std::string category;
            std::cin >> category;
            
            std::cout << "类别为 \"" << category << "\" 的地点：" << std::endl;
            bool found = false;
            
            // 遍历所有顶点
            for (Graph::Vertex v = 0; v < graph.VertexCount(); v++) {
                VertInfo info = graph.GetVertex(v);
                if (info.type == category) {
                    std::cout << "名称: " << info.name 
                              << ", 参观时间: " << info.visitTime << " 分钟" 
                              << std::endl;
                    found = true;
                }
            }
            
            if (!found) {
                std::cout << "未找到类别为 \"" << category << "\" 的地点。" << std::endl;
                }
            }
        }
        else if (choice == 2) { // 边相关操作
            std::cout << "边相关操作：" << std::endl
                << "1.输出特定边信息" << std::endl
                << "2.输出所有边信息" << std::endl
                << "3.添加一条边" << std::endl
                << "4.删除一条边" << std::endl
                << "5.存储边到文件中" << std::endl
                << "6.修改边权重" << std::endl
                << "7.按地点查找相关道路" <<std::endl
                << "8.回到上一级菜单" << std::endl
                << "请输入操作前的数字：";
            std::cin >> choice;
            if (choice == 1) {
                std::string from, to;
                std::cout << "请输入边的起点与终点(空格分隔)：";
                std::cin >> from >> to;
                try {
                    Graph::GElemSet w{ graph.GetEdge(from, to) };
                    std::cout << from << " <---> " << to << " 距离为：" << w << std::endl;
                }
                catch (Graph::GraphException& e) {
                    std::cout << from << " <---> " << to << " 不存在！" << std::endl;
                }
            }
            else if (choice == 2) {
                std::cout << "起点, 终点, 距离：" << std::endl;
                show_all_edges(graph);
            }
            else if (choice == 3) {
                std::cout << "请输入起点名称，终点名称，距离(空格分隔)：";
                std::string from, to;
                int length;
                std::cin >> from >> to >> length;
                graph.InsertEdge(from, to, length);
            }
            else if (choice == 4) {
                std::cout << "请输入起点名称，终点名称(空格分隔)：";
                std::string from, to;
                std::cin >> from >> to;
                graph.DeleteEdge(from, to); // 删除一条边
            }
            else if (choice == 5) {
                std::cout << "正在将边存储到test_edges.csv文件中..." << std::endl;
                store_edges("test_edges.csv", graph);
                std::cout << "存储成功！" << std::endl;
            }
            else if (choice == 6) { // 修改边权重
            std::cout << "请输入要修改的边的起点和终点（空格分隔）：";
            std::string from, to;
            std::cin >> from >> to;
            
            try {
                // 获取当前权重
                int current_weight = graph.GetEdge(from, to);
                std::cout << "当前权重: " << from << " <-> " << to << " = " << current_weight << "米" << std::endl;
                
                // 输入新权重
                int new_weight;
                std::cout << "请输入新的权重（米）：";
                std::cin >> new_weight;
                
                // 更新边权重
                graph.UpdateEdge(from, to, new_weight);
                std::cout << "权重已更新为 " << new_weight << "米！" << std::endl;
            }
            catch (Graph::GraphException& e) {
                std::cout << "错误: " << e.what() << std::endl;
                }
            }

            else if (choice == 7) { // 按地点查找相关道路
            std::cout << "请输入地点名称：";
            std::string location;
            std::cin >> location;
            
            try {
                // 获取顶点ID
                Graph::Vertex v = graph.Map().at(location);
                const Graph::HeadNode& node = graph.List()[v];
                
                // 构建ID到名称的映射
                std::map<Graph::Vertex, std::string> id_to_name;
                const auto& name_to_id = graph.Map();
                for (const auto& pair : name_to_id) {
                    id_to_name[pair.second] = pair.first;
                }
                
                std::cout << "与 " << location << " 相连的道路：" << std::endl;
                
                if (node.adj.empty()) {
                    std::cout << "  该地点没有相连的道路。" << std::endl;
                } else {
                    for (const auto& edge : node.adj) {
                        std::string neighbor = id_to_name[edge.dest];
                        std::cout << "  " << location << " <-> " << neighbor 
                                  << " (距离: " << edge.weight << "米)" << std::endl;
                    }
                }
            }
            catch (const std::out_of_range&) {
                std::cout << "地点 '" << location << "' 不存在！" << std::endl;
            }
            catch (const Graph::GraphException& e) {
                std::cout << "错误: " << e.what() << std::endl;
            }
        }
        }
        else if (choice == 3) {// 从文件中重新加载点与边
            init(graph);
        }
        else if (choice == 4) { 
            if (IsConnected(graph)) {
                std::cout << "图是连通的，不需要添加道路。" << std::endl;
            } else {
                std::cout << "图不是连通的，建议添加以下道路：" << std::endl;
                auto suggestions = Graph::Algorithm::GetConnectionSuggestions(graph);
                
                for (const auto& edge : suggestions) {
                    std::cout << "添加道路: " << edge.from << " <-> " << edge.to 
                            << " (长度: " << edge.weight << "米)" << std::endl;
                }
                
                std::cout << "是否要添加这些道路？(y/n): ";
                char choice;
                std::cin >> choice;
                
                if (choice == 'y' || choice == 'Y') {
                    for (const auto& edge : suggestions) {
                        graph.InsertEdge(edge.from, edge.to, edge.weight);
                    }
                    std::cout << "已添加 " << suggestions.size() << " 条道路。" << std::endl;
                    
                    // 验证连通性
                    if (IsConnected(graph)) {
                        std::cout << "现在图已连通！" << std::endl;
                    } else {
                        std::cout << "添加后图仍未连通，请检查。" << std::endl;
                    }
                }
            }
        }

        else if (choice == 5) { // 是否存在欧拉通路
            auto res = HaveEulerCircle(graph);
            std::cout << (res ? "存在欧拉回路" : "不存在欧拉回路") << std::endl;
        }
        else if (choice == 6) { // 求任意两点间的最短距离
            std::cout << "请输入两个地点，使用空格分开：" << std::endl;
            std::string x, y;
            std::cin >> x >> y;
            try {
                ShortestPathResult result = GetShortestPath(graph, x, y);
                std::cout << x << "和" << y << "之间的最短距离为：" << result.distance << std::endl;
                std::cout << "最短路径为：" << result.path_string << std::endl;

            }
            catch (Graph::GraphException& e) {
                std::cout << "你找到了虚空的距离" << std::endl;
            }
        }
        else if (choice == 7) { // 求最小生成树
            if (IsConnected(graph)) {
                std::cout << "最小生成树的点如下：";
                auto res = MinimumSpanningTree(graph);
                int sum = 0;
                for (auto item : res) {
                    const auto& e = edge{ graph.List()[item.from].data.name, graph.List()[item.dest].data.name, item.weight };
                    std::cout << e.from << "," << e.to << "," << e.length << std::endl;
                    sum += e.length;
                }
                std::cout << "总权重为" << sum << std::endl;
            }
            else {
                std::cout << "图不连通" << std::endl;
            }
        }
        else if (choice == 8) { // 求解拓扑受限时的最短路径
            std::cout << "请输入您希望的拓扑序，第一行一个n为序列长度，第二行n个地点为拓扑序列" << std::endl;
            int n;
            std::vector<std::string> list;
            std::cin >> n;
            while (n--) {
                std::string x;
                std::cin >> x;
                list.push_back(x);
            }
            std::cout << "最短路径为" << TopologicalShortestPath(graph, list) << std::endl;
        }
        
        else if (choice == 9) { // 参观所有教学楼的路径规划
            std::vector<std::string> gates = {
                    "Gate_788_Zaoyang_Road",
                    "Gate_460_Zaoyang_Road",
                    "Jinshajiang_Road_Gate"
                };

                while (true) {
                    std::cout << "请选择起始校门（输入编号，0返回主菜单）：" << std::endl;
                    for (size_t i = 0; i < gates.size(); ++i) {
                        std::cout << i + 1 << ". " << gates[i] << std::endl;
                    }
                    std::cout << "0. 返回主菜单" << std::endl;

                    int gate_choice;
                    std::cin >> gate_choice;
                    
                    if (gate_choice == 0) {
                        break; // 返回主菜单
                    }
                    
                    if (gate_choice < 1 || gate_choice > gates.size()) {
                        std::cout << "输入错误，请重新选择。" << std::endl;
                        continue;
                    }

                std::string start_gate = gates[gate_choice - 1];
                try {
                    auto result = PlanTeachingVisit(graph, start_gate); // 你需要实现这个函数
                    std::cout << "参观路径为：" << result.path_string << std::endl;
                    std::cout << "总花费时间为：" << result.total_time_minutes << "分" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "路径规划失败：" << e.what() << std::endl;
                }
            }
        }



        else {
            std::cout << "感谢您的使用，再见！\n 开发者：\\lky1433223/ \\Voltline/" << std::endl;
            break;
        }
    }
    return 0;
}

/* 从文件中读取顶点信息
 * @param nodes_path: 顶点文件路径
 * @return: 包含读取到的所有顶点的std::vector
 */
std::vector<VertInfo> read_nodes(const std::string& nodes_path) {
    std::ifstream fin(nodes_path);              // 创建文件输入流对象
    assert(fin.good());                         // 断言，通过文件输入流的good()方法检查文件是否存在
    std::vector<VertInfo> nodes;                // 临时存储顶点信息
    std::string line;                           // 通过std::getline读取到的每一行，临时存储在字符串line中
    while (std::getline(fin, line)) {       // 通过std::getline(std::istream&, std::string&)来获取输入流的一行
        for (auto& c : line) {
            if (c == ',') c = ' ';              // csv文件以逗号分隔，为了方便后续使用字符串输入流，将逗号替换为空格
        }
        std::istringstream sin(line);           // 利用std::istringstream，以获取的行为内容构建一个字符串输入流
        std::string name, type;                 // 文件每行中的顶点名，顶点类型
        int visit_time;                         // 文件中每行的顶点推荐游览时间
        sin >> name >> type >> visit_time;      // 利用已构造的字符串输入流，获取对应的几个参数
        nodes.push_back({ name, visit_time, type });  // 放入需要返回的std::vector中
    }
    fin.close();                                // 文件使用完毕后，请务必使用close访问关闭输入流，避免文件损坏
    return nodes;
}

/* 从文件中读取边信息
 * @param edges_path: 边文件路径
 * @return: 包含读取到的所有边的std::vector
 */
std::vector<edge> read_edges(const std::string& edges_path) {
    std::ifstream fin(edges_path);              // 创建文件输入流对象
    assert(fin.good());                         // 检查文件是否存在
    std::vector<edge> edges;                    // 存储边信息的容器
    std::string line;

    while (std::getline(fin, line)) {           // 逐行读取文件内容
        for (auto& c : line) {                  // 替换逗号为空格以便解析
            if (c == ',') c = ' ';
        }
        std::istringstream sin(line);           // 使用字符串流解析行内容
        std::string from, to;
        int length;
        sin >> from >> to >> length;            // 提取起点、终点和长度
        edges.push_back({from, to, length});   // 将边信息添加到容器
    }

    fin.close();                                // 关闭文件流
    return edges;
}

void init(LGraph& graph) {
    std::vector<VertInfo> nodes{ read_nodes(nodes_path) };        // 通过read_nodes函数获得顶点信息
    std::vector<edge> edges{ read_edges(edges_path) };            // 通过read_edges函数获得边信息
    for (auto& v : nodes) {
        // 直接使用 VertInfo（即 LocationInfo）插入顶点
        graph.InsertVertex(v);
    }

    for (auto& e : edges) {
        // 调用 InsertEdge 方法，参数为起点名、终点名、长度
        graph.InsertEdge(e.from, e.to, e.length);
    }
}

/* 打印目前的所有顶点
 * @param graph: 图
 */
void show_all_nodes(const LGraph& graph) {
    // TODO: 实现打印所有顶点信息
    // TODO: 每行输出格式：顶点名,顶点类型,推荐游览时间
    const auto& vertex_map = graph.Map();  // 获取顶点名到ID的映射
    for (const auto& pair : vertex_map) {
        const std::string& name = pair.first;          // 顶点名
        const VertInfo& info = graph.GetVertex(name);  // 获取顶点详细信息
        // 输出格式：顶点名,顶点类型,推荐游览时间
        std::cout << info.name << "," << info.type << "," << info.visitTime << std::endl;
    }
}

/* 打印目前的所有边
 * @param graph: 图
 */
 
void show_all_edges(const LGraph& graph) {
    // LGraph类只提供了获取邻接表和获取经过排序的边的方法
    // 为了能够获取到每条边的元信息，我们需要配合两组数据来完成信息的获取
    const auto& graph_edges{ graph.SortedEdges() };       // 获取经过排序的边
    const auto& graph_list{ graph.List() };               // 获取邻接表
    for (const auto& en : graph_edges) {                 // 遍历边，通过下标来获取对应的顶点信息
        const auto& e = edge{ graph_list[en.from].data.name, graph_list[en.dest].data.name, en.weight };
        std::cout << e.from << "," << e.to << "," << e.length << std::endl;
    }
}



/* 将顶点信息存储回文件中
 * @param nodes_path: 顶点文件路径
 * @param graph: 图
 */
void store_nodes(const std::string& nodes_path, const LGraph& graph) {
    std::ofstream fout(nodes_path);                                         // 通过目录创建文件输出流
    const auto& graph_list{ graph.List() };                                   // 通过LGraph对象的List()方法获得邻接表
    for (const auto& n : graph_list) {                                       // 遍历邻接表中的每个顶点
        const auto& v = n.data;                                             // 获得顶点中的元数据
        fout << v.name << "," << v.type << "," << v.visitTime << std::endl; // 按照顶点存储的格式，重新输入到文件中
    }
    fout.close();                                                           // 在使用完任何文件流后，都要使用close方法关闭
}

/* 将边信息存储回文件中
 * @param edges_path: 边文件路径
 * @param graph: 图
 */
void store_edges(const std::string& edges_path, const LGraph& graph) {
    // TODO: 请参考store_nodes函数的内容与提示，结合edges.csv文件的内容
    // TODO: 以及show_all_edges函数中获取边的元信息的方式完成store_edges函数的定义
    
    std::ofstream fout(edges_path);                     // 创建文件输出流
    const auto& graph_edges{ graph.SortedEdges() };      // 获取排序后的边列表（已去重）
    const auto& graph_list{ graph.List() };              // 获取邻接表以解析顶点名称

    for (const auto& en : graph_edges) {                 // 遍历每条边
        const std::string& from = graph_list[en.from].data.name;  // 起点名称
        const std::string& to = graph_list[en.dest].data.name;    // 终点名称
        fout << from << "," << to << "," << en.weight << std::endl; // 写入CSV格式
    }

    fout.close();                                        // 关闭文件流
}
