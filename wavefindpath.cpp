#include <iostream>
#include <vector>
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_iterator.hpp"
#include "utils.h"


// Глобальные переменные для хранения адресов
ScAddr graph, rrel_arcs, rrel_nodes, father;

// Функция для проверки множества на наличие элементов
bool set_is_not_empty (const std::unique_ptr<ScMemoryContext>& context, ScAddr set)
{
    // Итератор для поиска элементов в множестве
    ScIterator3Ptr check = context->Iterator3(
            set,
            ScType::EdgeAccessConstPosPerm,
            ScType(0)
            );
    // Если есть хотя бы 1 элемент, то множество не пустое
    if (check->Next())
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Функция поиска вершины в множестве
bool find_vertex_in_set (const std::unique_ptr<ScMemoryContext>& context, ScAddr element, ScAddr set)
{
    bool find = false;
    // Получение всех элементов множества
    ScIterator3Ptr location = context->Iterator3(
            set,
            ScType::EdgeAccessConstPosPerm,
            ScType(0)
            );
    // Итерируемся по всем элементам
    while (location->Next())
    {
        // В loc записываем адрес конкретного элемента
        // принадлежащего множеству
        ScAddr loc = location->Get(2);

        if (loc != element)
        {
            find = false;
            continue;
        }
        else
        {
            find = true;
            break;
        }
    }
    return find;
}

// Функция для получения адресов граничных вершин дуги
void get_edge_vertexes (const std::unique_ptr<ScMemoryContext>& context, ScAddr edge, ScAddr &v1, ScAddr &v2)
{
    v1 = context->GetEdgeSource(edge);
    v2 = context->GetEdgeTarget(edge);
}

// Функция для вывода графа
void print_graph (const std::unique_ptr<ScMemoryContext>& context)
{
    ScAddr arcs, nodes, v1, v2, printed_vertex;
    bool find;
    // Создаём множество напечатанных вершин
    printed_vertex = context->CreateNode(ScType::Const);

    // Выводим имя графа
    printEl(context, graph);
    std::cout << std::endl << "----------------------" << std::endl;

    // Ищем множество дуг графа
    ScIterator5Ptr it = context->Iterator5(
            graph,
            ScType::EdgeAccessConstPosPerm,
            ScType(0),
            ScType::EdgeAccessConstPosPerm,
            rrel_arcs
            );

    if (it->Next())
    {
        // В arcs записываем адрес найденного элемента
        arcs = it->Get(2);

        // Ищем все дуги графа
        ScIterator3Ptr arcs_it = context->Iterator3(
                arcs,
                ScType::EdgeAccessConstPosPerm,
                ScType(0)
                );

        // Запускаем цикл по всем дугам
        while (arcs_it->Next())
        {
            ScAddr t_arc = arcs_it->Get(2);

            // Заносим в вершины v1 и v2 адреса границ дуги
            get_edge_vertexes (context,t_arc, v1, v2);

            // Выводим трёхэлементную конструкцию
            printEl(context, v1);
            std::cout << " -- ";
            printEl(context, v2);
            std::cout << std::endl;

            // Если вершины не были напечатаны (т.е. не принадлежат множеству
            // напечатанных вершин), то заносим их туда (т.е. создаём дугу принадлежности)
            if (!find_vertex_in_set(context, v1, printed_vertex))
                context->CreateEdge(ScType::EdgeAccessConstPosPerm, printed_vertex, v1);
            if (!find_vertex_in_set(context, v2, printed_vertex))
                context->CreateEdge(ScType::EdgeAccessConstPosPerm, printed_vertex, v2);
        }
    }

    // Ищем множество врешин графа
    it = context->Iterator5(
            graph,
            ScType::EdgeAccessConstPosPerm,
            ScType(0),
            ScType::EdgeAccessConstPosPerm,
            rrel_nodes
            );

    if (it->Next())
    {
        // В nodes записываем адрес найденного элемента
        nodes = it->Get(2);

        // Ищем все вершины графа
        ScIterator3Ptr nodes_it = context->Iterator3(
                nodes,
                ScType::EdgeAccessConstPosPerm,
                ScType(0)
                );


        // Запускаем цикл по всем вершинам
        while (nodes_it->Next())
        {
            ScAddr t_node = nodes_it->Get(2);
            //Проверяем, напечатана ли рассматриваемая вершина 
            find = find_vertex_in_set(context,t_node, printed_vertex);

            // Если вершина не была напечатана, то печатаем её
            if (!find)
            {
                printEl(context, t_node);
                std::cout << std::endl;
            }
        }
    }
}

// Функция для получения вершины инцидентной vertex
ScAddr get_other_vertex_incidence_edge (const std::unique_ptr<ScMemoryContext>& context, ScAddr edge, ScAddr vertex)
{
    ScAddr v1, v2, empty;

    get_edge_vertexes(context, edge, v1, v2);
    if (vertex == v1 || vertex == v2)
    {
        if (vertex == v1)
        {
            return v2;
        }
        else
        {
            return v1;
        }
    }
    return empty;
}

// Функция для печати пути
void print_route (const std::unique_ptr<ScMemoryContext>& context, ScAddr beg, ScAddr end)
{
    printEl(context, end);

    ScAddr curr_vert, curr_ver = end;

    while (true)
    {
        ScIterator5Ptr arcs = context->Iterator5(
                curr_ver,
                ScType::EdgeDCommon,
                ScType(0),
                ScType::EdgeAccessConstPosPerm,
                father
                );
        if (arcs->Next())
        {
            curr_vert = arcs->Get(2);
            std::cout << "<-";
            printEl(context, curr_vert);
            if (curr_vert == beg) break;
            curr_ver = curr_vert;
        }
        else break;
    }
}

ScAddr create_wave (const std::unique_ptr<ScMemoryContext>& context, ScAddr wave, ScAddr &not_checked_vertexes)
{
    // Создаём константный узел new_wave
    ScAddr new_wave = context->CreateNode(ScType::Const);

    // Ищем узлы принадлежащие волне
    ScIterator3Ptr it_vertex = context->Iterator3(
            wave,
            ScType::EdgeAccessConstPosPerm,
            ScType(0)
            );

    // Итерируемся по всем составляющим волны
    while (it_vertex->Next())
    {
        ScAddr vertex = it_vertex->Get(2);

        // Получаем множество дуг графа
        ScIterator5Ptr arcs = context->Iterator5(
                graph,
                ScType::EdgeAccessConstPosPerm,
                ScType(0),
                ScType::EdgeAccessConstPosPerm,
                rrel_arcs
                );
        if (arcs->Next())
        {
            ScAddr set_arcs = arcs->Get(2);
            // Получаем все дуги графа
            ScIterator3Ptr it_arc = context->Iterator3(
                    set_arcs,
                    ScType::EdgeAccessConstPosPerm,
                    ScType(0)
                    );

            // Итерируемся по всем дугам графа
            while (it_arc->Next())
            {
                ScAddr t_arc = it_arc->Get(2);
                // Получаем следующую вершину
                ScAddr other_vertex = get_other_vertex_incidence_edge(context, t_arc, vertex);

                if (!other_vertex.IsValid()) continue;
                // Проверяем, была ли просмотрена полученная вершина
                ScIterator3Ptr find = context->Iterator3(
                        not_checked_vertexes,
                        ScType::EdgeAccessConstPosPerm,
                        other_vertex
                        );

                if (find->Next())
                {
                    // Если вершина не была просмотрена, то заносим её в новую волну
                    // Создаём дугу между other_vertex и vertex и связываем их отношением father
                    ScAddr edge = find->Get(1);
                    context->EraseElement(edge);
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm, new_wave, other_vertex);
                    ScAddr boof = context->CreateEdge(ScType::EdgeDCommon, other_vertex, vertex);
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm, father, boof);
                }
            }
        }
    }

    if (set_is_not_empty(context, new_wave))
    {
        return new_wave;
    } 
    else 
    {
        context->EraseElement(new_wave);
        ScAddr new_wave;
        return new_wave;
    }
}

