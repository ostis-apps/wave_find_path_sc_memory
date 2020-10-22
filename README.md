                                                                        КП ППвИС(часть 1)
                                                                            Установка

Для выполнения первой части КП по курсу ППвИС разработан пример реализации волнового алгоритма поиска минимального пути в графе на базе библиотеки sc-memory.

На вашем компьютере должна быть установлена версия ostis ветки 0.5.0 из репозитория: https://github.com/ShunkevichDV/ostis и Clion.

Последние версии указанного примера и данного документа могут быть найдены в репозитории: https://github.com/ostis-apps/wave_find_path_sc_memory
или на сервере: Info/Studinfo/~Методическое обеспечение кафедры/~Учебные курсы/2 курс/ПпвИС/1sem/КП 2020

1) Запустите терминал и перейдите в папку ostis, выполните команду git clone https://github.com/ostis-apps/wave_find_path_sc_memory

2) Перейдите в папку wave_find_path_sc_memory и скопируйте папку graph в ostis/kb

3) Запустите Clion. Запустив среду, необходимо выбрать пункт меню Open Project и в появившемся диалоговом окне указать файл CMakeLists.txt,
находящийся в папке с примером.

4) После этого проект будет виден во вкладке Редактор(Edit). Перейдите в файл wavefindpath.cpp

5) Измените переменные в функции main
    params.repo_path = "/home/<имя_пользователя>/../kb.bin";
    params.config_file = "/home/<имя_пользователя>/../config/sc-web.ini";
    params.ext_path = "/home/<имя_пользователя>/../sc-machine/bin/extensions";

6) Перейдите в файл /home/<имя_компьютера>/../config/sc-web.ini и измените в нем строчки Path и Directory на:
    Path = /home/<имя_пользователя>/../kb.bin
    Directory = /home/<имя_пользователя>/../sc-machine/bin/extensions

7) Пересоберите базу знаний (~/../scripts$ ./build_kb.sh)

8)  Перейдите во вкладку Run. Найтите строчку Edit configuration и нажмите кнопку Add.
Изменить конфигурацию в строчке Executable нажмите на кнопку Browse... выберите файл wave,
который должен находится по пути /home/<имя_пользователя>/../sc-machine/bin/wave .

Примечание: Если файла wave нет, попробуйте выполнить пункт 7) еще раз

9)Нажмите на вкладку проект (Project) и перейдите во вкладку Run. Найтите строчку Edit configuration.
Добавьте в строку Environment variables «LD_LIBRARY_PATH=/home/<имя_пользователя>/../sc-machine/bin».
Найдите строчку Working directory, там путь должен выглядеть следующим образом : /home/<имя_пользователя>/../sc-machine/bin




10) Сохраните конфигурацию Ctrl+S.


11) Соберите и запустите проект. Сборка проекта — Ctrl+9, запуск проекта — Shift++10
Программа должна найти пути(если они есть) для графов которые находятся в папке ostis/kb/graph.




                                                                          Основные функции

ScAddr — основной тип данных, предназначенный для хранения адреса sc-элемента в памяти.
По этому адресу мы можем хранить sc-узел (context->CreateNode(ScType)), sc-ссылку (context->CreateLink())
или sc-дугу (context->CreateEdge(ScType, const ScAddr, const ScAddr)).

Типы дуг:
    • ScType::EdgeAccessConstPosPerm  - константная позитивная sc-дуга принадлежности
      
    • ScType::Const - константный sc-узел
      
    • sScType::EdgeDCommon — sc-ребро общего вида

Функции для работы с sc-памятью:

    • ScAddr printed_vertex; 
		 	- объявляется переменная printed_vertex, которая содержит адрес узла printed_vertex в sc-памяти (sc-адрес)

    • printed_vertex = context->CreateNode(ScType::Const);
			 - в sc-памяти создается новый узел, sc-адрес которого возвращается переменной printed_vertex

    • context->CreateEdge(ScType::EdgeAccessConstPosPerm,printed_vertex, v1);
			- функция создает дугу с типом type (например,EdgeAccessConstPosPerm) от узла printed_vertex к узлу v1

    • graph = context->HelperResolveSystemIdtf(gr);
			-функция записывает в graph адрес элемента, имеющего системный идентификатор gr.

    • context->IsElement(lebel);
			-функция возвращает true, если элемент label существует в sc-памяти, и false, если элемент lebel удалён.
      
    • scontext->EraseElement(lebel);
			-функция удаляет элемент по адресу lebel из sc-памяти.

Операторы сравнения sc-памяти:

 • vertex == v1;
   - возвращает true, если vertex эквивалентен v1, и false, если vertex не эквивалентен v1
 • loc != element;
      - функция возвращает true, если loc не эквивалентен element, и false, если loc эквивалентен element
      
Методы sc-памяти:

    • addr.IsValid();
      - возвращает true, если адрес addr непустой, и false, если адрес addr пустой.
      
    • Поумолчанию при создании sc-узла он является пустым.

Функции, реализованные в примере:

    • find_vertex_in_set(context, v1, printed_vertex); 
      - функция проверяет, есть ли элемент v1 во множестве  printed_vertex. Возвращает false если элемента нет, и true если элемент есть.
      
    • printEl(context, t_node); 
      - функция выводит в консоль идентификатор узла t_node
      
    • get_edge_vertexes(context,t_arc, v1, v2);
      - функция записывает начало дуги t_arc в v1, конец дуги t_arc в v2
      
    • other_vertex = get_other_vertex_incidence_edge(context,t_arc, vertex);
      -функция возвращает переменной other_vertex вершину графа G0, которая связана с вершиной vertex дугой t_arc.
      
    • Функции для работы с sc-ссылками:
      
    • ScMemoryContext::GetLinkContent(ScAddr const & addr, ScStream & stream);
                 - сохраняет в поток содержимое sc-ссылки
                 
    • ScStream::StreamToString(ScStream const & stream, std::string & outString);
                 - передает данные из потока в строку
                 
    • ScMemoryContext::SetLinkContent(ScAddr const & addr, ScStream const & stream);
                 - передает данные из потока в sc-ссылку
        
    • StreamFromString(std::string const & str, ScStreamMemory & outStream);;
                 - передает данные из строки в поток
      
