//code written based on tutorial by Michael Sambol youtube https://youtu.be/lJFi5akwsTM?si=1tjozUo5qG6gAGJZ
#include <vector>
#include <cmath>
#include <iostream>
#include <stdexcept>
class FibonacciHeap{
    private:
        struct Node{
            int value;
            int degree;
            bool marked;
            Node* child;
            Node* parent;
            Node* next;
            Node* prev;
            Node(int val): value(val), degree(0), marked(false),child(nullptr), parent(nullptr),next(this), prev(this){}
        };
        int num_elem;
        Node* root_list;
        Node* min;
    public:
        //Konstruktor
        FibonacciHeap(): num_elem(0), root_list(nullptr), min(nullptr){}
        void add_to_root_list(Node* n){
            if(!root_list){
                root_list=n;
            }
            else{
                n->next=root_list;
                n->prev=root_list->prev;
                root_list->prev->next=n;
                root_list->prev=n;
            }
        }
        void remove_from_root_list(Node* n){
            if(n==root_list)root_list = n->next;
                n->prev->next=n->next;
                n->next->prev=n->prev;
        }
        void push(int val){
            Node* newNode = new Node(val);
            add_to_root_list(newNode);
            if(!min||min->value>val) min = newNode;
            num_elem++;
        }
        void add_to_child_list(Node* parent, Node* node){
            if( !parent->child){
                parent->child = node;
            }
            else{
                node->next = parent->child->next;
                node->prev = parent->child;
                parent->child->next->prev = node;
                parent->child->next = node;
            }
        }
        void remove_from_child_list(Node* parent, Node* node){
            if( parent->child==parent->child->next){
                parent->child = nullptr;
            }
            else if(parent->child == node){
                parent->child=node->next;
                node->next->parent=parent;
            }
            node->prev->next=node->next;
            node->next->prev=node->prev;
        }
        void heap_link(Node* y, Node* x){
            remove_from_root_list(y);
            y->prev = y->next = y;
            add_to_child_list(x, y);
            x->degree ++;
            y->parent = x;
            y->marked = false;
        }
        
        void consolidate() {
            int max_degree = static_cast<int>(log(num_elem) * 2);
            std::vector<Node*> A(max_degree, nullptr);

            if (!root_list) {
                return;
            }

            Node* current = root_list;
            Node* end = root_list->prev;

            while (current != end) {
                Node* next_node = current->next;

                int d = current->degree;

                while (A[d] != nullptr) {
                    Node* y = A[d];
                    if (current->value > y->value) {
                        std::swap(current, y);
                    }
                    //większa,mniejsza
                    heap_link(y, current);
                    A[d] = nullptr;
                    //++d;
                    d=current->degree;
                }
                A[d] = current;

                current = next_node;
            }

            // Szukanie nowego minimum
            Node* temp = root_list;
            Node* root_end = root_list->prev;
            min=root_list;
            while(temp!=root_end){
                temp = temp->next;
                if(!min || min->value > temp->value)
                    min=temp;
                
            }

        }
        int pop(){
            if (!min) {
                throw std::out_of_range("FibonacciHeap is empty"); 
            }
            
            Node* min_ptr = min;
            int min_value = min_ptr->value;
            if(min_ptr->child){
                Node* child = min_ptr->child;
                while(true){
                    Node* next_child = child->next;
                    add_to_root_list(child);
                    child->parent = nullptr;

                    if (next_child == min_ptr->child) {
                        break;
                    }
                    child = next_child;
                }
            }
            remove_from_root_list(min_ptr);
            
            if(min_ptr==min_ptr->next){
                min=nullptr;
                root_list=nullptr;
            }
            else{
                min=min_ptr->next;
                consolidate();
            }
            delete min_ptr;
            num_elem--;
            return min_value;

        }
        int top(){
            if(num_elem==0){
                throw std::out_of_range("FibonacciHeap is empty");
            }
            return min->value;
        }
        int size(){
            return num_elem;
        }
        void cut(Node* x, Node* y){
            remove_from_child_list(y,x);
            y->degree--;
            add_to_root_list(x);
            x->parent = nullptr;
            x->marked = false;
        }
        void recursive_cut(Node* y){
            Node* parent=y->parent;
            if(parent){
                if(!y->marked){
                    y->marked = true;
                }
                else{
                    cut(y,parent);
                    recursive_cut(parent);
                }
            }
        }
        void decrease_key(Node* x,int key){
            if(key>x->value) return;
            x->value = key;
            Node* y=x->parent;

            if(y && x->value < y->value){
                cut(x,y);
                recursive_cut(y);
            }
            if(x->value < min->value){
                min = x;
            }
        }
        void remove(Node* x){
            decrease_key(x, std::numeric_limits<int>::min());
            pop();
        }   
};

int main(){
    FibonacciHeap fh;
    int tab[] = {7, 3, 5, 9, 2, 6, 1, 4, 8};

    for(int i = 0; i<9; i++){
        fh.push(tab[i]);
        std::cout<<"Dodadno element: "<<tab[i]<<" Minimum wynosi: "<<fh.top()<<std::endl;
    }

    std::cout<<std::endl<<"pop() kolejnych elementów:"<<std::endl;


    for(int i = 0; i<9; i++){
        std::cout<< fh.pop();
    }
   
    std::cout << std::endl;

    
    

}
