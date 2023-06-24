#include <iostream>
#include <random>

using namespace std;

typedef struct{ 
    int value;
    void* next;
}node;

node* create_node(int value) {
    node* no = new node;
    no->value = value;
    no->next = nullptr;
    return no;
}

void* create_node(node* tail, int value) {
    // cria um novo nó e salva seu ponteiro no nó anterior
    tail->next = create_node(value);
    return (tail->next);
}

void insert_value(node* head, int index, int value) {
    node* current_node = head;
    for(int i = 0; i <= index; i++){
        // adiciona no index 
        if (current_node->next == nullptr and i == index-1){
            current_node->next = create_node(current_node, value);
        }
        // adiciona 0 caso seja null
        if (current_node->next == nullptr and i < index-1){
            current_node->next = create_node(current_node, 0);
        }
        // substitui o valor
        if(i == index and current_node != nullptr){
            current_node->value = value;
        }else{
            current_node = (node*)(current_node->next);
        }
    }
}

void remove_node(node* head, int index){
    node* last_node = 0;
    node* current_node = head;
    for(int i = 0; i <= index; i++){
        if(i == index){
            // aponta o ponteiro seguinte para o anterior
            last_node->next = current_node->next;
            // apaga o nó desejado
            delete current_node; 
        }else{
            last_node = current_node;
            current_node = (node*)(current_node->next);
        }
    }
}

int len(node* head){
    node* ptr = head;
    int contador = 0;
    // conta enquanto nao achar o final
    while(ptr != 0){
        ptr = (node*) ptr->next;
        contador++;
    }
    delete ptr;
    return contador;
}

node* tail(node** head){
    node* ptr = *head;
    // retorna o ultimo item
    while(ptr->next != 0){
        ptr = (node*) ptr->next;
    }
    return ptr;
}

// cria e retorna um nó novo com referencia a o nó seguinte
node* next_node(node* no){
    node* next = (node*) no->next;
    return next;
}

// altera o ponteiro do nó para o nó seguinte
void next(node** no){
    if(!(*no)->next){ 
        printf("Error. last value is: %d\n", (*no)->value);
        return;
    }
    *no = (node*) (*no)->next;
}

// adiciona o nó head de um array ao tail de outro
void append_node(node* head1, node* head2){
    node* t = tail(&head1);
    t->next = head2;
}

// printa todos os elementos da lista
void print_list(node* head){
    node* current_node = head;
    for(int i = 0; i < len(head); i++){
        printf("%d ", current_node->value);
        current_node = (node*)(current_node->next);
    }
    printf("\n");
}

int read(node* head, int index){
    if(index >= len(head)) return -1;
    node* current_node = head;
    for(int i = 0; i <= index; i++){
        // substitui o valor
        if(i == index)
            return current_node->value;
        else
            current_node = (node*)(current_node->next);
    }
}

void change_values(node* head, int index1, int index2){
    int v1 = read(head, index1);
    int v2 = read(head, index2);
    insert_value(head, index2, v1);
    insert_value(head, index1, v2);
}

int particionamento(node* head, int min, int max){
    int pivo = max -1;
    int pivo_value = read(head, pivo);
    int pe = min;
    int pd = pivo -1;

    while(true){
        while(read(head, pe) <= pivo_value && pe < pivo)
            pe++;
        while(read(head, pd) > pivo_value && pd > pe)
            pd--;

        if(pe >= pd) break;
        else change_values(head, pe, pd);
    }
    change_values(head, pe, pivo);

    return pe;
}

int select(node* head, int min, int max, int busca) {
    if (max - min <= 1) {
        return read(head, min);
    }

    int pivo = particionamento(head, min, max);

    if (pivo < busca)
        return select(head, pivo, max, busca);
    else if (pivo > busca)
        return select(head, min, pivo, busca);
    else
        return read(head, pivo);
}

void quick(node* head, int min, int max){
    if(max - min <= 1) return;
    int pivo = particionamento(head, min, max);
    quick(head, min, pivo);
    quick(head, pivo, max);
}

void quick_sort(node* head){
    quick(head, 0, len(head));
}

int quick_select(node* head, int busca){
    return select(head, 0, len(head), busca);
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-100, 100);

    node* head = create_node(0);
    node* head2 = create_node(0);
    for(int i = 1; i < 10; i++){
        insert_value(head, i, dist(gen));
        insert_value(head2, i, dist(gen));
    }

    printf("Lista 1 inicial:\n");
    print_list(head);

    printf("index 5: %d\n", quick_select(head, 5));

    quick_sort(head);
    printf("Lista 1 organizada:\n");
    print_list(head);

    // procura o proximo item de duas maneiras
    //node* n = tail(&head);
    //next(&n);

    printf("Lista 2 inicial:\n");
    print_list(head2);

    printf("remove item 2 da lista 1: \n");
    remove_node(head, 2);
    print_list(head);

    printf("adiciona o valor 3 no index 6: \n");
    insert_value(head, 6, 3);
    print_list(head);

    printf("indexa as duas listas: \n");
    append_node(head, head2);
    print_list(head);

    quick_sort(head);
    printf("Lista 1 organizada:\n");
    print_list(head);
    return 0;
}