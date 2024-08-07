#include "mld.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

/*Application Structures*/

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

    print_structure_db(struct_db); 

    object_db_t *object_db = calloc(1, sizeof(object_db_t));
    object_db->struct_db = struct_db;

    student_t *John = xcalloc(object_db, "student_t", 1);
    strncpy(John->stud_name, "John", strlen("John"));
    mld_set_dynamic_object_as_root(object_db, John);

    student_t *Jane = xcalloc(object_db, "student_t", 1);
    strncpy(Jane->stud_name, "Jane", strlen("Jane"));
    //John->best_colleage = Jane;

    emp_t *Joseph = xcalloc(object_db, "emp_t", 1);
    strncpy(Joseph->emp_name, "Joseph", strlen("Joseph"));
    mld_set_dynamic_object_as_root(object_db, Joseph);
    Joseph->p = xcalloc(object_db, "int", 1);
    Joseph->p = NULL;

    print_object_db(object_db);

    run_mld_algorithm(object_db);
    printf("Leaked Objects : \n");
    report_leaked_objects(object_db);

    return 0;
}