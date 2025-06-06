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

// Hint: �������д����Ŀ�����������ļ���������backup�ļ����л�ȡԭʼ�ļ�
// TODO: ����ݿ�ִ���ļ���csv��λ����д�ļ�·��
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
    LGraph graph(false); //��ʼ��һ������ͼ��
    init(graph);
    while (true) {
        int choice{ 0 };
        std::cout << "��ӭʹ��У԰����ϵͳ��" << std::endl
            << "��ѡ����Ҫ���еĲ�����" << std::endl
            << "1.������ز���" << std::endl
            << "2.����ز���" << std::endl
            << "3.���ļ������¼��ص����" << std::endl
            << "4.�ж�ͼ�Ƿ���ͨ��������ӵ�·" << std::endl 
            << "5.�Ƿ����ŷ��ͨ·" << std::endl
            << "6.��������������̾���" << std::endl
            << "7.����С������" << std::endl
            << "8.�����������ʱ�����·��" << std::endl
            << "9.��У�ų����ι����н�ѧ¥��·���滮" << std::endl
            << "10.�˳�����" << std::endl
            << "���������ǰ�����֣�";
        std::cin >> choice;
        if (choice == 1) { // ������ز���
            std::cout << "������ز�����" << std::endl
                << "1.����ض�������Ϣ" << std::endl
                << "2.������ж�����Ϣ" << std::endl
                << "3.���һ������" << std::endl
                << "4.ɾ��һ������" << std::endl
                << "5.�洢���㵽�ļ���" << std::endl
                << "6.�޸ĵص�ι�ʱ��" << std::endl  // ��������
                << "7.���������ҵص�" << std::endl
                << "8.�ص���һ���˵�" << std::endl
                << "���������ǰ�����֣�";
            std::cin >> choice;
            if (choice == 1) {
                std::string name;
                std::cout << "�����붥�����ƣ�";
                std::cin >> name;
                try {
                    VertInfo v{ graph.GetVertex(name) };
                    std::cout << "�������ƣ�" << v.name << std::endl
                        << "�������ͣ�" << v.type << std::endl
                        << "��������ʱ�䣺" << v.visitTime << std::endl;

                }
                catch (Graph::GraphException& e) {
                    std::cout << "����Ϊ " << name << " �Ķ��㲻����" << std::endl;
                }
            }
            else if (choice == 2) {
                std::cout << "��������, ��������, ��������ʱ�䣺" << std::endl;
                show_all_nodes(graph);
            }
            else if (choice == 3) {
                std::cout << "�����붥�����ƣ��������ͣ���������ʱ��(�ո�ָ�)��";
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
                std::cout << "�����붥�����ƣ�";
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
                std::cout << "���ڽ�����洢��test_nodes.csv��..." << std::endl;
                store_nodes("test_nodes.csv", graph);
                std::cout << "�洢�ɹ���" << std::endl;
            }
            else if (choice == 6) { // �޸ĵص�ι�ʱ��
            std::cout << "������Ҫ�޸ĵĵص����ƣ�";
            std::string name;
            std::cin >> name;
            
            try {
                VertInfo v = graph.GetVertex(name);
                std::cout << "��ǰ�ص�: " << v.name << ", ��ǰ�ι�ʱ��: " << v.visitTime << " ����" << std::endl;
                
                int new_time;
                std::cout << "�������µĲι�ʱ��(����)��";
                std::cin >> new_time;
                
                // �����µĶ�����Ϣ
                VertInfo new_info = v;
                new_info.visitTime = new_time;
                
                // ���¶�����Ϣ
                graph.UpdateVertex(v, new_info);
                std::cout << "�ι�ʱ���Ѹ���Ϊ " << new_time << " ���ӣ�" << std::endl;
            }
            catch (Graph::GraphException& e) {
                std::cout << "����: " << e.what() << std::endl;
                }
            }
            else if (choice == 7) { // ���������ҵص�
            std::cout << "������Ҫ���ҵ������ Teaching_Research_and_Administration, dormitory �ȣ���";
            std::string category;
            std::cin >> category;
            
            std::cout << "���Ϊ \"" << category << "\" �ĵص㣺" << std::endl;
            bool found = false;
            
            // �������ж���
            for (Graph::Vertex v = 0; v < graph.VertexCount(); v++) {
                VertInfo info = graph.GetVertex(v);
                if (info.type == category) {
                    std::cout << "����: " << info.name 
                              << ", �ι�ʱ��: " << info.visitTime << " ����" 
                              << std::endl;
                    found = true;
                }
            }
            
            if (!found) {
                std::cout << "δ�ҵ����Ϊ \"" << category << "\" �ĵص㡣" << std::endl;
                }
            }
        }
        else if (choice == 2) { // ����ز���
            std::cout << "����ز�����" << std::endl
                << "1.����ض�����Ϣ" << std::endl
                << "2.������б���Ϣ" << std::endl
                << "3.���һ����" << std::endl
                << "4.ɾ��һ����" << std::endl
                << "5.�洢�ߵ��ļ���" << std::endl
                << "6.�޸ı�Ȩ��" << std::endl
                << "7.���ص������ص�·" <<std::endl
                << "8.�ص���һ���˵�" << std::endl
                << "���������ǰ�����֣�";
            std::cin >> choice;
            if (choice == 1) {
                std::string from, to;
                std::cout << "������ߵ�������յ�(�ո�ָ�)��";
                std::cin >> from >> to;
                try {
                    Graph::GElemSet w{ graph.GetEdge(from, to) };
                    std::cout << from << " <---> " << to << " ����Ϊ��" << w << std::endl;
                }
                catch (Graph::GraphException& e) {
                    std::cout << from << " <---> " << to << " �����ڣ�" << std::endl;
                }
            }
            else if (choice == 2) {
                std::cout << "���, �յ�, ���룺" << std::endl;
                show_all_edges(graph);
            }
            else if (choice == 3) {
                std::cout << "������������ƣ��յ����ƣ�����(�ո�ָ�)��";
                std::string from, to;
                int length;
                std::cin >> from >> to >> length;
                graph.InsertEdge(from, to, length);
            }
            else if (choice == 4) {
                std::cout << "������������ƣ��յ�����(�ո�ָ�)��";
                std::string from, to;
                std::cin >> from >> to;
                graph.DeleteEdge(from, to); // ɾ��һ����
            }
            else if (choice == 5) {
                std::cout << "���ڽ��ߴ洢��test_edges.csv�ļ���..." << std::endl;
                store_edges("test_edges.csv", graph);
                std::cout << "�洢�ɹ���" << std::endl;
            }
            else if (choice == 6) { // �޸ı�Ȩ��
            std::cout << "������Ҫ�޸ĵıߵ������յ㣨�ո�ָ�����";
            std::string from, to;
            std::cin >> from >> to;
            
            try {
                // ��ȡ��ǰȨ��
                int current_weight = graph.GetEdge(from, to);
                std::cout << "��ǰȨ��: " << from << " <-> " << to << " = " << current_weight << "��" << std::endl;
                
                // ������Ȩ��
                int new_weight;
                std::cout << "�������µ�Ȩ�أ��ף���";
                std::cin >> new_weight;
                
                // ���±�Ȩ��
                graph.UpdateEdge(from, to, new_weight);
                std::cout << "Ȩ���Ѹ���Ϊ " << new_weight << "�ף�" << std::endl;
            }
            catch (Graph::GraphException& e) {
                std::cout << "����: " << e.what() << std::endl;
                }
            }

            else if (choice == 7) { // ���ص������ص�·
            std::cout << "������ص����ƣ�";
            std::string location;
            std::cin >> location;
            
            try {
                // ��ȡ����ID
                Graph::Vertex v = graph.Map().at(location);
                const Graph::HeadNode& node = graph.List()[v];
                
                // ����ID�����Ƶ�ӳ��
                std::map<Graph::Vertex, std::string> id_to_name;
                const auto& name_to_id = graph.Map();
                for (const auto& pair : name_to_id) {
                    id_to_name[pair.second] = pair.first;
                }
                
                std::cout << "�� " << location << " �����ĵ�·��" << std::endl;
                
                if (node.adj.empty()) {
                    std::cout << "  �õص�û�������ĵ�·��" << std::endl;
                } else {
                    for (const auto& edge : node.adj) {
                        std::string neighbor = id_to_name[edge.dest];
                        std::cout << "  " << location << " <-> " << neighbor 
                                  << " (����: " << edge.weight << "��)" << std::endl;
                    }
                }
            }
            catch (const std::out_of_range&) {
                std::cout << "�ص� '" << location << "' �����ڣ�" << std::endl;
            }
            catch (const Graph::GraphException& e) {
                std::cout << "����: " << e.what() << std::endl;
            }
        }
        }
        else if (choice == 3) {// ���ļ������¼��ص����
            init(graph);
        }
        else if (choice == 4) { 
            if (IsConnected(graph)) {
                std::cout << "ͼ����ͨ�ģ�����Ҫ��ӵ�·��" << std::endl;
            } else {
                std::cout << "ͼ������ͨ�ģ�����������µ�·��" << std::endl;
                auto suggestions = Graph::Algorithm::GetConnectionSuggestions(graph);
                
                for (const auto& edge : suggestions) {
                    std::cout << "��ӵ�·: " << edge.from << " <-> " << edge.to 
                            << " (����: " << edge.weight << "��)" << std::endl;
                }
                
                std::cout << "�Ƿ�Ҫ�����Щ��·��(y/n): ";
                char choice;
                std::cin >> choice;
                
                if (choice == 'y' || choice == 'Y') {
                    for (const auto& edge : suggestions) {
                        graph.InsertEdge(edge.from, edge.to, edge.weight);
                    }
                    std::cout << "����� " << suggestions.size() << " ����·��" << std::endl;
                    
                    // ��֤��ͨ��
                    if (IsConnected(graph)) {
                        std::cout << "����ͼ����ͨ��" << std::endl;
                    } else {
                        std::cout << "��Ӻ�ͼ��δ��ͨ�����顣" << std::endl;
                    }
                }
            }
        }

        else if (choice == 5) { // �Ƿ����ŷ��ͨ·
            auto res = HaveEulerCircle(graph);
            std::cout << (res ? "����ŷ����·" : "������ŷ����·") << std::endl;
        }
        else if (choice == 6) { // ��������������̾���
            std::cout << "�����������ص㣬ʹ�ÿո�ֿ���" << std::endl;
            std::string x, y;
            std::cin >> x >> y;
            try {
                ShortestPathResult result = GetShortestPath(graph, x, y);
                std::cout << x << "��" << y << "֮�����̾���Ϊ��" << result.distance << std::endl;
                std::cout << "���·��Ϊ��" << result.path_string << std::endl;

            }
            catch (Graph::GraphException& e) {
                std::cout << "���ҵ�����յľ���" << std::endl;
            }
        }
        else if (choice == 7) { // ����С������
            if (IsConnected(graph)) {
                std::cout << "��С�������ĵ����£�";
                auto res = MinimumSpanningTree(graph);
                int sum = 0;
                for (auto item : res) {
                    const auto& e = edge{ graph.List()[item.from].data.name, graph.List()[item.dest].data.name, item.weight };
                    std::cout << e.from << "," << e.to << "," << e.length << std::endl;
                    sum += e.length;
                }
                std::cout << "��Ȩ��Ϊ" << sum << std::endl;
            }
            else {
                std::cout << "ͼ����ͨ" << std::endl;
            }
        }
        else if (choice == 8) { // �����������ʱ�����·��
            std::cout << "��������ϣ���������򣬵�һ��һ��nΪ���г��ȣ��ڶ���n���ص�Ϊ��������" << std::endl;
            int n;
            std::vector<std::string> list;
            std::cin >> n;
            while (n--) {
                std::string x;
                std::cin >> x;
                list.push_back(x);
            }
            std::cout << "���·��Ϊ" << TopologicalShortestPath(graph, list) << std::endl;
        }
        
        else if (choice == 9) { // �ι����н�ѧ¥��·���滮
            std::vector<std::string> gates = {
                    "Gate_788_Zaoyang_Road",
                    "Gate_460_Zaoyang_Road",
                    "Jinshajiang_Road_Gate"
                };

                while (true) {
                    std::cout << "��ѡ����ʼУ�ţ������ţ�0�������˵�����" << std::endl;
                    for (size_t i = 0; i < gates.size(); ++i) {
                        std::cout << i + 1 << ". " << gates[i] << std::endl;
                    }
                    std::cout << "0. �������˵�" << std::endl;

                    int gate_choice;
                    std::cin >> gate_choice;
                    
                    if (gate_choice == 0) {
                        break; // �������˵�
                    }
                    
                    if (gate_choice < 1 || gate_choice > gates.size()) {
                        std::cout << "�������������ѡ��" << std::endl;
                        continue;
                    }

                std::string start_gate = gates[gate_choice - 1];
                try {
                    auto result = PlanTeachingVisit(graph, start_gate); // ����Ҫʵ���������
                    std::cout << "�ι�·��Ϊ��" << result.path_string << std::endl;
                    std::cout << "�ܻ���ʱ��Ϊ��" << result.total_time_minutes << "��" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "·���滮ʧ�ܣ�" << e.what() << std::endl;
                }
            }
        }



        else {
            std::cout << "��л����ʹ�ã��ټ���\n �����ߣ�\\lky1433223/ \\Voltline/" << std::endl;
            break;
        }
    }
    return 0;
}

