#include <iostream>
#include <vector>
#include "cpp/sc_addr.hpp"
#include "cpp/sc_memory.hpp"
#include "cpp/sc_iterator.hpp"
#include "utils.h"


ScAddr graph, rrel_arcs, rrel_nodes, father;

bool set_is_not_empty (const std::unique_ptr<ScMemoryContext>& context, ScAddr set){
    ScIterator3Ptr check = context->Iterator3(set,ScType::EdgeAccessConstPosPerm,ScType(0));
    if (check->Next()) {
        return true;
    } else {
        return false;
    }
}

bool find_vertex_in_set (const std::unique_ptr<ScMemoryContext>& context,ScAddr element, ScAddr set){
    bool find = false;
    ScIterator3Ptr location = context->Iterator3(set,ScType::EdgeAccessConstPosPerm,ScType(0));

    while (location->Next()) {
        ScAddr loc = location->Get(2);

        if (loc != element) {
            find = false;
            continue;
        } else {
            find = true;
            break;
        }
    }
    return find;
}

void get_edge_vertexes (const std::unique_ptr<ScMemoryContext>& context,ScAddr edge, ScAddr &v1, ScAddr &v2){
    v1 = context->GetEdgeSource(edge);
    v2 = context->GetEdgeTarget(edge);
}

void print_graph (const std::unique_ptr<ScMemoryContext>& context){

    ScAddr arcs, nodes, v1, v2, printed_vertex;
    bool find;
    printed_vertex = context->CreateNode(ScType::Const);

    printEl(context, graph);
    std::cout << std::endl << "----------------------" << std::endl;

    ScIterator5Ptr it = context->Iterator5(graph,ScType::EdgeAccessConstPosPerm,ScType(0),ScType::EdgeAccessConstPosPerm,rrel_arcs);

    if (it->Next()) {
        arcs = it->Get(2);

        ScIterator3Ptr arcs_it = context->Iterator3(arcs,ScType::EdgeAccessConstPosPerm,ScType(0));

        while (arcs_it->Next()) {

            ScAddr t_arc = arcs_it->Get(2);

            get_edge_vertexes (context,t_arc, v1, v2);

            printEl(context, v1);
            std::cout << " -- ";
            printEl(context, v2);
            std::cout << std::endl;

            if (!find_vertex_in_set(context,v1, printed_vertex))
                context->CreateEdge(ScType::EdgeAccessConstPosPerm,printed_vertex, v1);
            if (!find_vertex_in_set(context,v2, printed_vertex))
                context->CreateEdge(ScType::EdgeAccessConstPosPerm,printed_vertex, v2);
        }
    }

    it = context->Iterator5(graph,ScType::EdgeAccessConstPosPerm,ScType(0),ScType::EdgeAccessConstPosPerm,rrel_nodes);

    if (it->Next()) {
        nodes = it->Get(2);

        ScIterator3Ptr nodes_it = context->Iterator3(nodes,ScType::EdgeAccessConstPosPerm,ScType(0));


        while (nodes_it->Next()) {

            ScAddr t_node = nodes_it->Get(2);

            find = find_vertex_in_set(context,t_node, printed_vertex);

            if (!find) {
                printEl(context, t_node);
                std::cout << std::endl;
            }
        }
    }
}

ScAddr get_other_vertex_incidence_edge (const std::unique_ptr<ScMemoryContext>& context,ScAddr edge, ScAddr vertex){
    ScAddr v1, v2, empty;


    get_edge_vertexes(context,edge, v1, v2);
    if (vertex == v1 || vertex == v2) {
        if (vertex == v1) {
            return v2;
        } else {
            return v1;
        }
    }
    return empty;
}

void print_route (const std::unique_ptr<ScMemoryContext>& context,ScAddr beg, ScAddr end){
    printEl(context, end);


    ScAddr curr_vert, curr_ver = end;

    while (true) {
        ScIterator5Ptr arcs = context->Iterator5(curr_ver,ScType::EdgeDCommon,ScType(0),ScType::EdgeAccessConstPosPerm,father);
        if (arcs->Next()) {
            curr_vert = arcs->Get(2);
            std::cout << "<-";
            printEl(context, curr_vert);
            if (curr_vert == beg) break;
            curr_ver = curr_vert;
        } else break;
    }

}

