#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include <stdlib.h> 
#include <conio.h>
#include <nlohmann/json.hpp>

#define confirmmation                         \
    cout << "\nPress any key to continue..."; \
    getch();                                  \

using json = nlohmann::json;
using namespace std;

const string categoriesFilename = "categories.json";
const string todosFilename = "todos.json";

void clearScreen();
time_t convertToTimestamp(const string& dateTimeStr);
string timestampFormattor(time_t timestamp);
time_t getCurrentTimestamp();
time_t getTodayBeginTimestamp();
time_t getTodayLastTimestamp();
time_t getTomorrowBeginTimestamp();
time_t getTomorrowLastTimestamp();

struct ToDo {
    int id;
    string title;
    string description;
    int category;
    time_t deadline;
    bool isDone;
};

struct Category {
    int id;
    string name;
};

struct ToDoControllers {
    json loadTodos(const string& filename);
    void saveTodos(const string& filename, const json& todos);
    int generateTodoId(const json& todos);
    void addTodo(json& todos, ToDo& todo);
    void displayTodos(const json& todos);
    void displayTodosTitle(const json& todos);
    void todayTodos(const json& todos);
    void tommorowTodos(const json& todos);
    void completedTodos(const json& todos);
    void overdueTodos(const json& todos);
    void todosByCategory(const json& todos, int& category);
    string editTodo(json& todos, int& todoId, string& field, string& newValue);
    void markTodoAsDone(json& todos, int todoId);
    void deleteTodo(json& todos, int todoId);
};

struct CategoryControllers {
    json loadCategories(const string& filename);
    void saveCategories(const string& filename, const json& categories);
    int generateCategoryId(const json& categories);
    void newCategory(json& categories, const string& name);
    void displayCategories(const json& categories);
    string getCategoryName(const json& categories, int categoryId);
    string editCategory(json& categories, int categoryId, const string& newName);
    void removeCategory(json& categories, int categoryId);
};

struct View {
    void mainMenu();
    void showTodosMenu();

    // Category Views
    void manageCategories();
    void newCategory();
    void showCategories();
    void editCategory();
    void deleteCategory();

    // ToDo Views
    void toDoTemplate(ToDo& todo);
    void newTodo();
    void showAllTodos();
    void todayTodos();
    void tomorrowTodos();
    void completedTodos();
    void overdueTodos();
    void todosByCategorySelector();
    void todosByCategory(int& categoryID);
    void editTodoSelector();
    void editTodo(int& todoID);
    void markAsDone();
    void deleteTodo();
};

ToDoControllers todo;
json todos = todo.loadTodos(todosFilename);

CategoryControllers category;
json categories = category.loadCategories(categoriesFilename);

int main() {
    View view;

    view.mainMenu();

    return 0;
}

// UTILS
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

time_t convertToTimestamp(const string& dateTimeStr) {
    tm dateTime = {};
    stringstream ss(dateTimeStr);
    ss >> get_time(&dateTime, "%Y-%m-%d %H:%M");
    
    if (ss.fail()) {
        // cerr << "Failed to parse datetime string." << endl;
        return -1;
    }

    return mktime(&dateTime);
}

string timestampFormattor(time_t timestamp) {
    tm* dateTime = localtime(&timestamp);

    if (!dateTime) {
        // cerr << "Failed to convert timestamp to local time." << endl;
        return "Error"; // Error code, adjust as needed
    }

    stringstream ss;
    ss << put_time(dateTime, "%a, %d %b %Y %H:%M:%S");

    return ss.str();
}

time_t getCurrentTimestamp() {
    return time(nullptr);
}

time_t getTodayBeginTimestamp() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    localTime->tm_hour = 0;
    localTime->tm_min = 0;
    localTime->tm_sec = 0;

    return mktime(localTime);
}

time_t getTodayLastTimestamp() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    localTime->tm_hour = 23;
    localTime->tm_min = 59;
    localTime->tm_sec = 59;

    return mktime(localTime);
}

time_t getTomorrowBeginTimestamp() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    localTime->tm_hour = 0;
    localTime->tm_min = 0;
    localTime->tm_sec = 0;

    localTime->tm_mday += 1;

    return mktime(localTime);
}