/* ���ļ��ж�ȡ������Ϣ
 * @param nodes_path: �����ļ�·��
 * @return: ������ȡ�������ж����std::vector
 */
std::vector<VertInfo> read_nodes(const std::string& nodes_path) {
    std::ifstream fin(nodes_path);              // �����ļ�����������
    assert(fin.good());                         // ���ԣ�ͨ���ļ���������good()��������ļ��Ƿ����
    std::vector<VertInfo> nodes;                // ��ʱ�洢������Ϣ
    std::string line;                           // ͨ��std::getline��ȡ����ÿһ�У���ʱ�洢���ַ���line��
    while (std::getline(fin, line)) {       // ͨ��std::getline(std::istream&, std::string&)����ȡ��������һ��
        for (auto& c : line) {
            if (c == ',') c = ' ';              // csv�ļ��Զ��ŷָ���Ϊ�˷������ʹ���ַ������������������滻Ϊ�ո�
        }
        std::istringstream sin(line);           // ����std::istringstream���Ի�ȡ����Ϊ���ݹ���һ���ַ���������
        std::string name, type;                 // �ļ�ÿ���еĶ���������������
        int visit_time;                         // �ļ���ÿ�еĶ����Ƽ�����ʱ��
        sin >> name >> type >> visit_time;      // �����ѹ�����ַ�������������ȡ��Ӧ�ļ�������
        nodes.push_back({ name, visit_time, type });  // ������Ҫ���ص�std::vector��
    }
    fin.close();                                // �ļ�ʹ����Ϻ������ʹ��close���ʹر��������������ļ���
    return nodes;
}

