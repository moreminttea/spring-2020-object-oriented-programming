#include <iostream>
#include "hw4part2_MultipleInheritance.h"

int main(void)
{
    //create Work_Study object:
    Work_Study Obj_WS;
    Work_Study *Obj_WS_ptr = &Obj_WS;

    //Work_Study object pointer under influence of casting:
    Student *student_ptr = (Student *) Obj_WS_ptr; //casting Obj_WS_ptr to Student type, then stored in student_ptr
    Employee *employee_ptr = (Employee *) Obj_WS_ptr; //casting Obj_WS_ptr to Employee type, then stored in employee_ptr
    
    //print the addresses for class components:
    std::cout << "Address of A1, Employee Account: " << &(employee_ptr->A1) << std::endl;
    std::cout << "Address of A1, Student Account: " << &(student_ptr->A1) << std::endl;
    std::cout << "Address of E1, Employee: " << &(Obj_WS_ptr->E1) << std::endl;
    std::cout << "Address of S1, Student: " << &(Obj_WS_ptr->S1) << std::endl;
    std::cout << "Address of W1, Work Study: " << &(Obj_WS_ptr->W1) << std::endl;
    
    //print address value of object pointers under the influence of casting:
    std::cout << "Address under Employee Account Casting: " << (Account *) employee_ptr << std::endl; //casting employee_ptr to Account type
    std::cout << "Address under Student Account Casting: " << (Account *) student_ptr << std::endl; //casting student_ptr to Account type
    std::cout << "Address under Employee Casting: " << (Employee *) Obj_WS_ptr << std::endl; //casting Obj_WS_ptr to Employee type
    std::cout << "Address under Student Casting: " << (Student *) Obj_WS_ptr << std::endl; //casting Obj_WS_ptr to Student type
    std::cout << "Address under Work Study Casting: " << (Work_Study *) Obj_WS_ptr << std::endl; //casting Obj_WS_ptr to Work_Study type
    
    return 0;
}
