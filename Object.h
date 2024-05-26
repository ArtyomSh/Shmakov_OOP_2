#pragma once

#include <utility>
#include <cstring>
#include <climits>
#include <cstdlib>

class Object
{
public:
    Object()
    {
        objects_count++;
    }

    Object(const Object &)
    {
        objects_count++;
    }

    Object &operator=(const Object &other)
    {
        if (this != &other)
        {
            objects_count++;
        }
        return *this;
    }

    virtual ~Object()
    {
        objects_count--;
    }

    virtual std::string toString() const = 0;

    static int get_objects_count()
    {
        return Object::objects_count;
    }

private:
    static inline int objects_count;
};

class Named : public virtual Object
{
public:
    explicit Named(std::string data)
    {
        name = std::move(data);
    }
    std::string name;
};

class Task : public virtual Object
{
public:
    Task()
    {
        completed = false;
    };
    virtual ~Task() = default;
    virtual void execute() = 0;
    virtual bool hasOutput() const = 0;
    bool isCompleted() const
    {
        return completed;
    }

protected:
    bool completed;
};

class BinaryTask : public Task, public Named
{
public:
    BinaryTask() : Task(), Named("Binary") {}

    BinaryTask(double first, double second)
        : Task(), Named("Binary"), op1(first), op2(second) {}

    virtual ~BinaryTask() = default;

    bool hasOutput() const override
    {
        return true;
    }

    double getOutput() const
    {
        return output;
    }

    void execute() override
    {
        if (!completed)
        {
            output = executeOperation(op1, op2);
            completed = true;
        }
        else
        {
            throw std::runtime_error("Task already executed");
        }
    }

    std::string toString() const override
    {
        if (isCompleted())
        {
            return name + " task: " + std::to_string(op1) + " " + getOperationSymbol() + " " + std::to_string(op2) + " = " + std::to_string(output);
        }
        else
        {
            return name + " task: need to " + getOperationDescription() + " " + std::to_string(op1) + " and " + std::to_string(op2);
        }
    }

protected:
    virtual double executeOperation(double first, double second) const = 0;
    virtual std::string getOperationSymbol() const = 0;
    virtual std::string getOperationDescription() const = 0;

    double op1 = 0.0;
    double op2 = 0.0;
    double output = 0.0;
};

class AdditionTask : public BinaryTask
{
public:
    AdditionTask(double first, double second) : BinaryTask(first, second) {}

protected:
    double executeOperation(double first, double second) const override
    {
        return first + second;
    }

    std::string getOperationSymbol() const override
    {
        return "+";
    }

    std::string getOperationDescription() const override
    {
        return "add";
    }
};

class MultiplicationTask : public BinaryTask
{
public:
    MultiplicationTask(double first, double second) : BinaryTask(first, second) {}

protected:
    double executeOperation(double first, double second) const override
    {
        return first * second;
    }

    std::string getOperationSymbol() const override
    {
        return "*";
    }

    std::string getOperationDescription() const override
    {
        return "multiply";
    }
};
class SubtractionTask : public BinaryTask
{
public:
    SubtractionTask(double first, double second) : BinaryTask(first, second) {}

protected:
    double executeOperation(double first, double second) const override
    {
        return first - second;
    }

    std::string getOperationSymbol() const override
    {
        return "-";
    }

    std::string getOperationDescription() const override
    {
        return "subtract";
    }
};

class DivisionTask : public BinaryTask
{
public:
    DivisionTask(double first, double second) : BinaryTask(first, second) {}

protected:
    double executeOperation(double first, double second) const override
    {
        if (std::abs(second) < 1e-5)
        {
            throw std::runtime_error("Division by zero");
        }
        return first / second;
    }

    std::string getOperationSymbol() const override
    {
        return "/";
    }

    std::string getOperationDescription() const override
    {
        return "divide";
    }
};

class TaskWithResult : public Task
{
public:
    bool hasOutput() const override
    {
        return true;
    }

    virtual std::string toString() const override
    {
        if (isCompleted())
        {
            return description + std::to_string(output);
        }
        else
        {
            return "This will be task with result";
        }
    }

protected:
    std::string description;
    uint32_t output = 0;
};

class TaskWithoutResult : public Task
{
public:
    bool hasOutput() const override
    {
        return false;
    }

    virtual std::string toString() const override
    {
        if (isCompleted())
        {
            return description;
        }
        else
        {
            return "This will be task without result";
        }
    }

protected:
    std::string description;
};

class CountTasksInContainer : public TaskWithResult
{
public:
    CountTasksInContainer(Container<Task *> *container)
    {
        task_container = container;
    }

    void execute() override
    {
        if (!completed)
        {
            output = task_container->get_size();
            completed = true;
            description = "Count of tasks in container: ";
        }
        else
        {
            throw std::runtime_error("CountTasksInContainer: task already done");
        }
    }

private:
    Container<Task *> *task_container;
};

class TaskAddAnotherTask : public TaskWithoutResult
{
public:
    TaskAddAnotherTask(Task *task, Container<Task *> *container)
    {
        task_container = container;
        new_task = task;
    }

    void execute() override
    {
        if (!completed)
        {
            task_container->push_back(new_task);
            completed = true;
            description = "Task added";
        }
        else
        {
            throw std::runtime_error("TaskAddAnotherTask: task already executed");
        }
    }

private:
    Task *new_task;
    Container<Task *> *task_container;
};

class CountResultTasksInContainer : public TaskWithResult
{
public:
    CountResultTasksInContainer(Container<Task *> *container)
    {
        task_container = container;
    }

    void execute() override
    {
        if (!completed)
        {
            for (auto x : *task_container)
            {
                output += x->hasOutput();
            }
            completed = true;
            description = "Count of result tasks in container: ";
        }
        else
        {
            throw std::runtime_error("CountResultTasksInContainer: task already done");
        }
    }

private:
    Container<Task *> *task_container;
};

class TaskClearContainerTask : public TaskWithoutResult
{
public:
    TaskClearContainerTask(Container<Task *> *container)
    {
        task_container = container;
    }

    void execute() override
    {
        if (!completed)
        {
            task_container->clear();
            completed = true;
            description = "Container has been cleared";
        }
        else
        {
            throw std::runtime_error("TaskClearContainerTask: task already done");
        }
    }

private:
    Container<Task *> *task_container;
};

class CountObjectsTask : public TaskWithResult
{
public:
    CountObjectsTask()
    {
        output = 0;
    }

    void execute() override
    {
        if (!completed)
        {
            output = get_objects_count();
            completed = true;
            description = "Count of objects in program: ";
        }
        else
        {
            throw std::runtime_error("CountObjectsTask: task already done");
        }
    }
};