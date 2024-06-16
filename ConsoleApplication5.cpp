#include <iostream>
#include <string>
#include <cmath>
#include <time.h>
#include <chrono>
#include <string>
using namespace std;

class Object {
public:
    virtual string get_object_name() {
        return "Object";
    }
    virtual Object* clone() = 0;
    virtual ~Object() {};
};

class Point: public Object{
private:
    int x;
    int y;
public:
    Point() : x(0), y(0) {}
    Point(int x, int y): x(x), y(y){}
    Point(const Point& p) : x(p.x), y(p.y) {}
    string get_object_name() override {
        return "Point";
    }

    void change_coords(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    }

    void get_coords() {
        printf("X: %d Y: %d\n", x, y);
    }

    double dist_to(Point& other) {
        return pow((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y), 0.5);
    }

    Point* clone() override {
        return new Point(*this);
    }
    ~Point() override{}
    
};

class Section : public Object {
private:
    Point* pbeg;
    Point* pend;
public:
    Section(): pbeg(nullptr), pend(nullptr){}
    Section(int x1, int y1, int x2, int y2) : pbeg(new Point(x1, y1)), pend(new Point(x2, y2)){}
    Section(const Section& other): pbeg(new Point(*other.pbeg)), pend(new Point(*other.pend)){}
    ~Section() {
        delete pbeg;
        delete pend;
    }
    string get_object_name() override {
        return "Section";
    }
    double get_length() {
        return pend->dist_to(*pbeg);
    }
    Section* clone() override {
        return new Section(*this);
    }

};

class Animal : public Object {
public:
    string get_object_name() override {
        return "Animal";
    }
    Animal* clone() {
        return new Animal(*this);
    }

};

class Dog : public Animal {
private:
    string name;
    int age;
public:
    Dog() :name("null"), age(0){}
    Dog(string name, int age):name(name),age(age){}
    Dog(const Dog& other) :name(other.name), age(other.age){}
    string get_object_name() override {
        return "Dog";
    }
    Dog* clone() {
        return new Dog(*this);
    }
    void sound() {
        cout << "Woof-woof" << endl;
    }
    int get_age() {
        return age;
    }
    string get_name() {
        return name;
    }
};

class Container {
private:
    int capacity;
    int current_size = 0;
    Object** MyContainer;
public:
    Container():capacity(10), current_size(0), MyContainer(new Object*[capacity]){}
    Container(int capacity): capacity(capacity), MyContainer(new Object*[capacity]){}
    
    int get_size() {
        return current_size;
    }

    void push_back(Object* obj) {
        if (current_size < capacity) {
            MyContainer[current_size] = obj;
            current_size++;
        }
        else {
            Object** new_container = new Object*[capacity+10];
            copy_arr(new_container);
            new_container[current_size] = obj;
            delete_arr(MyContainer);
            current_size++;
            MyContainer = new_container;
            capacity += 10;
            
        }
    }

    void push_start(Object* obj) {
        Object** new_container;
        if (current_size < capacity)
            new_container = new Object * [capacity];
        else {
            new_container = new Object * [capacity + 10];
            capacity += 10;
        }

        new_container[0] = obj;
        for (int i = 0; i < current_size; i++) {
            new_container[i + 1] = MyContainer[i]->clone();
        }
        delete_arr(MyContainer);
        MyContainer = new_container;
        current_size++;
    }
    
    void push_middle(Object* obj, int index) {
        Object** new_container;
        if (current_size < capacity)
            new_container = new Object * [capacity];
        else {
            new_container = new Object * [capacity + 10];
            capacity += 10;
        }
        if (index < current_size - 1 && current_size != 0) {
            for (int i = 0; i <= index; i++) {
                new_container[i] = MyContainer[i]->clone();
            }
            new_container[index + 1] = obj;
            for (int i = index + 1; i < current_size; i++) {
                new_container[i + 1] = MyContainer[i]->clone();
            }
        }
        else {
            new_container[current_size] = obj;
            for (int i = 0; i < current_size; i++) {
                new_container[i] = MyContainer[i]->clone();
            }
        }

        delete_arr(MyContainer);
        MyContainer = new_container;
        current_size++;
    }

    void delete_obj(int index) {
        if (current_size >= index + 1) {
            delete MyContainer[index];
            for (int i = index; i < current_size - 1; ++i) {
                MyContainer[i] = MyContainer[i + 1];
            }
            current_size--;
        }
        else {
            printf("No object in %d position\n", index);
        }
    }
    
    Object* get_obj(int index) {
        return MyContainer[index];
    }

    
    