time_t getTomorrowLastTimestamp() {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    localTime->tm_hour = 23;
    localTime->tm_min = 59;
    localTime->tm_sec = 59;

    localTime->tm_mday += 1;

    return mktime(localTime);
}

// VIEW
void View::mainMenu() {
    clearScreen();
    int choice;

    cout << "TODO LIST APP" << endl
         << "1. New ToDo" << endl
         << "2. Show ToDos" << endl
         << "3. Edit ToDo" << endl
         << "4. Delete ToDo" << endl
         << "5. Manage Categories" << endl
         << "0. Save & Exit" << endl;
    cout << "Choice: ";
    cin >> choice;

    switch(choice) {
    case 1:
        newTodo();
        break;
    case 2:
        showTodosMenu();
        break;
    case 3:
        editTodoSelector();
        break;
    case 4:
        deleteTodo();
        break;
    case 5:
        manageCategories();
        break;
    case 0:
        category.saveCategories(categoriesFilename, categories);
        todo.saveTodos(todosFilename, todos);
        exit(0);
        break;
    default:
        cout << "Invalid choice!" << endl;
        getch();
        mainMenu();
    }
}

// CATEGORY VIEW
void View::manageCategories() {
    clearScreen();
    int choice;

    cout << "MANAGE CATEGORIES" << endl
         << "1. New Category" << endl
         << "2. Show Category" << endl
         << "3. Edit Category" << endl
         << "4. Delete Category" << endl
         << "0. Back to Main Menu" << endl;
    cout << "Choice: ";
    cin >> choice;

    switch(choice) {
    case 1:
        newCategory();
        break;
    case 2:
        showCategories();
        break;
    case 3:
        editCategory();
        break;
    case 4:
        deleteCategory();
        break;
    case 0:
        mainMenu();
    default:
        cout << "Invalid choice!" << endl;
        getch();
        manageCategories();
    }
}

void View::newCategory() {
    string name;
    char confirm;

    clearScreen();
    cout << "NEW CATEGORY" << endl << endl;

    cout << "Name: ";
    cin.ignore();
    getline(cin, name);

    cout << endl << "Are you sure? (Y/n): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        category.newCategory(categories, name);
        cout << "Category created succesfully!" << endl;
        confirmmation;
        manageCategories();
    } else {
        cout << "Create category canceled!" << endl;
        confirmmation;
        manageCategories();
    }
}

void View::showCategories() {
    clearScreen();
    cout << "CATEGORIES" << endl << endl;
    category.displayCategories(categories);
    confirmmation;
    manageCategories();
}

void View::editCategory() {
    int id;
    char confirm;
    string newName;

    clearScreen();
    cout << "EDIT CATEGORIES" << endl << endl;
    category.displayCategories(categories);

    cout << endl;

    cout << "Choice (id): ";
    cin >> id;

    cout << endl;

    if (category.getCategoryName(categories, id) == "Unknown Category") {
        cout << "Unknown Category" << endl;
        confirmmation;
        manageCategories();
    }

    cout << category.getCategoryName(categories, id) << endl;
    cout << "New Name: ";
    cin.ignore();
    getline(cin, newName);

    cout << endl << "Are you sure? (Y/n): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        category.editCategory(categories, id, newName);
        cout << "Category edited succesfully!" << endl;
        confirmmation;
        manageCategories();
    } else {
        cout << "Category editing has ben canceled!" << endl;
        confirmmation;
        manageCategories();
    }
}

void View::deleteCategory() {
    int id;
    char confirm;

    clearScreen();
    cout << "DELETE CATEGORIES" << endl << endl;
    category.displayCategories(categories);

    cout << endl;

    cout << "Choice (id): ";
    cin >> id;

    if (category.getCategoryName(categories, id) == "Unknown Category") {
        cout << endl << "Unknown Category" << endl;
        confirmmation;
        manageCategories();
    }

    cout << endl << "Are you sure? (Y/n): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        category.removeCategory(categories, id);
        cout << "Category deleted succesfully!" << endl;
        confirmmation;
        manageCategories();
    } else {
        cout << "Category deletion has been cancelled!" << endl;
        confirmmation;
        manageCategories();
    }
}


