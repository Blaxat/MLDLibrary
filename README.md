
# Memory Leak Detection (MLD)

Memory Leak Detection (MLD) is a project designed to identify and report memory leaks in C applications. This system registers and tracks the dynamic memory allocations of objects, marking them as reachable or leaked depending on whether they're accessible from global variables or root objects in the program.

## Features

- Automatically register memory allocations for tracking.
- Detect unreachable memory blocks during program execution.
- Support for both primitive and complex data types, including user-defined structures.
- Memory leak reports with detailed information about the leaked objects.

## Components

### 1. `struct_db_t` - Structure Database
This module maintains a registry of all the data structures used in the application. Each structure is registered with details about its fields, their types, and sizes.

### 2. `object_db_t` - Object Database
This component stores the actual memory objects (allocated in the heap) and tracks their status (visited/unvisited) during the leak detection process.

### 3. Leak Detection Algorithm
The core of the system. It performs a Depth-First Search (DFS) starting from root objects (reachable objects) and marks them as visited. Unvisited objects after this traversal are considered leaked.

### 4. API Functions
- `xcalloc`: Allocates memory and registers the object in the object database.
- `xfree`: Frees memory and removes the object record from the object database.
- `mld_register_global_object_as_root`: Marks a global object as root to ensure it is reachable.
- `run_mld_algorithm`: Runs the DFS-based leak detection algorithm and marks unreachable objects as leaked.
- `report_leaked_objects`: Prints detailed information about the leaked objects.

## How It Works

1. **Initialization**: The application registers structures and their fields in a structure database (`struct_db`). Each structure's details, including field names, types, and offsets, are stored in the `struct_db`.

2. **Memory Allocation**: Memory objects are dynamically allocated using `xcalloc`. These objects are tracked in the object database (`object_db`).

3. **Leak Detection**: The leak detection algorithm begins by marking all objects as unvisited. It then starts from all root objects and marks all reachable objects as visited using DFS traversal.

4. **Leak Report**: After traversal, any object not marked as visited is reported as leaked. The report includes details such as the object's type, pointer, and field values.

## Example

Here’s a sample usage of MLD in an application that tracks employee and student data:

```c
typedef struct emp_ {
    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
    int *p;
} emp_t;

typedef struct student_{
    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct student_ *best_colleage;
} student_t;

int main(int argc, char **argv){
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t)); 
    mld_init_primitive_data_types(struct_db);
    
    // Register structures
    field_info_t emp_fields[] = { 
        FIELD_INFO(emp_t, emp_name, CHAR,    0),
        FIELD_INFO(emp_t, emp_id,   UINT32,  0),
        FIELD_INFO(emp_t, age,      UINT32,  0),
        FIELD_INFO(emp_t, mgr,      OBJ_PTR, emp_t),
        FIELD_INFO(emp_t, salary,   FLOAT, 0),
        FIELD_INFO(emp_t, p, OBJ_PTR, 0)
    };
    
    REG_STRUCT(struct_db, emp_t, emp_fields); 

    field_info_t stud_fields[] = {
        FIELD_INFO(student_t, stud_name, CHAR, 0),
        FIELD_INFO(student_t, rollno,    UINT32, 0),
        FIELD_INFO(student_t, age,       UINT32, 0),
        FIELD_INFO(student_t, aggregate, FLOAT, 0),
        FIELD_INFO(student_t, best_colleage, OBJ_PTR, student_t)
    };
    REG_STRUCT(struct_db, student_t, stud_fields);

    // Print structure DB
    print_structure_db(struct_db); 

    // Create object database
    object_db_t *object_db = calloc(1, sizeof(object_db_t));
    object_db->struct_db = struct_db;

    // Allocate and track student object
    student_t *John = xcalloc(object_db, "student_t", 1);
    strncpy(John->stud_name, "John", strlen("John"));
    mld_set_dynamic_object_as_root(object_db, John);

    student_t *Jane = xcalloc(object_db, "student_t", 1);
    strncpy(Jane->stud_name, "Jane", strlen("Jane"));

    emp_t *Joseph = xcalloc(object_db, "emp_t", 1);
    strncpy(Joseph->emp_name, "Joseph", strlen("Joseph"));
    mld_set_dynamic_object_as_root(object_db, Joseph);
    Joseph->p = xcalloc(object_db, "int", 1);

    // Print object DB and run leak detection
    print_object_db(object_db);
    run_mld_algorithm(object_db);
    report_leaked_objects(object_db);

    return 0;
}
```

This example shows the registration of `emp_t` and `student_t` structures, the allocation of memory objects, and the running of the leak detection algorithm.

## How to Use

1. Clone the repository.
2. Compile the program with the required header files.
3. Use the provided API to track your memory allocations.
4. Run the program to see if there are any memory leaks in your application.