Представление графа в SCs-коде 

В SСs и SСg граф G0 состоящий из трех вершин можно записать следующим образом: 

SCs: 								SCg:

graph -> G0;;
G0 -> rrel_arcs: ..arcs;;
G0 -> rrel_nodes: ..nodes;;

..nodes -> V1;;
..nodes -> V2;;
..nodes -> V3;;

..arcs -> (V1 => V2);;
..arcs -> (V2 => V3);;

//arcs — множество дуг 
//nodes — множество вершин

//rrel_arcs — дуга'
//rrel_nodes — вершина'

                                                                            Итераторы

                                                              Трёхэлементный итератор (итератор 3)

ScIterator3Ptr ищет конструкции типа : V1------>V2

ScIterator3Ptr задаётся тремя параметрами. 

ScIterator3Ptr nodes_it = context->Iterator3(nodes,                                              //известный элемент
                                             ScType::EdgeAccessConstPosPerm,                     //тип дуги
                                             ScType(0));                                         //тип элемента (0 - любой)

- функция создает трёхэлементный итератор с названием nodes_it . Итератор будет искать для нашего G0 вершины V1, V2 и V3,
в данном случае эти вершины — неизвестное (2) 









nodes_it->Next();
- функция переходит к следующему элементу nodes_it и возвращает true, если такой элемент есть,
в противном случае она возвращает false (при первом использовании находит первую конструкцию).

ScAddr t_node = nodes_it->Get(2);
- функция заносит в узел t_node элемент итератора nodes_it под номером 2 (наше неизвестное).

В переменной nodes хранится множество всех вершин графа G0. Итератор nodes_it найдет три удовлетворяющие условию конструкции (смотрите SCg).
В переменную t_node на каждом шаге занесется вершина, на которую указывает итератор nodes_it.


                                                                Пятиэлементный итератор (итератор 5)

sc_iterator5 ищет конструкции типа : V1----------->V2
                                            ^
                                            |
                                            |
                                            |
                                            V3
sc_iterator5 задаётся пятью параметрами.
  
it = context->Iterator5(graph,                                             //V1
                        ScType::EdgeAccessConstPosPerm,                    //тип дуги от V1 к V2
                        ScType(0),                                         //тип элемента V2 (0 - любой)
                        ScType::EdgeAccessConstPosPerm,                    //тип дуги от V3 к дуге (от V1 к V2)
                        rrel_nodes);                                       //V3
                        
-функция создает пятиэлементный итератор с названием it. Итератор будет искать  пятиэлементные конструкции,
(?) - неизвестное задано параметром 0, чтобы учесть все типы sc-элементов

it->Next();
- функция переходит к следующему элементу it и возвращает true, если есть такой элемент,
в противном случае она возвращает false (при первом использовании находит первую конструкцию).

nodes = it->Get(2);
- функция заносит в узел nodes элемент итератора it под номером 2 (наше неизвестное).

В переменной graph хранится название нашего графа, в первом тесте это G0.
Итератор it найдет единственную удовлетворяющую конструкцию и занесет в переменную  nodes - узел с названием  nodes (смотрите SCg) — наше неизвестное (V2).


Пример работы итераторов на основе функции вывода всех вершин :

void showAllNodes() // Функция выводит на экран все вершины графа graph
{
    std::cout << "Vertex : ";
    ScIterator5Ptr it = context->Iterator5(                                 // Создаём итератор c названием it с пятью элементами (0, 1, 2, 3, 4)
                                          graph,                            // 0 - известный узел
                                          ScType::EdgeAccessConstPosPerm,   // 1 - неизвестная  дуга принадлежности
                                          ScType(0),                        // 2 - неизвестное
                                          ScType::EdgeAccessConstPosPerm,   // 3 - неизвестная дуга принадлежности
                                          rrel_nodes);                      // 4 - известный узел
                                          
    if(it->Next()){                                                         // Переходим к первому элементу итератора it, если он есть
       nodes = it->Get(2);                                                  // В переменную nodes заносим 2 элемент, на который указывает итератор it
                                                                            // В переменной nodes хранится объект nodes, т. к.  nodes <- graph связаны отношением rrel_nodes
                                                                            
        ScIterator3Ptr nodes_it = context->Iterator3(                       // Создаем итератор c названием nodes_it с тремя элементами (0, 1, 2)
                                          nodes,                            // 0 - известный узел
                                          ScType::EdgeAccessConstPosPerm,   // 1 - неизвестная дуга принадлежности
                                          ScType(0));                       // 2 - неизвестное
        while(nodes_it->Next(){
                                                                            // Если функция использована 1 раз: Переходим к первому элементу итератора nodes_it, если он есть
                                                                            // Если функция использована 2 раз: Переходим к следующему элементу итератора nodes_it, если он есть
              ScAddr t_node = nodes_it->Get(2);
                                                                            // В переменную t_node заносим 2 элемент, на который указывает итератор it
                                                                            // В переменной t_node хранится вершина графа graph (вершина может быть любой)
                                                                            
              printEl(context, t_node);                                     // Выводим в консоль название этой вершины
              std::cout << " ";
              }
       }
    std::cout << std::endl;
}    