// TODO VIEW
void View::toDoTemplate(ToDo& todo) {
    char status;
    if (todo.isDone) {
        status = 'V';
    } else {
        if (todo.deadline < getCurrentTimestamp()) {
            status = 'X';
        } else {
            status = ' ';
        }
    }
    cout << "[" << status << "] " << timestampFormattor(todo.deadline) << " | " << category.getCategoryName(categories, todo.category) << endl
         << "    " << todo.title << " (" << todo.id << ")" << endl
         << "    " << todo.description << endl
         << endl;
}

void View::showTodosMenu() {
    clearScreen();
    int choice;

    cout << "SHOW TODOS" << endl
         << "1. All" << endl
         << "2. Today" << endl
         << "3. Tommorow" << endl
         << "4. Completed" << endl
         << "5. Overdue" << endl
         << "6. By Category" << endl
         << "0. Back to Main Menu" << endl;
    cout << "Choice: ";
    cin >> choice;

    switch(choice) {
    case 1:
        showAllTodos();
        break;
    case 2:
        todayTodos();
        break;
    case 3:
        tomorrowTodos();
        break;
    case 4:
        completedTodos();
        break;
    case 5:
        overdueTodos();
        break;
    case 6:
        todosByCategorySelector();
        break;
    case 0:
        mainMenu();
    default:
        cout << "Invalid choice!" << endl;
        getch();
        showTodosMenu();
    }
}

void View::newTodo() {
    ToDo todoData;
    string deadlineStr;
    char confirm;

    clearScreen();
    cout << "NEW TODO" << endl << endl;

    cout << "Title: ";
    cin.ignore();
    getline(cin, todoData.title);

    cout << "Description: ";
    // cin.ignore();
    getline(cin, todoData.description);

    cout << endl;

    cout << "Category:" << endl;
    category.displayCategories(categories);

    cout << "Select Category (id): ";
    cin >> todoData.category;

    cout << "Deadline (YYYY-MM-DD hh:mm): ";
    cin.ignore();
    getline(cin, deadlineStr);

    todoData.deadline = convertToTimestamp(deadlineStr);

    todoData.id = todo.generateTodoId(todos);
    todoData.isDone = false;

    cout << endl;

    cout << "Preview:" << endl;

    toDoTemplate(todoData);

    cout << endl << "Are you sure? (Y/n): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        todo.addTodo(todos, todoData);
        cout << "Todo created succesfully!" << endl;
        confirmmation;
        mainMenu();
    } else {
        cout << "Create todo canceled!" << endl;
        confirmmation;
        mainMenu();
    }
}

void View::showAllTodos() {
    int choice;

    clearScreen();
    cout << "ALL TODOS" << endl;
    todo.displayTodos(todos);
    
    cout << "Menu:" << endl
         << "1. Mark as done" << endl
         << "2. Back to ToDo menu" << endl
         << "Choice: ";
    
    cin >> choice;

    switch (choice) {
    case 1:
        markAsDone();
        break;
    case 2:
        showTodosMenu();
        break;
    default:
        cout << "Invalid choice" << endl;
        confirmmation;
        todayTodos();
    }
}

void View::todayTodos() {
    int choice;

    clearScreen();
    cout << "ALL TODOS" << endl;
    todo.todayTodos(todos);
    
    cout << "Menu:" << endl
         << "1. Mark as done" << endl
         << "2. Back to ToDo menu" << endl
         << "Choice: ";
    
    cin >> choice;

    switch (choice) {
    case 1:
        markAsDone();
        break;
    case 2:
        showTodosMenu();
        break;
    default:
        cout << "Invalid choice" << endl;
        confirmmation;
        todayTodos();
    }
}