// Функция для поиска минимального пути
ScAddr find_min_path (const std::unique_ptr<ScMemoryContext>& context, ScAddr beg_vertex, ScAddr end_vertex)
{
    ScAddr empty;

    bool check = false;
    // Создаём множество не просмотренных вершин
    ScAddr not_checked_vertexes = context->CreateNode(ScType::Const);

    // Получаем множество вершин графа
    ScIterator5Ptr vertexes = context->Iterator5(
            graph,
            ScType::EdgeAccessConstPosPerm,
            ScType(0),
            ScType::EdgeAccessConstPosPerm,
            rrel_nodes
            );

    if (vertexes->Next())
    {
        ScAddr set_vertexes = vertexes->Get(2);
        // Получаем все вершины графа
        ScIterator3Ptr it_vertex = context->Iterator3(
                set_vertexes,
                ScType::EdgeAccessConstPosPerm,
                ScType(0)
                );
        // Заносим все вершины в множество не просмотренных
        while (it_vertex->Next())
        {
            ScAddr curr_vertex = it_vertex->Get(2);

            if (curr_vertex != beg_vertex)
            {
                if (!find_vertex_in_set(context, curr_vertex, not_checked_vertexes))
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm, not_checked_vertexes, curr_vertex);
            }
        }
    }

    // Создаём адрес новой волны
    ScAddr new_wave = context->CreateNode(ScType::Const);
    // Добавляем начальную вершину в новую волну
    context->CreateEdge(ScType::EdgeAccessConstPosPerm, new_wave, beg_vertex);

    ScAddr wave_list = context->CreateNode(ScType::Const);
    ScAddr wave_list_tail = context->CreateEdge(ScType::EdgeAccessConstPosPerm, wave_list, new_wave);

    do 
    {
        // Формируем новую волну
        new_wave = create_wave(context, new_wave, not_checked_vertexes);


        // Если волна пустая, то удаляем ненужные элементы
        if (!new_wave.IsValid()) 
        {
            context->EraseElement(wave_list);
            context->EraseElement(new_wave);
            context->EraseElement(not_checked_vertexes);
            return empty;
        }

        wave_list_tail = context->CreateEdge(ScType::EdgeAccessConstPosPerm, wave_list, new_wave);

        // Получаем все элементы волны
        ScIterator3Ptr find_end = context->Iterator3(
                new_wave,
                ScType::EdgeAccessConstPosPerm,
                ScType(0)
                );

        // Ищем последний элемент волны
        while (find_end->Next())
        {
            ScAddr tmp_vertex = find_end->Get(2);
            if (tmp_vertex == end_vertex)
            {
                check = true;
                continue;
            }
        }
    } while (!check);

    // Удаляем ненужные элементы
    context->EraseElement(not_checked_vertexes);
    empty = context->CreateNode(ScType::Const);
    return empty;
}

