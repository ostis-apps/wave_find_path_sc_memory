   ## КП ППвИС (часть 1)
   
   - [Установка](#установка)
   - [Запуск в VS Code](#запуск-в-vs-code)
   - [Запуск в CLion](#запуск-в-clion)
   - [Ручной запуск](#ручной-запуск)
   - [Представление графов в sc-памяти](#представление-графов-в-sc-памяти)
   - [Основные функции](#основные-функции)

### Установка

Для выполнения первой части КП по курсу ППвИС разработан пример реализации волнового алгоритма поиска минимального пути в графе на базе библиотеки sc-memory.

На вашем компьютере должна быть установлена версия ostis ветки 0.6.0 из репозитория: https://github.com/ostis-dev/ostis-web-platform.

Последние версии указанного примера и данного документа могут быть найдены в репозитории: https://github.com/ostis-apps/wave_find_path_sc_memory

1. Запустите терминал и перейдите в папку *ostis-web-platform*, выполните команду 
	```
	git clone https://github.com/ostis-apps/wave_find_path_sc_memory
	```

1. Откройте файл *ostis-web-platform/repo.path* и допишите в конец строчку:
	```
	wave_find_path_sc_memory/graph
	```
1. Перейдите в файл *<абсолютный путь к ostis-web-platform>/config/sc-web.ini* и измените в нем строчки Path и Directory на:
	```
	Path = <абсолютный путь к ostis-web-platform>/kb.bin
	Directory = <абсолютный путь к ostis-web-platform>/sc-machine/bin/extensions
	```

### Запуск в VS Code

Для работы Вам необходимо заранее установить расширение CMake Tools. Для его установки необходимо открыть вкладку *Расширения (Extensions) (Ctrl+Shift+X)* и написать в поиске "cmake", после чего открыть нужную вкладку и нажать *Установить (Install)*.

На нижней панели нажатием на *Выбор инструмента (Select kit)* выбрать установленный компилятор. 

1. Запустите Visual Studio Code (VS Code). Запустив редактор, необходимо выбрать пункт меню *File->Open Folder* и в появившемся окне выберите папку *wave_find_path_sc_memory*.

1. Измените переменные в функции *main*

	```
 	params.repo_path = "<абсолютный путь к ostis-web-platform>/kb.bin";
 	params.config_file = "<абсолютный путь к ostis-web-platform>/config/sc-web.ini";
 	params.ext_path = "<абсолютный путь к ostis-web-platform>/sc-machine/bin/extensions";
	```

1. Пересоберите базу знаний 

	```
	cd <абсолютный путь к ostis-web-platform>/scripts/
	./build_kb.sh
	```

1. Соберите проект нажатием на клавишу *Сборка (Build) (F7)* или запустите нажатием на клавишу *Запуск (Run) (Shift+F5)*.

### Запуск в CLion

1. Запустите вашу среду. Запустив среду, необходимо выбрать пункт меню *Open Project* и в появившемся диалоговом окне указать файл *CMakeLists.txt*, находящийся в папке с примером.

1. После этого проект будет виден во вкладке *Редактировать (Edit)*. Перейдите в файл *wavefindpath.cpp*

1. Измените переменные в функции *main*

	```
	params.repo_path = "<абсолютный путь к ostis-web-platform>/kb.bin";
	params.config_file = "<абсолютный путь к ostis-web-platform>/config/sc-web.ini";
	params.ext_path = "<абсолютный путь к ostis-web-platform>/sc-machine/bin/extensions";
	```

1. Соберите проект
1. Пересоберите базу знаний

	```
	cd <абсолютный путь к ostis-web-platform>/scripts/
	./build_kb.sh
	```

1. Перейдите во вкладку *Запуск (Run)*. Найтите строчку *Изменить конфигурацию (Edit configuration)* и нажмите кнопку *Добавить (Add)*. Измените конфигурацию в строчке *Исполняемый (Executable)* нажмите на кнопку *Найти... (Browse...)* выберите файл *wave*, который должен находится по пути *<абсолютный путь к ostis-web-platform>/sc-machine/bin/wave*.

	Примечание: Если файла wave нет, попробуйте выполнить пункт 7. еще раз

1. Нажмите на вкладку *Проект (Project)* и перейдите во вкладку *Запуск (Run)*. Найтите строчку *Изменить конфигурацию (Edit configuration)*. Добавьте в строку *Переменные окружения (Environment variables) «LD_LIBRARY_PATH=<абсолютный путь к ostis-web-platform>/sc-machine/bin»*. Найдите строчку *Рабочая папка (Working directory)*, там путь должен выглядеть следующим образом: *<абсолютный путь к ostis-web-platform>/sc-machine/bin*

1. Сохраните конфигурацию Ctrl+S.

1. Соберите и запустите проект. Сборка проекта — *Ctrl+9*, запуск проекта — *Shift+10*. Программа должна найти пути (если они есть) для графов которые находятся в папке *ostis-example-app/kb/graph*.

### Ручной запуск

1. Измените переменные в функции *main*

	```
	params.repo_path = "<абсолютный путь к ostis-web-platform>/kb.bin";
	params.config_file = "<абсолютный путь к ostis-web-platform>/config/sc-web.ini";
	params.ext_path = "<абсолютный путь к ostis-web-platform>/sc-machine/bin/extensions";
	```

1.  Пересоберите базу знаний

	```
	cd <абсолютный путь к ostis-web-platform>/scripts/
	./build_kb.sh
	```

1. Перейдите в папку с проектом

	```
	cd <абсолютный путь к ostis-web-platform>/wave_find_path_sc_memory/
	```

1. Соберите проект 

	```
	cmake -B build
	cmake --build build
	```

1. При отсутствии ошибок сборки исполняемый файл *wave* будет находиться в папке *<абсолютный путь к ostis-web-platform>/sc-machine/bin*. Перейдя туда, запустите исполняемый файл.

	```
	cd <абсолютный путь к ostis-web-platform>/sc-machine/bin
	./wave
	```

### Представление графов в sc-памяти

В SСs граф G0 состоящий из трех вершин можно записать следующим образом:  	

```cpp
graph -> G0;;

// arcs — множество дуг, nodes — множество вершин
// rrel_arcs — дуга', rrel_nodes — вершина'
G0 -> rrel_arcs: ..arcs;
   -> rrel_nodes: ..nodes;;

..nodes -> V1;
        -> V2;
        -> V3;;

..arcs -> (V1 => V2);
       -> (V2 => V3);;
```

Граф G0 записаный в SCg выглядит следующим образом
![](/docs/graph_example.png)

### Основные функции

*ScAddr* — основной тип данных, предназначенный для хранения адреса *sc-элемента* в памяти.
По этому адресу мы можем хранить:
- sc-узел (*context->CreateNode(ScType)*),
- sc-ссылку (*context->CreateLink(ScType)*)
- sc-дугу (*context->CreateEdge(ScType, const ScAddr, const ScAddr)*).

 1. #### Основные типы элементов

	Основные типы используемых элементов могут быть найдены в официальной документации по [ссылке](https://ostis-dev.github.io/sc-machine/cpp/el_types/)

2. #### Основные функции для работы с sc-памятью:

```cpp
// объявляется переменная printed_vertex, которая содержит адрес узла printed_vertex в 
// sc-памяти (sc-адрес)
ScAddr printed_vertex;

// в sc-памяти создается новый константный sc-узел, sc-адрес которого возвращается 
// переменной printed_vertex
printed_vertex = context->CreateNode(ScType::Const);

// функция создает дугу заданного типа (например, EdgeAccessConstPosPerm) от узла 
// printed_vertex к узлу v1
context->CreateEdge(ScType::EdgeAccessConstPosPerm, printed_vertex, v1);

// функция записывает в graph адрес элемента, имеющего системный идентификатор concept_graph
graph = context->HelperResolveSystemIdtf("concept_graph");

// функция возвращает true, если элемент label (типа ScAddr) существует в sc-памяти, и 
// false, если элемент label удалён
context->IsElement(label);

// функция удаляет элемент по адресу label из sc-памяти
context->EraseElement(label);

// возвращает true, если адрес addr непустой, и false, если адрес addr пустой
// по умолчанию при создации элемента он всегда является пустым
addr.IsValid();

// сохраняет содержимое sc-ссылки addr в sc-поток stream
ScMemoryContext::GetLinkContent(ScAddr const& addr, ScStream& stream);

// передает данные из потока stream в строку outString
ScStream::StreamToString(ScStream const& stream, std::string& outString);

// передает данные из потока stream в sc-ссылку addr
ScMemoryContext::SetLinkContent(ScAddr const& addr, ScStream const& stream);

// Создаёт и возвращает поток из строки str
ScStreamConverter::StreamFromString(std::string const& str);
```

3. #### Операторы сравнения в sc-памяти:

```cpp
// возвращает true, если vertex эквивалентен v1, и false, если vertex не эквивалентен v1
vertex == v1;

// функция возвращает true, если loc не эквивалентен element, и false, если loc 
// эквивалентен element
loc != element;
```

4. #### Итераторы

* **Трёхэлементный итератор (итератор 3)**

	*ScIterator3Ptr* ищет конструкции типа :

	![](/docs/iterator3.png)

	*ScIterator3Ptr* задаётся тремя параметрами, соответствующими элементам в трёхэлементной конструкции: 
	
	```cpp
	// nodes - адрес известного элемента
	// ScType::EdgeAccessConstPosPerm - тип 2-го элемента
	// ScType(0) - тип 3-го элемента
	ScIterator3Ptr nodes_it = context->Iterator3(
				nodes,
				ScType::EdgeAccessConstPosPerm,
				ScType(0)
				);                                         
	```

	Запись *ScType(0)* эквивалентна записи *ScType::Unknown* и соответствует элементу любого типа.

	Результатом работы функции является создание трёхэлементного итератора *nodes_it*, содержащего 	все конструкции, попадающие под переданные на вход функции типы элементов. Таким образом можно получить все элементы, принадлежащие узлу с адресом nodes.

* **Пятиэлементный итератор (итератор 5)**

	*ScIterator5Ptr* ищет конструкции типа : 
	
	![](/docs/iterator5.png)
					    
	*ScIterator5Ptr* задаётся пятью параметрами, соответствующими элементам в пятиэлементной конструкции:

	```cpp
	// graph - адрес известного элемента
	// ScType::EdgeAccessConstPosPerm - тип 2-го элемента
	// ScType(0) - тип 3-го элемента
	// ScType::EdgeAccessConstPosPerm - тип 4-го элемента
	// rrel_nodes - адрес известного элемента
	ScIterator5Ptr it = context->Iterator5(
				graph,
				ScType::EdgeAccessConstPosPerm,
				ScType(0),
				ScType::EdgeAccessConstPosPerm,
				rrel_nodes
				);                                         
	```
			
	Результатом работы функции является создание пятиэлементного итератора *it*, содержащего все конструкции, попадающие под переданные на вход функции типы элементов. При помощи данного итератора можно получать элементы связанные некоторым отношением с заданным.
	

* **Применение итераторов**

	Для получения следующего элемента итератора предусмотрена функция *Next()*, а для получения конкретного элемента конструкции используется функция *Get(<номер элемента>)*.
	
	Так представляется два основных способа работы с итераторами:
	```cpp
	// Проверка итератора на содержание в нём элементов
	if (it->Next())
	{
		// Этот способ удобен для проверки наличия какой-либо конструкции в памяти
		...
	}

	// Обработка всех конструкций, содержащихся в итераторе
	while (it->Next())
	{
		...
	}
	```

	Пример работы итераторов на основе функции вывода всех вершин :

	```cpp
	// Функция выводит на экран все вершины графа graph
	void showAllNodes()
	{
		std::cout << "Vertex: ";
		// Создаём итератор c названием it с пятью элементами (0, 1, 2, 3, 4)
		// 0 - адрес графа
		// 1 - неизвестная дуга принадлежности
		// 2 - неизвестная вершина
		// 3 - неизвестная дуга принадлежности
		// 4 - ролевое отношение вершина'
		ScIterator5Ptr it = context->Iterator5(
			graph,
			ScType::EdgeAccessConstPosPerm,
			ScType(0),
			ScType::EdgeAccessConstPosPerm,
			rrel_nodes
			);
      
		// Переходим к первому элементу итератора it, если он есть
		if(it->Next())
		{
			// В переменную nodes заносим 2 элемент, на который указывает итератор it
			nodes = it->Get(2);
			// В переменной nodes хранится объект nodes, т.к. nodes <- graph связаны
			// отношением rrel_nodes
      
			// Создаем итератор c названием nodes_it с тремя элементами (0, 1, 2)
			// 0 - известный узел
			// 1 - неизвестная дуга принадлежности
			// 2 - неизвестное
			ScIterator3Ptr nodes_it = context->Iterator3(
				nodes,
				ScType::EdgeAccessConstPosPerm,
				ScType(0)
				);
			while(nodes_it->Next()
			{
				// В переменную t_node заносим 2 элемент, на который указывает
                                // итератор it
				ScAddr t_node = nodes_it->Get(2);
				// В переменной t_node хранится вершина графа graph
                                // (вершина может быть любой)

				// Выводим в консоль название этой вершины
				printEl(context, t_node);
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
	```  
