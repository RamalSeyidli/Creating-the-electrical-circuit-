#include <iostream>
#include <fstream> //for

using namespace std;

struct resistor{ //essential node
    char group;
    double value;
    int quantity;
    resistor *next;
};

struct resistor_count{ //this structure is created to apply circuit_info() function as asked
    double value;
    int quantity;
    resistor_count *next;
};

struct circuit{
    resistor *head;
    resistor_count *head_count; //head for resistor_count

    void create();
    void add_resistor(char, double); //upgraded double from int in parameter
    void add_resistor_count(double); //adds resistor node for counting
    void remove_resistor(char, double);
    void delete_resistor(char);
    void circuit_info();
    void print_resistor_count(); //prints resistors
    void clear();
    void clear_count(); //clear of resistor_count
    resistor* search_element(char);  //searches for node
    resistor_count* search_element_count(double); //searches for node to count
};

void circuit::create()
{
    head = NULL;
    head_count = NULL;
}

void circuit::add_resistor(char group, double value)
{
    if(!head || head->group > group) //checks for head if any //changes head if needed
    {
        resistor *new_resistor = new resistor;
        new_resistor->group = group;
        new_resistor->value = value;
        new_resistor->quantity = 1;
        new_resistor->next = head;
        head = new_resistor;
    }
    else
    {
        resistor *found = search_element(group);
        if(found)	//if resistor is already exist
        {
            found->quantity += 1;
        }
        else	//if resistor is not found, this will create new one
        {
            resistor *temp = head;
            while(temp->next != NULL && temp->next->group < group)
            {
                temp = temp->next;
            }

            resistor *new_resistor = new resistor;
            new_resistor->group = group;
            new_resistor->value = value;
            new_resistor->quantity = 1;
            new_resistor->next = temp->next;
            temp->next = new_resistor;
        }
    }
}


void circuit::add_resistor_count(double value)	//adding function for calculating linked list 
{
    if(!head_count || head->value > value)
    {
        resistor_count *new_resistor = new resistor_count;
        new_resistor->value = value;
        new_resistor->quantity = 1;
        new_resistor->next = head_count;
        head_count = new_resistor;
    }
    else	
    {
        resistor_count *found = search_element_count(value);
        if(found)
        {
            found->quantity += 1;
        }
        else
        {
            resistor_count *temp = head_count;
            while(temp->next != NULL && temp->next->value < value)
            {
                temp = temp->next;
            }

            resistor_count *new_resistor = new resistor_count;
            new_resistor->value = value;
            new_resistor->quantity = 1;
            new_resistor->next = temp->next;
            temp->next = new_resistor;
        }
    }
}

void circuit::remove_resistor(char group, double value)	//removes negative inputs
{
    resistor *found = search_element(group);
    if(found)
    {
        if(found->quantity > 1)		//if found decreases the quantity
        {
            found->quantity -= 1;
        }
        else
        {
            delete_resistor(group);		//if quantity gets 0, calls delete function
        }
    }
    else
    {
        cout << "NO_RESISTOR" << '\n';	//if input askes for absent node, gives warning 
    }
}

void circuit::delete_resistor(char group)	//for deleting resistor	//deletes the node connects rest of them
{
    resistor *temp = head;
    resistor *prev = NULL;

    if(temp != NULL && temp->group == group)	//deletes
    {
        head = temp->next;
        delete temp;
        return;
    }

    while(temp != NULL && temp->group != group)	//connects
    {
        prev = temp;
        temp = temp->next;
    }

    prev->next = temp->next;
    delete temp;
}

void circuit::circuit_info()	
{
    resistor *temp = head;
    double total_res = 0;

    while(temp != NULL)
    {
        for(int i=0; i<temp->quantity; i++)
        {
            add_resistor_count(temp->value);
        }
        total_res += temp->value / temp->quantity;
        temp = temp->next;
    }

    print_resistor_count();	//calls for print function

    cout << "Total resistance=" << total_res << " ohm" << '\n';

    clear_count();	//clears count linked list
}

void circuit::print_resistor_count()	//prints output as asked
{
    resistor_count *temp = head_count;
    while(temp != NULL)
    {
        cout << temp->value << " " << temp->quantity << '\n';
        temp = temp->next;
    }
}

void circuit::clear()	//clears the main linked list
{
    resistor *prev = NULL;
    while(head != NULL)
    {
        prev = head;
        head = head->next;
        delete prev;
    }
}

void circuit::clear_count()	//clears the counter linked list
{
    resistor_count *prev = NULL;
    while(head_count != NULL)
    {
        prev = head_count;
        head_count = head_count->next;
        delete prev;
    }
}

resistor* circuit::search_element(char group)	//searches for group of resistor
{
    resistor *temp = head;
    while(temp != NULL)
    {
        if(temp->group == group)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

resistor_count* circuit::search_element_count(double value)		//searches for group of resistor of counter list
{
    resistor_count *temp = head_count;
    while(temp != NULL)
    {
        if(temp->value == value)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

int main(int argc, char *argv[])	//to get the input in compiling session
{
    const char *file_name;
    if(argc > 1)
    {
        file_name = argv[1];
    }
    else
    {
        cout << "No arguments!" << '\n';
        return 0;
    }

    ifstream file(file_name);
    char current_group;
    double current_value;

    circuit cir {};
    cir.create();

    while(file >> current_group >> current_value)
    {
        if(current_value > 0)
        {
            cir.add_resistor(current_group, current_value);
        }
        else if(current_group == 'A' && current_value == 0)
        {
            cir.circuit_info();
        }
        else
        {
            cir.remove_resistor(current_group, current_value);
        }
    }

    cir.clear();

    return 0;
}