void run_test (const std::unique_ptr<ScMemoryContext>& context, std::string number_test, std::string beg_vertex, std::string end_vertex)
{
    ScAddr beg, end;
    father = context->CreateNode(ScType::Const);

    std::string gr = "Gx";
    gr[1] = number_test[0];
    graph = context->HelperResolveSystemIdtf(gr);
    rrel_arcs = context->HelperResolveSystemIdtf("rrel_arcs");
    rrel_nodes = context->HelperResolveSystemIdtf("rrel_nodes");
    beg = context->HelperResolveSystemIdtf(beg_vertex);
    end = context->HelperResolveSystemIdtf(end_vertex);
    std::cout << "Graph: ";

    print_graph(context);

    std::cout << "Find minimal path from " << beg_vertex << " to " << end_vertex << " " << std::endl;
    ScAddr label = find_min_path(context,beg, end);

    std::cout << "Path";

    if (context->IsElement(label))
    {
        std::cout << ": " << std::endl;
        print_route(context,beg, end);
        context->EraseElement(label);
    }
    else std::cout << " doesn't exist" << std::endl;

    std::cout << std::endl;
    context->EraseElement(father);
}


int main()
{
    sc_memory_params params;

    sc_memory_params_clear(&params);
    params.repo_path = "<absolute/path/to/ostis-web-platform>/kb.bin";
    params.config_file = "<absolute/path/to/ostis-web-platform>/config/sc-web.ini";
    params.ext_path = "<absolute/path/to/ostis-web-platform>/sc-machine/bin/extensions";
    params.clear = SC_FALSE;

    ScMemory mem;
    mem.Initialize(params);

    const std::unique_ptr<ScMemoryContext> context(new ScMemoryContext(sc_access_lvl_make_max, "example"));
    run_test(context,"0", "V1", "V3");
    run_test(context,"1", "V1", "V5");
    run_test(context,"2", "V1", "V6");
    run_test(context,"3", "V1", "V9");
    run_test(context,"4", "V5", "V11");


    mem.Shutdown(true);

    return 0;
}
