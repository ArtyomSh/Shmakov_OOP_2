#include <iostream>
#include <stdexcept>
#include "Container.h"
#include "Object.h"

void addTasks(Container<Task *> *task_container, const std::vector<Task *> &task_list)
{
    for (const auto task : task_list)
    {
        task_container->push_back(task);
    }
}

int main()
{
    auto task_container = Container<Task *>();

    std::vector<Task *>
        task_list = {
            new CountObjectsTask(),
            new SubtractionTask(5, 2),
            new SubtractionTask(4, 8),
            new AdditionTask(813, 23),
            new AdditionTask(8, -2),
            new MultiplicationTask(12, 24),
            new MultiplicationTask(1, 0),
            new TaskAddAnotherTask(new CountResultTasksInContainer(&task_container), &task_container),
            new DivisionTask(1, 3),
            new DivisionTask(0, 8),
            new CountResultTasksInContainer(&task_container),
            new CountTasksInContainer(&task_container),
            new TaskAddAnotherTask(new CountTasksInContainer(&task_container), &task_container),
            new TaskAddAnotherTask(new CountObjectsTask(), &task_container)};

    addTasks(&task_container, task_list);

    auto result_container = new Container<std::string>;

    std::cout << "*-------------------------*\n";
    std::cout << "* Objects in program : " << Object::get_objects_count() << std::endl;
    std::cout << "*-------------------------*\n";

    while (!task_container.is_empty())
    {
        auto tmp = task_container.get_first_element();
        task_container.pop_front();
        tmp->execute();
        auto s = std::string(tmp->toString());
        result_container->push_back(s);
        delete tmp;
    }

    result_container->printList();

    std::cout << "*-------------------------*\n";
    std::cout << "* Objects in program : " << Object::get_objects_count() << std::endl;
    std::cout << "*-------------------------*\n";

    delete result_container;

    return 0;
}