/* ���ļ��ж�ȡ����Ϣ
 * @param edges_path: ���ļ�·��
 * @return: ������ȡ�������бߵ�std::vector
 */
std::vector<edge> read_edges(const std::string& edges_path) {
    std::ifstream fin(edges_path);              // �����ļ�����������
    assert(fin.good());                         // ����ļ��Ƿ����
    std::vector<edge> edges;                    // �洢����Ϣ������
    std::string line;

    while (std::getline(fin, line)) {           // ���ж�ȡ�ļ�����
        for (auto& c : line) {                  // �滻����Ϊ�ո��Ա����
            if (c == ',') c = ' ';
        }
        std::istringstream sin(line);           // ʹ���ַ���������������
        std::string from, to;
        int length;
        sin >> from >> to >> length;            // ��ȡ��㡢�յ�ͳ���
        edges.push_back({from, to, length});   // ������Ϣ��ӵ�����
    }

    fin.close();                                // �ر��ļ���
    return edges;
}

void init(LGraph& graph) {
    std::vector<VertInfo> nodes{ read_nodes(nodes_path) };        // ͨ��read_nodes������ö�����Ϣ
    std::vector<edge> edges{ read_edges(edges_path) };            // ͨ��read_edges������ñ���Ϣ
    for (auto& v : nodes) {
        // ֱ��ʹ�� VertInfo���� LocationInfo�����붥��
        graph.InsertVertex(v);
    }

    for (auto& e : edges) {
        // ���� InsertEdge ����������Ϊ��������յ���������
        graph.InsertEdge(e.from, e.to, e.length);
    }
}