ScAddr create_wave (const std::unique_ptr<ScMemoryContext>& context,ScAddr wave, ScAddr &not_checked_vertexes){
    ScAddr new_wave = context->CreateNode(ScType::Const);

    ScIterator3Ptr it_vertex = context->Iterator3(wave,ScType::EdgeAccessConstPosPerm,ScType(0));

    while (it_vertex->Next()) {
        ScAddr vertex = it_vertex->Get(2);

        ScIterator5Ptr arcs = context->Iterator5(graph,ScType::EdgeAccessConstPosPerm,ScType(0),ScType::EdgeAccessConstPosPerm,rrel_arcs);
        if (arcs->Next()) {
            ScAddr set_arcs = arcs->Get(2);
            ScIterator3Ptr it_arc = context->Iterator3(set_arcs,ScType::EdgeAccessConstPosPerm,ScType(0));

            while (it_arc->Next()) {
                ScAddr t_arc = it_arc->Get(2);
                ScAddr other_vertex = get_other_vertex_incidence_edge(context,t_arc, vertex);

                if (!other_vertex.IsValid()) {
                    continue;
                }
                ScIterator3Ptr find = context->Iterator3(not_checked_vertexes,ScType::EdgeAccessConstPosPerm,other_vertex);

                if (find->Next()) {
                    ScAddr edge = find->Get(1);
                    context->EraseElement(edge);
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm,new_wave, other_vertex);
                    ScAddr boof = context->CreateEdge(ScType::EdgeDCommon,other_vertex, vertex);
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm,father, boof);
                    // sc_memory_element_free(boof);
                }
            }
        }
    }

    if (set_is_not_empty(context,new_wave)) {
        return new_wave;
    } else {
        context->EraseElement(new_wave);
        ScAddr new_wave;
        return new_wave;
    }
}

ScAddr find_min_path (const std::unique_ptr<ScMemoryContext>& context,ScAddr beg_vertex, ScAddr end_vertex){
    ScAddr empty;

    bool check = false;
    ScAddr not_checked_vertexes = context->CreateNode(ScType::Const);

    ScIterator5Ptr vertexes = context->Iterator5(graph,ScType::EdgeAccessConstPosPerm,ScType(0),ScType::EdgeAccessConstPosPerm,rrel_nodes);

    if (vertexes->Next()) {
        ScAddr set_vertexes = vertexes->Get(2);
        ScIterator3Ptr it_vertex = context->Iterator3(set_vertexes,ScType::EdgeAccessConstPosPerm,ScType(0));
        while (it_vertex->Next()) {
            ScAddr curr_vertex = it_vertex->Get(2);

            if (curr_vertex != beg_vertex) {
                if (!find_vertex_in_set(context,curr_vertex, not_checked_vertexes))
                    context->CreateEdge(ScType::EdgeAccessConstPosPerm,not_checked_vertexes,curr_vertex);
            }
        }
    }

    ScAddr new_wave = context->CreateNode(ScType::Const);
    context->CreateEdge(ScType::EdgeAccessConstPosPerm,new_wave, beg_vertex);

    ScAddr wave_list = context->CreateNode(ScType::Const);
    ScAddr wave_list_tail = context->CreateEdge(ScType::EdgeAccessConstPosPerm,wave_list,new_wave);

    do {
        new_wave = create_wave(context,new_wave, not_checked_vertexes);


        if (!new_wave.IsValid()) {
            context->EraseElement(wave_list);
            context->EraseElement(new_wave);
            context->EraseElement(not_checked_vertexes);
            return empty;
        }

        wave_list_tail = context->CreateEdge(ScType::EdgeAccessConstPosPerm,wave_list, new_wave);

        ScIterator3Ptr find_end = context->Iterator3(new_wave,ScType::EdgeAccessConstPosPerm,ScType(0));

        while (find_end->Next()) {
            ScAddr tmp_vertex = find_end->Get(2);
            if (tmp_vertex == end_vertex) {
                check = true;
                continue;
            }
        }

    }
    while (!check);

    context->EraseElement(not_checked_vertexes);
    empty = context->CreateNode(ScType::Const);
    return empty;

}

void run_test (const std::unique_ptr<ScMemoryContext>& context,std::string number_test, std::string beg_vertex, std::string end_vertex){
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

    std::cout << "Find minimal path from " << beg_vertex << " to "
              << end_vertex << " " << std::endl;
    ScAddr lebel = find_min_path(context,beg, end);

    std::cout << "Path";

    if (context->IsElement(lebel)) {
        std::cout << ": " << std::endl;
        print_route(context,beg, end);
        context->EraseElement(lebel);
    } else {
        std::cout << " doesn't exist" << std::endl;
    }

    std::cout << std::endl;
    context->EraseElement(father);
}


int main()
{
    sc_memory_params params;

    sc_memory_params_clear(&params);
    params.repo_path = "/home/tima/ostis/kb.bin";
    params.config_file = "/home/tima/ostis/config/sc-web.ini";
    params.ext_path = "/home/tima/ostis/sc-machine/bin/extensions";
    params.clear = SC_FALSE;

    ScMemory mem;
    mem.Initialize(params);

    const std::unique_ptr<ScMemoryContext> context(new ScMemoryContext(sc_access_lvl_make_max,"example"));
    run_test(context,"0", "V1", "V3");
    run_test(context,"1", "V1", "V5");
    run_test(context,"2", "V1", "V6");
    run_test(context,"3", "V1", "V9");
    run_test(context,"4", "V5", "V11");



    //////////////////////////////////////////////////////////////////////////////////

    std::cout << "The end" << std::endl;

    mem.Shutdown(true);

    return 0;
}