void View::tomorrowTodos() {
    int choice;

    clearScreen();
    cout << "TODOS FOR TOMORROW" << endl << endl;
    todo.tommorowTodos(todos);

    cout << "Menu:" << endl
         << "1. Mark as done" << endl
         << "2. Back to ToDo menu" << endl
         << "Choice: ";
    
    cin >> choice;

    switch (choice) {
    case 1:
        markAsDone();
        break;
    case 2:
        showTodosMenu();
        break;
    default:
        cout << "Invalid choice" << endl;
        confirmmation;
        todayTodos();
    }
}

void View::completedTodos() {
    int choice;

    clearScreen();
    cout << "COMPLETED TODOS" << endl << endl;
    todo.completedTodos(todos);
    
    cout << "Menu:" << endl
         << "1. Mark as done" << endl
         << "2. Back to ToDo menu" << endl
         << "Choice: ";
    
    cin >> choice;

    switch (choice) {
    case 1:
        markAsDone();
        break;
    case 2:
        showTodosMenu();
        break;
    default:
        cout << "Invalid choice" << endl;
        confirmmation;
        todayTodos();
    }
}

void View::overdueTodos() {
    int choice;

    clearScreen();
    cout << "OVERDUE TODOS" << endl << endl;
    todo.overdueTodos(todos);
    
    cout << "Menu:" << endl
         << "1. Mark as done" << endl
         << "2. Back to ToDo menu" << endl
         << "Choice: ";
    
    cin >> choice;

    switch (choice) {
    case 1:
        markAsDone();
        break;
    case 2:
        showTodosMenu();
        break;
    default:
        cout << "Invalid choice" << endl;
        confirmmation;
        todayTodos();
    }
}

void View::todosByCategorySelector() {
    int id;

    clearScreen();
    cout << "TODOS BY CATEGORY" << endl << endl;

    category.displayCategories(categories);

    cout << endl;

    cout << "Choice (id): ";
    cin >> id;

    todosByCategory(id);
}

void View::todosByCategory(int& categoryID) {
    int choice;

    clearScreen();
    cout << "TODOS | " << category.getCategoryName(categories, categoryID) << endl << endl;
    todo.todosByCategory(todos, categoryID);

    cout << "Menu:" << endl
         << "1. Mark as done" << endl
         << "2. Back to ToDo menu" << endl
         << "Choice: ";
    
    cin >> choice;

    switch (choice) {
    case 1:
        markAsDone();
        break;
    case 2:
        showTodosMenu();
        break;
    default:
        cout << "Invalid choice" << endl;
        confirmmation;
        todayTodos();
    }
}

void View::editTodoSelector() {
    int id;

    clearScreen();
    cout << "EDIT TODO" << endl << endl;

    todo.displayTodosTitle(todos);

    cout << endl;

    cout << "Choice (id): ";
    cin >> id;

    editTodo(id);
}

void View::editTodo(int& todoID) {
    int choice;
    string field, newValue;

    clearScreen();
    cout << "EDIT TODO | " << todoID << endl;

    cout << "1. Title" << endl
         << "2. Description" << endl
         << "3. Category" << endl
         << "4. Deadline" << endl
         << "5. Is Done" << endl
         << "0. Back" << endl;
    
    cout << "Choice: ";
    cin >> choice;


    switch (choice) {
    case 1:
        field = "title";
        cout << "New Title: ";
        cin.ignore();
        getline(cin, newValue);
        cout << todo.editTodo(todos, todoID, field, newValue) << endl;
        confirmmation;
        editTodo(todoID);
        break;
    case 2:
        field = "description";
        cout << "New Description: ";
        cin.ignore();
        getline(cin, newValue);
        cout << todo.editTodo(todos, todoID, field, newValue) << endl;
        confirmmation;
        editTodo(todoID);
        break;
    case 3:
        field = "category";
        category.displayCategories(categories);
        cout << "New Category (id): ";
        cin.ignore();
        getline(cin, newValue);
        cout << todo.editTodo(todos, todoID, field, newValue) << endl;
        confirmmation;
        editTodo(todoID);
        break;
    case 4:
        field = "deadline";
        cout << "Deadline (YYYY-MM-DD hh:mm): ";
        cin.ignore();
        getline(cin, newValue);
        cout << todo.editTodo(todos, todoID, field, newValue) << endl;
        confirmmation;
        editTodo(todoID);
        break;
    case 5:
        field = "isDone";
        cout << "New Is Done (true/false): ";
        cin.ignore();
        getline(cin, newValue);
        cout << todo.editTodo(todos, todoID, field, newValue) << endl;
        confirmmation;
        editTodo(todoID);
        break;
    case 0:
        mainMenu();
    default:
        cout << "Invalid choice!" << endl;
        getch();
        editTodo(todoID);
    }
}