    Object* get_objD(int index) {
        if (current_size >= index + 1 && current_size!=0) {
            Object* buffer = MyContainer[index];
            Object** new_container = new Object * [current_size-1];
            int new_container_index = 0;
            for (int i = 0; i < current_size; i++) {
                if (i != index) {
                    new_container[new_container_index] = MyContainer[i];
                    new_container_index++;
                }

            }
            current_size--;
            MyContainer = new_container;
            return buffer;
        }
        else {
            printf("No object in %d position\n", index);
            return nullptr;
        }
    }
    

    ~Container() {
        for (int i = 0; i < current_size; i++)
            delete MyContainer[i];
        delete[] MyContainer;
    }
private:
    void copy_arr(Object** new_container) {
        for (int i = 0; i < current_size; i++) {
            new_container[i] = MyContainer[i]->clone();
            
        }
        
    }

    void delete_arr(Object** MyContainer) {

        for (int i = 0; i < current_size; i++)
            delete MyContainer[i];
        
    }   
};

void add_random_obj(Container& c) {
    srand(time(NULL));
    int random_number;
    for (int i = 0; i < 10; i++) {
        random_number = rand() % 100;
        if (random_number < 20)
            c.push_back(new Point (rand() % 150, rand() % 150));
        else if (random_number >= 20 && random_number < 40) {
            c.push_back(new Section(rand() % 150, rand() % 150, rand() % 150, rand() % 150));
        }
        else {
            c.push_back(new Dog("Barya", rand()%10));
        }
    }
}

int add_to_random_place(Container &c) {
    srand(time(NULL));
    int random_number = rand()%100;
    int place{};
    if (random_number < 33) {
        c.push_back(new Point(rand() % 150, rand() % 150));
        place = c.get_size() - 1;
    }
    
    else if (random_number >= 33 && random_number < 66) {
        if (c.get_size() != 0) {
            place = rand() % c.get_size();
            c.push_middle(new Section(rand() % 150, rand() % 150, rand() % 150, rand() % 150), place);
            place++;
        }
        else
            c.push_back(new Section(rand() % 150, rand() % 150, rand() % 150, rand() % 150));
        
    }
    
    else {
        c.push_start(new Dog("Barya", rand() % 15));
        place = 0;
    }
    return place;
}

string* delete_random_object(Container& c) {
    srand(time(NULL));
    string* new_s = new string[2];
    if (c.get_size() != 0) {
        int random_number = rand() % c.get_size();
        Object* obj = c.get_objD(random_number);
        if (obj != nullptr) {
            string obj_name = obj->get_object_name();
            new_s[0] = to_string(random_number);
            new_s[1] = obj_name;
            return new_s;
        }
    }
    return nullptr;
    
}

void random_object_method(Container& c) {
    int curr_size = c.get_size();
    if (curr_size > 0) {
        srand(time(NULL));
        int random_number = rand() % c.get_size();
        Object* obj = c.get_obj(random_number);
        int random_number2 = rand() % 100;
        if (random_number2 < 10)
            cout << obj->get_object_name() << endl;
        else {
            if (dynamic_cast<Point*>(obj)) {
                printf("Point[%d]: ", random_number);
                dynamic_cast<Point*>(obj)->get_coords();
            }
            else if (dynamic_cast<Section*>(obj)) {
                printf("Section[%d] length: ", random_number);
                cout << dynamic_cast<Section*>(obj)->get_length() << endl;
            }
            else {
                if (random_number2 < 50)
                    dynamic_cast<Dog*>(obj)->sound();
                else
                    cout << dynamic_cast<Dog*>(obj)->get_age() << endl;
            }
        }
    }
    else
        return;
}

void random_action(Container& c1) {
    int random_number = rand() % 300;
    int choose{};
    int place;
    string* new_s;
    if (random_number < 100)
        choose = 0;
    if (random_number < 200 && random_number >= 100)
        choose = 1;
    if (random_number >= 200 && random_number < 300)
        choose = 2;
    switch (choose) {
    
    case 0:
        place = add_to_random_place(c1);
        printf("%s - Container[%d]: added\n", c1.get_obj(place)->get_object_name().c_str(), place);
        break;

    case 1:
        new_s = delete_random_object(c1);
        if (new_s != nullptr) {
            printf("%s - Container[%s]: deleted\n", new_s[1].c_str(), new_s[0].c_str());
            delete[] new_s;
        }
        break;

    case 2:
        random_object_method(c1);
        break;

    }
}

void first(Container& c1) {
    
    for (int i = 0; i < 100; i++) {
        random_action(c1);
    }
}

void second(Container& c1) {
    
    for (int i = 0; i < 1000; i++) {
        random_action(c1);
    }
}

void third(Container& c1) {
    
    for (int i = 0; i < 10000; i++) 
        random_action(c1);
}


int main()
{
    auto start = std::chrono::steady_clock::now();
    Container c1(10);
    first(c1);
    second(c1);
    third(c1);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "The programm was running:" << duration.count() << "ms";
}