/* ��ӡĿǰ�����ж���
 * @param graph: ͼ
 */
void show_all_nodes(const LGraph& graph) {
    // TODO: ʵ�ִ�ӡ���ж�����Ϣ
    // TODO: ÿ�������ʽ��������,��������,�Ƽ�����ʱ��
    const auto& vertex_map = graph.Map();  // ��ȡ��������ID��ӳ��
    for (const auto& pair : vertex_map) {
        const std::string& name = pair.first;          // ������
        const VertInfo& info = graph.GetVertex(name);  // ��ȡ������ϸ��Ϣ
        // �����ʽ��������,��������,�Ƽ�����ʱ��
        std::cout << info.name << "," << info.type << "," << info.visitTime << std::endl;
    }
}

/* ��ӡĿǰ�����б�
 * @param graph: ͼ
 */
 
void show_all_edges(const LGraph& graph) {
    // LGraph��ֻ�ṩ�˻�ȡ�ڽӱ�ͻ�ȡ��������ıߵķ���
    // Ϊ���ܹ���ȡ��ÿ���ߵ�Ԫ��Ϣ��������Ҫ������������������Ϣ�Ļ�ȡ
    const auto& graph_edges{ graph.SortedEdges() };       // ��ȡ��������ı�
    const auto& graph_list{ graph.List() };               // ��ȡ�ڽӱ�
    for (const auto& en : graph_edges) {                 // �����ߣ�ͨ���±�����ȡ��Ӧ�Ķ�����Ϣ
        const auto& e = edge{ graph_list[en.from].data.name, graph_list[en.dest].data.name, en.weight };
        std::cout << e.from << "," << e.to << "," << e.length << std::endl;
    }
}