void View::markAsDone() {
    int todoID;
    char confirm;

    cout << "Mark As Done (id): ";
    cin >> todoID;

    cout << endl << "Are you sure? (Y/n): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        todo.markTodoAsDone(todos, todoID);
        cout << "ToDo Mark As Done successfuly!" << endl;
        confirmmation;
        showTodosMenu();
    } else {
        cout << "ToDo Mark As Done has ben canceled!" << endl;
        confirmmation;
        showTodosMenu();
    }

}

void View::deleteTodo() {
    int id;
    char confirm;

    clearScreen();
    cout << "DELETE TODO" << endl << endl;
    todo.displayTodosTitle(todos);

    cout << endl;

    cout << "Choice (id): ";
    cin >> id;

    cout << endl << "Are you sure? (Y/n): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        todo.deleteTodo(todos, id);
        cout << "Todo deleted succesfully!" << endl;
        confirmmation;
        showTodosMenu();
    } else {
        cout << "Todo deletion has been cancelled!" << endl;
        confirmmation;
        showTodosMenu();
    }
}

// CATEGORY CONTROLLERS
json CategoryControllers::loadCategories(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        json data;
        file >> data;
        file.close();
        return data;
    }
    return json::array();
}

void CategoryControllers::saveCategories(const string& filename, const json& categories) {
    ofstream file(filename);
    if (file.is_open()) {
        file << setw(4) << categories << endl;
        file.close();
    }
}

int CategoryControllers::generateCategoryId(const json& categories) {
    if (categories.empty()) {
        return 0;
    }
    return categories.back()["id"].get<int>() + 1;
}

void CategoryControllers::newCategory(json& categories, const string& name) {
    int id = generateCategoryId(categories);
    Category newCategory{ id, name };

    categories.push_back({
        {"id", newCategory.id},
        {"name", newCategory.name}
    });
}

void CategoryControllers::displayCategories(const json& categories) {
    cout << "ID\tNAME" << endl;
    for (const auto& category : categories) {
        string categoryName = category["name"];
        cout << category["id"] << "\t" << categoryName << endl;
    }
}

string CategoryControllers::getCategoryName(const json& categories, int categoryId) {
    for (const auto& category : categories) {
        if (category["id"] == categoryId) {
            return category["name"];
        }
    }
    return "Unknown Category";
}

string CategoryControllers::editCategory(json& categories, int categoryId, const string& newName) {
    for (auto& category : categories) {
        if (category["id"] == categoryId) {
            category["name"] = newName;
            return "Category edited successfully.";
        }
    }
    return "Category not found.";
}

void CategoryControllers::removeCategory(json& categories, int categoryId) {
    auto it = remove_if(categories.begin(), categories.end(), [categoryId](const auto& category) {
        return category["id"] == categoryId;
    });
    categories.erase(it, categories.end());
}

// TODO CONTROLLERS
json ToDoControllers::loadTodos(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        json data;
        file >> data;
        file.close();
        return data;
    }
    return json::array();
}

void ToDoControllers::saveTodos(const string& filename, const json& todos) {
    ofstream file(filename);
    if (file.is_open()) {
        file << std::setw(4) << todos << std::endl;
        file.close();
    }
}

int ToDoControllers::generateTodoId(const json& todos) {
    if (todos.empty()) {
        return 0;
    }
    return todos.back()["id"].get<int>() + 1;
}

void ToDoControllers::addTodo(json& todos, ToDo& todo) {    
    todos.push_back({
        {"id", todo.id},
        {"title", todo.title},
        {"description", todo.description},
        {"category", todo.category},
        {"deadline", todo.deadline},
        {"isDone", todo.isDone}
    });
}