/* ��������Ϣ�洢���ļ���
 * @param nodes_path: �����ļ�·��
 * @param graph: ͼ
 */
void store_nodes(const std::string& nodes_path, const LGraph& graph) {
    std::ofstream fout(nodes_path);                                         // ͨ��Ŀ¼�����ļ������
    const auto& graph_list{ graph.List() };                                   // ͨ��LGraph�����List()��������ڽӱ�
    for (const auto& n : graph_list) {                                       // �����ڽӱ��е�ÿ������
        const auto& v = n.data;                                             // ��ö����е�Ԫ����
        fout << v.name << "," << v.type << "," << v.visitTime << std::endl; // ���ն���洢�ĸ�ʽ���������뵽�ļ���
    }
    fout.close();                                                           // ��ʹ�����κ��ļ����󣬶�Ҫʹ��close�����ر�
}

/* ������Ϣ�洢���ļ���
 * @param edges_path: ���ļ�·��
 * @param graph: ͼ
 */
void store_edges(const std::string& edges_path, const LGraph& graph) {
    // TODO: ��ο�store_nodes��������������ʾ�����edges.csv�ļ�������
    // TODO: �Լ�show_all_edges�����л�ȡ�ߵ�Ԫ��Ϣ�ķ�ʽ���store_edges�����Ķ���
    
    std::ofstream fout(edges_path);                     // �����ļ������
    const auto& graph_edges{ graph.SortedEdges() };      // ��ȡ�����ı��б���ȥ�أ�
    const auto& graph_list{ graph.List() };              // ��ȡ�ڽӱ��Խ�����������

    for (const auto& en : graph_edges) {                 // ����ÿ����
        const std::string& from = graph_list[en.from].data.name;  // �������
        const std::string& to = graph_list[en.dest].data.name;    // �յ�����
        fout << from << "," << to << "," << en.weight << std::endl; // д��CSV��ʽ
    }

    fout.close();                                        // �ر��ļ���
}