void ToDoControllers::displayTodos(const json& todos) {
    for (const auto& todo : todos) {
        View view;
        ToDo todoData;

        todoData = {
            todo["id"],
            todo["title"],
            todo["description"],
            todo["category"],
            todo["deadline"],
            todo["isDone"]
        };

        view.toDoTemplate(todoData);
    }
}

void ToDoControllers::displayTodosTitle(const json& todos) {
    cout << "ID\tTITLE" << endl;
    for (const auto& todo : todos) {
        string todoTitle = todo["title"];
        cout << todo["id"] << "\t" << todoTitle << endl;
    }
}

void ToDoControllers::todayTodos(const json& todos) {
    for (const auto& todo : todos) {
        if (todo["deadline"] >= getTodayBeginTimestamp() && todo["deadline"] <= getTodayLastTimestamp()) {
            View view;
            ToDo todoData;

            todoData = {
                todo["id"],
                todo["title"],
                todo["description"],
                todo["category"],
                todo["deadline"],
                todo["isDone"]
            };

            view.toDoTemplate(todoData);
        }
    }
}

void ToDoControllers::tommorowTodos(const json& todos) {
    for (const auto& todo : todos) {
        if (todo["deadline"] >= getTomorrowBeginTimestamp() && todo["deadline"] <= getTomorrowLastTimestamp()) {
            View view;
            ToDo todoData;

            todoData = {
                todo["id"],
                todo["title"],
                todo["description"],
                todo["category"],
                todo["deadline"],
                todo["isDone"]
            };

            view.toDoTemplate(todoData);
        }
    }
}

void ToDoControllers::completedTodos(const json& todos) {
    for (const auto& todo : todos) {
        if (todo["isDone"] == true) {
            View view;
            ToDo todoData;

            todoData = {
                todo["id"],
                todo["title"],
                todo["description"],
                todo["category"],
                todo["deadline"],
                todo["isDone"]
            };

            view.toDoTemplate(todoData);
        }
    }
}

void ToDoControllers::overdueTodos(const json& todos) {
    for (const auto& todo : todos) {
        if (todo["isDone"] == false && todo["deadline"] < getCurrentTimestamp()) {
            View view;
            ToDo todoData;

            todoData = {
                todo["id"],
                todo["title"],
                todo["description"],
                todo["category"],
                todo["deadline"],
                todo["isDone"]
            };

            view.toDoTemplate(todoData);
        }
    }
}

void ToDoControllers::todosByCategory(const json& todos, int& category) {
    for (const auto& todo : todos) {
        if (todo["category"] == category) {
            View view;
            ToDo todoData;

            todoData = {
                todo["id"],
                todo["title"],
                todo["description"],
                todo["category"],
                todo["deadline"],
                todo["isDone"]
            };

            view.toDoTemplate(todoData);
        }
    }
}

string ToDoControllers::editTodo(json& todos, int& todoId, string& field, string& newValue) {
    for (auto& todo : todos) {
        if (todo["id"] == todoId) {
            if (field == "title" || field == "description") {
                todo[field] = newValue;
                return "Todo edited successfully.";
            } else if (field == "category") {
                todo[field] = stoi(newValue);
                return "Todo edited successfully.";
            } else if (field == "deadline") {
                todo[field] = convertToTimestamp(newValue);
                return "Todo edited successfully.";
            } else if (field == "isDone") {
                todo[field] = (newValue == "true");
                return "Todo edited successfully.";
            } else {
                return "Invalid field for editing.";
            }
        }
    }

    return "Todo not found.";
}

void ToDoControllers::markTodoAsDone(json& todos, int todoId) {
    for (auto& todo : todos) {
        if (todo["id"] == todoId) {
            todo["isDone"] = true;
            break;
        };
    }
}

void ToDoControllers::deleteTodo(json& todos, int todoId) {
    auto it = remove_if(todos.begin(), todos.end(), [todoId](const auto& todo) {
        return todo["id"] == todoId;
    });
    todos.erase(it, todos.end());
}
